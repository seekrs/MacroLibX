/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 18:03:35 by maldavid          #+#    #+#             */
/*   Updated: 2024/09/14 00:04:29 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <core/errors.h>
#include <renderer/images/texture.h>
#include <renderer/buffers/vk_buffer.h>
#include <renderer/renderer.h>
#include <core/profiler.h>
#include <cstring>

#define STB_IMAGE_IMPLEMENTATION
#ifdef MLX_COMPILER_GCC
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wstringop-overflow"
		#include <stb_image.h>
	#pragma GCC diagnostic pop
#else
	#include <stb_image.h>
#endif

#ifdef IMAGE_OPTIMIZED
	#define TILING VK_IMAGE_TILING_OPTIMAL
#else
	#define TILING VK_IMAGE_TILING_LINEAR
#endif

namespace mlx
{
	void Texture::create(std::uint8_t* pixels, std::uint32_t width, std::uint32_t height, VkFormat format, const char* name, bool dedicated_memory)
	{
		MLX_PROFILE_FUNCTION();
		Image::create(width, height, format, TILING, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, name, dedicated_memory);
		Image::createImageView(VK_IMAGE_VIEW_TYPE_2D, VK_IMAGE_ASPECT_COLOR_BIT);
		Image::createSampler();
		transitionLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		std::vector<Vertex> vertexData = {
			{{0, 0},			{1.f, 1.f, 1.f, 1.f},	{0.0f, 0.0f}},
			{{width, 0},		{1.f, 1.f, 1.f, 1.f},	{1.0f, 0.0f}},
			{{width, height},	{1.f, 1.f, 1.f, 1.f},	{1.0f, 1.0f}},
			{{0, height},		{1.f, 1.f, 1.f, 1.f},	{0.0f, 1.0f}}
		};

		std::vector<std::uint16_t> indexData = { 0, 1, 2, 2, 3, 0 };

		#ifdef DEBUG
			_vbo.create(sizeof(Vertex) * vertexData.size(), vertexData.data(), name);
			_ibo.create(sizeof(std::uint16_t) * indexData.size(), indexData.data(), name);
			_name = name;
		#else
			_vbo.create(sizeof(Vertex) * vertexData.size(), vertexData.data(), nullptr);
			_ibo.create(sizeof(std::uint16_t) * indexData.size(), indexData.data(), nullptr);
		#endif

		Buffer staging_buffer;
		std::size_t size = width * height * formatSize(format);
		if(pixels != nullptr)
		{
			#ifdef DEBUG
				staging_buffer.create(Buffer::kind::dynamic, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, name, pixels);
			#else
				staging_buffer.create(Buffer::kind::dynamic, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, nullptr, pixels);
			#endif
		}
		else
		{
			std::vector<std::uint32_t> default_pixels(width * height, 0x00000000);
			#ifdef DEBUG
				staging_buffer.create(Buffer::kind::dynamic, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, name, default_pixels.data());
			#else
				staging_buffer.create(Buffer::kind::dynamic, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, nullptr, default_pixels.data());
			#endif
		}
		Image::copyFromBuffer(staging_buffer);
		staging_buffer.destroy();
	}

	void Texture::setPixel(int x, int y, std::uint32_t color) noexcept
	{
		MLX_PROFILE_FUNCTION();
		if(x < 0 || y < 0 || static_cast<std::uint32_t>(x) > getWidth() || static_cast<std::uint32_t>(y) > getHeight())
			return;
		if(_map == nullptr)
			openCPUmap();
		_cpu_map[(y * getWidth()) + x] = color;
		_has_been_modified = true;
	}

	int Texture::getPixel(int x, int y) noexcept
	{
		MLX_PROFILE_FUNCTION();
		if(x < 0 || y < 0 || static_cast<std::uint32_t>(x) > getWidth() || static_cast<std::uint32_t>(y) > getHeight())
			return 0;
		if(_map == nullptr)
			openCPUmap();
		std::uint32_t color = _cpu_map[(y * getWidth()) + x];
		std::uint8_t* bytes = reinterpret_cast<std::uint8_t*>(&color);
		std::uint8_t tmp = bytes[0];
		bytes[0] = bytes[2];
		bytes[2] = tmp;
		return *reinterpret_cast<int*>(bytes);
	}

	void Texture::openCPUmap()
	{
		MLX_PROFILE_FUNCTION();
		if(_map != nullptr)
			return;

		#ifdef DEBUG
			core::error::report(e_kind::message, "Texture : enabling CPU mapping");
		#endif
		std::size_t size = getWidth() * getHeight() * formatSize(getFormat());
		_buf_map.emplace();
		#ifdef DEBUG
			_buf_map->create(Buffer::kind::dynamic, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, _name.c_str());
		#else
			_buf_map->create(Buffer::kind::dynamic, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, nullptr);
		#endif
		Image::copyToBuffer(*_buf_map);
		_buf_map->mapMem(&_map);
		_cpu_map = std::vector<std::uint32_t>(getWidth() * getHeight(), 0);
		std::memcpy(_cpu_map.data(), _map, size);
		#ifdef DEBUG
			core::error::report(e_kind::message, "Texture : mapped CPU memory using staging buffer");
		#endif
	}

	void Texture::render(std::array<VkDescriptorSet, 2>& sets, Renderer& renderer, int x, int y)
	{
		MLX_PROFILE_FUNCTION();
		if(_has_been_modified)
		{
			std::memcpy(_map, _cpu_map.data(), _cpu_map.size() * formatSize(getFormat()));
			Image::copyFromBuffer(*_buf_map);
			_has_been_modified = false;
		}
		if(!_set.isInit())
			_set = renderer.getFragDescriptorSet().duplicate();
		if(getLayout() != VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
			transitionLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		if(!_has_set_been_updated)
			updateSet(0);
		auto cmd = renderer.getActiveCmdBuffer();
		_vbo.bind(renderer);
		_ibo.bind(renderer);
		glm::vec2 translate(x, y);
		vkCmdPushConstants(cmd.get(), renderer.getPipeline().getPipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(translate), &translate);
		sets[1] = _set.get();
		vkCmdBindDescriptorSets(renderer.getActiveCmdBuffer().get(), VK_PIPELINE_BIND_POINT_GRAPHICS, renderer.getPipeline().getPipelineLayout(), 0, sets.size(), sets.data(), 0, nullptr);
		vkCmdDrawIndexed(cmd.get(), static_cast<std::uint32_t>(_ibo.getSize() / sizeof(std::uint16_t)), 1, 0, 0, 0);
	}

	void Texture::destroy() noexcept
	{
		MLX_PROFILE_FUNCTION();
		Image::destroy();
		_set.destroy();
		if(_buf_map.has_value())
			_buf_map->destroy();
		_vbo.destroy();
		_ibo.destroy();
	}

	Texture stbTextureLoad(std::filesystem::path file, int* w, int* h)
	{
		MLX_PROFILE_FUNCTION();
		Texture texture;
		int channels;
		std::uint8_t* data = nullptr;
		std::string filename = file.string();

		if(!std::filesystem::exists(std::move(file)))
			core::error::report(e_kind::fatal_error, "Image : file not found '%s'", filename.c_str());
		if(stbi_is_hdr(filename.c_str()))
			core::error::report(e_kind::fatal_error, "Texture : unsupported image format '%s'", filename.c_str());
		int dummy_w;
		int dummy_h;
		data = stbi_load(filename.c_str(), (w == nullptr ? &dummy_w : w), (h == nullptr ? &dummy_h : h), &channels, 4);
		#ifdef DEBUG
			texture.create(data, (w == nullptr ? dummy_w : *w), (h == nullptr ? dummy_h : *h), VK_FORMAT_R8G8B8A8_UNORM, filename.c_str());
		#else
			texture.create(data, (w == nullptr ? dummy_w : *w), (h == nullptr ? dummy_h : *h), VK_FORMAT_R8G8B8A8_UNORM, nullptr);
		#endif
		stbi_image_free(data);
		return texture;
	}
}
