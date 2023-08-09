/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 18:03:35 by maldavid          #+#    #+#             */
/*   Updated: 2023/08/09 13:42:48 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <core/errors.h>
#include <renderer/images/texture.h>
#include <renderer/buffers/vk_buffer.h>
#include <renderer/renderer.h>
#include <cstring>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>

#ifdef IMAGE_OPTIMIZED
	#define TILING VK_IMAGE_TILING_OPTIMAL
#else
	#define TILING VK_IMAGE_TILING_LINEAR
#endif

namespace mlx
{
	void Texture::create(uint8_t* pixels, uint32_t width, uint32_t height, VkFormat format)
	{
		Image::create(width, height, format, TILING,
			VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
			{ VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT }
		);

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

	void Texture::setPixel(int x, int y, uint32_t color) noexcept
	{
		if(x < 0 || y < 0 || x > getWidth() || y > getHeight())
			return;
		if(_map == nullptr)
			openCPUmap();
		_cpu_map[(y * getWidth()) + x] = color;
		_has_been_modified = true;
	}

	int Texture::getPixel(int x, int y) noexcept
	{
		if(x < 0 || y < 0 || x > getWidth() || y > getHeight())
			return 0;
		if(_map == nullptr)
			openCPUmap();
		uint32_t color = _cpu_map[(y * getWidth()) + x];
		return (color);
	}

	void Texture::openCPUmap()
	{
		if(_map != nullptr)
			return;

		#ifdef DEBUG
			core::error::report(e_kind::message, "Texture : enabling CPU mapping");
		#endif

		std::size_t size = getWidth() * getHeight() * formatSize(getFormat());
		_buf_map.emplace();
		_buf_map->create(Buffer::kind::dynamic, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT);
		Image::copyToBuffer(*_buf_map);
		_buf_map->mapMem(&_map);
		_cpu_map = std::vector<uint32_t>(getWidth() * getHeight(), 0);
		std::memcpy(_cpu_map.data(), _map, size);
		#ifdef DEBUG
			core::error::report(e_kind::message, "Texture : mapped CPU memory using staging buffer");
		#endif
	}

	void Texture::render(Renderer& renderer, int x, int y)
	{
		if(_has_been_modified)
		{
			std::memcpy(_map, _cpu_map.data(), _cpu_map.size() * formatSize(getFormat()));
			Image::copyFromBuffer(*_buf_map);
			_has_been_modified = false;
		}
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
