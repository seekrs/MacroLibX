/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Texture.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 18:03:35 by maldavid          #+#    #+#             */
/*   Updated: 2024/04/23 21:52:23 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <Precompiled.h>

#include <Renderer/Images/Texture.h>
#include <Renderer/Buffers/Buffer.h>
#include <Renderer/Renderer.h>

#ifdef IMAGE_OPTIMIZED
	#define TILING VK_IMAGE_TILING_OPTIMAL
#else
	#define TILING VK_IMAGE_TILING_LINEAR
#endif

namespace mlx
{
	void Texture::Create(std::uint8_t* pixels, std::uint32_t width, std::uint32_t height, VkFormat format, const char* name, bool dedicated_memory)
	{
		MLX_PROFILE_FUNCTION();
		Image::Create(width, height, format, TILING, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, name, dedicated_memory);
		Image::CreateImageView(VK_IMAGE_VIEW_TYPE_2D, VK_IMAGE_ASPECT_COLOR_BIT);
		Image::CreateSampler();
		TransitionLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		std::vector<Vertex> vertex_data = {
			{{0, 0},			{1.f, 1.f, 1.f, 1.f},	{0.0f, 0.0f}},
			{{width, 0},		{1.f, 1.f, 1.f, 1.f},	{1.0f, 0.0f}},
			{{width, height},	{1.f, 1.f, 1.f, 1.f},	{1.0f, 1.0f}},
			{{0, height},		{1.f, 1.f, 1.f, 1.f},	{0.0f, 1.0f}}
		};

		std::vector<std::uint16_t> index_data = { 0, 1, 2, 2, 3, 0 };

		#ifdef DEBUG
			m_vbo.Create(sizeof(Vertex) * vertex_data.size(), vertex_data.data(), name);
			m_ibo.Create(sizeof(std::uint16_t) * index_data.size(), index_data.data(), name);
			m_name = name;
		#else
			m_vbo.Create(sizeof(Vertex) * vertex_data.size(), vertex_data.data(), nullptr);
			m_ibo.Create(sizeof(std::uint16_t) * index_data.size(), index_data.data(), nullptr);
		#endif

		Buffer staging_buffer;
		std::size_t size = width * height * formatSize(format);
		if(pixels != nullptr)
		{
			#ifdef DEBUG
				staging_buffer.Create(BufferType::HighDynamic, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, name, pixels);
			#else
				staging_buffer.Create(BufferType::HighDynamic, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, nullptr, pixels);
			#endif
		}
		else
		{
			std::vector<std::uint32_t> default_pixels(width * height, 0x00000000);
			#ifdef DEBUG
				staging_buffer.Create(BufferType::HighDynamic, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, name, default_pixels.data());
			#else
				staging_buffer.Create(BufferType::HighDynamic, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, nullptr, default_pixels.data());
			#endif
		}
		Image::CopyFromBuffer(staging_buffer);
		staging_buffer.Destroy();
	}

	void Texture::SetPixel(int x, int y, std::uint32_t color) noexcept
	{
		MLX_PROFILE_FUNCTION();
		if(x < 0 || y < 0 || static_cast<std::uint32_t>(x) > GetWidth() || static_cast<std::uint32_t>(y) > GetHeight())
			return;
		if(m_map == nullptr)
			PpenCPUmap();
		m_cpu_map[(y * GetWidth()) + x] = color;
		m_has_been_modified = true;
	}

	int Texture::GetPixel(int x, int y) noexcept
	{
		MLX_PROFILE_FUNCTION();
		if(x < 0 || y < 0 || static_cast<std::uint32_t>(x) > GetWidth() || static_cast<std::uint32_t>(y) > GetHeight())
			return 0;
		if(m_map == nullptr)
			OpenCPUmap();
		std::uint32_t color = m_cpu_map[(y * GetWidth()) + x];
		std::uint8_t* bytes = reinterpret_cast<std::uint8_t*>(&color);
		std::uint8_t tmp = bytes[0];
		bytes[0] = bytes[2];
		bytes[2] = tmp;
		return *reinterpret_cast<int*>(bytes);
	}

	void Texture::OpenCPUmap()
	{
		MLX_PROFILE_FUNCTION();
		if(m_map != nullptr)
			return;

		DebugLog("Texture : enabling CPU mapping");
		std::size_t size = GetWidth() * GetHeight() * FormatSize(GetFormat());
		m_buf_map.emplace();
		#ifdef DEBUG
			m_buf_map->Create(BufferType::HighDynamic, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, m_name.c_str());
		#else
			m_buf_map->Create(BufferType::HighDynamic, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, nullptr);
		#endif
		Image::CopyToBuffer(*m_buf_map);
		m_buf_map->MapMem(&_map);
		m_cpu_map = std::vector<std::uint32_t>(GetWidth() * GetHeight(), 0);
		std::memcpy(m_cpu_map.data(), m_map, size);
		DebugLog("Texture : mapped CPU memory using staging buffer");
	}

	void Texture::Render(Renderer& renderer, int x, int y)
	{
		MLX_PROFILE_FUNCTION();
		if(m_has_been_modified)
		{
			std::memcpy(m_map, m_cpu_map.data(), m_cpu_map.size() * FormatSize(GetFormat()));
			Image::copyFromBuffer(*m_buf_map);
			m_has_been_modified = false;
		}
		if(!m_set.IsInit())
			m_set = renderer.GetFragDescriptorSet().Duplicate();
		if(GetLayout() != VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
			TransitionLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		if(!m_has_set_been_updated)
			UpdateSet(0);
		auto cmd = renderer.GetActiveCmdBuffer();
		m_vbo.bind(renderer);
		m_ibo.bind(renderer);
		glm::vec2 translate(x, y);
		vkCmdPushConstants(cmd.Get(), renderer.GetPipeline().GetPipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(translate), &translate);
		m_set.Bind();
		vkCmdDrawIndexed(cmd.Get(), static_cast<std::uint32_t>(m_ibo.GetSize() / sizeof(std::uint16_t)), 1, 0, 0, 0);
	}

	void Texture::Destroy() noexcept
	{
		MLX_PROFILE_FUNCTION();
		Image::Destroy();
		m_set.Destroy();
		if(m_buf_map.has_value())
			m_buf_map->Destroy();
		m_vbo.destroy();
		m_ibo.destroy();
	}

	Texture stbTextureLoad(std::filesystem::path file, int* w, int* h)
	{
		MLX_PROFILE_FUNCTION();
		Texture* texture = new Texture;
		int channels;
		std::uint8_t* data = nullptr;
		std::string filename = file.string();

		if(!std::filesystem::exists(std::move(file)))
		{
			Error("Image : file not found '%s'", filename.c_str());
			return nullptr;
		}
		if(stbi_is_hdr(filename.c_str()))
		{
			Error("Texture : unsupported image format '%s'", filename.c_str());
			return nullptr;
		}
		int dummy_w;
		int dummy_h;
		data = stbi_load(filename.c_str(), (w == nullptr ? &dummy_w : w), (h == nullptr ? &dummy_h : h), &channels, 4);
		#ifdef DEBUG
			texture->Create(data, (w == nullptr ? dummy_w : *w), (h == nullptr ? dummy_h : *h), VK_FORMAT_R8G8B8A8_UNORM, filename.c_str());
		#else
			texture->Create(data, (w == nullptr ? dummy_w : *w), (h == nullptr ? dummy_h : *h), VK_FORMAT_R8G8B8A8_UNORM, nullptr);
		#endif
		stbi_image_free(data);
		return texture;
	}
}
