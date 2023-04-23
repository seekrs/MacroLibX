/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 18:03:35 by maldavid          #+#    #+#             */
/*   Updated: 2023/04/23 15:30:54 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <core/errors.h>
#include <renderer/images/texture.h>
#include <renderer/buffers/vk_buffer.h>
#include <renderer/renderer.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace mlx
{
	void Texture::create(uint8_t* pixels, uint32_t width, uint32_t height, VkFormat format, bool enable_mapping)
	{
		if(enable_mapping)
		{
			#ifdef DEBUG
				core::error::report(e_kind::message, "Texture : creating CPU mappable texture");
			#endif
			Image::create(width, height, format, VK_IMAGE_TILING_LINEAR,
				VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
				{
					VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
					VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
					VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
					VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
				}
			);
		}
		else
		{
			#ifdef DEBUG
				core::error::report(e_kind::message, "Texture : creating non CPU mappable texture");
			#endif
			Image::create(width, height, format, VK_IMAGE_TILING_OPTIMAL,
				VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
				{ VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT }
			);
		}

		Image::createImageView(VK_IMAGE_VIEW_TYPE_2D, VK_IMAGE_ASPECT_COLOR_BIT);
		Image::createSampler();

		std::vector<Vertex> vertexData = {
			{{0, 0},			{1.f, 1.f, 1.f, 1.f},	{0.0f, 0.0f}},
			{{width, 0},		{1.f, 1.f, 1.f, 1.f},	{1.0f, 0.0f}},
			{{width, height},	{1.f, 1.f, 1.f, 1.f},	{1.0f, 1.0f}},
			{{0, height},		{1.f, 1.f, 1.f, 1.f},	{0.0f, 1.0f}}
		};

		std::vector<uint16_t> indexData = { 0, 1, 2, 2, 3, 0 };

		_vbo.create(sizeof(Vertex) * vertexData.size(), vertexData.data());
		_ibo.create(sizeof(uint16_t) * indexData.size(), indexData.data());

		if(pixels != nullptr)
		{
			Buffer staging_buffer;
			std::size_t size = width * height * formatSize(format);
			staging_buffer.create(Buffer::kind::dynamic, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, pixels);
			Image::copyFromBuffer(staging_buffer);
			staging_buffer.destroy();
		}
	}

	void* Texture::openCPUmap()
	{
		if(_cpu_map != nullptr)
			return _cpu_map;

		#ifdef DEBUG
			core::error::report(e_kind::message, "Texture : enabling CPU mapping");
		#endif

		Buffer staging_buffer;
		std::size_t size = getWidth() * getHeight() * formatSize(getFormat());
		staging_buffer.create(Buffer::kind::dynamic, size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT, nullptr);
		Image::copyToBuffer(staging_buffer);

		this->destroy();
		this->create(nullptr, getWidth(), getHeight(), getFormat(), true);

		Image::copyFromBuffer(staging_buffer);
		staging_buffer.destroy();

		if(vkMapMemory(Render_Core::get().getDevice().get(), getDeviceMemory(), 0, VK_WHOLE_SIZE, 0, &_cpu_map) != VK_SUCCESS)
		{
			_buf_map.emplace();
			_buf_map->create(Buffer::kind::dynamic, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT);
			Image::copyToBuffer(*_buf_map);
			_buf_map->mapMem(&_cpu_map);
			#ifdef DEBUG
				core::error::report(e_kind::message, "Texture : mapped CPU memory using staging buffer");
			#endif
		}
		#ifdef DEBUG
			else
				core::error::report(e_kind::message, "Texture : mapped CPU memory using direct memory mapping");
		#endif
		return _cpu_map;
	}

	void Texture::render(Renderer& renderer, int x, int y)
	{
		if(_buf_map.has_value())
			Image::copyFromBuffer(*_buf_map);
		auto cmd = renderer.getActiveCmdBuffer().get();
		_vbo.bind(renderer);
		_ibo.bind(renderer);
		glm::vec2 translate(x, y);
		vkCmdPushConstants(cmd, renderer.getPipeline().getPipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(translate), &translate);
		vkCmdDrawIndexed(cmd, static_cast<uint32_t>(_ibo.getSize() / sizeof(uint16_t)), 1, 0, 0, 0);
	}

	void Texture::destroy() noexcept
	{
		Image::destroy();
		if(_buf_map.has_value())
			_buf_map->destroy();
		_vbo.destroy();
		_ibo.destroy();
	}

	Texture stbTextureLoad(std::filesystem::path file, int* w, int* h)
	{
		Texture texture;
		int channels;
		uint8_t* data = nullptr;
		std::string filename = file.string();

		if(!std::filesystem::exists(std::move(file)))
			core::error::report(e_kind::fatal_error, "Image : file not found '%s'", filename.c_str());
		if(stbi_is_hdr(filename.c_str()))
			core::error::report(e_kind::fatal_error, "Texture : unsupported image format '%s'", filename.c_str());
		data = stbi_load(filename.c_str(), w, h, &channels, 4);
		texture.create(data, *w, *h, VK_FORMAT_R8G8B8A8_UNORM);
		stbi_image_free(data);
		return texture;
	}
}
