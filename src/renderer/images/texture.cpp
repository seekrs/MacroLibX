/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 18:03:35 by maldavid          #+#    #+#             */
/*   Updated: 2023/04/02 15:47:49 by maldavid         ###   ########.fr       */
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
	void Texture::create(uint8_t* pixels, uint32_t width, uint32_t height, VkFormat format)
	{
		Image::create(width, height, format,
			VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
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
			std::size_t size = width * height * (format == VK_FORMAT_R32G32B32A32_SFLOAT ? 16 : 4);
			staging_buffer.create(Buffer::kind::dynamic, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, pixels);
			Image::copyBuffer(staging_buffer);
			staging_buffer.destroy();
		}
	}

	void Texture::render(Renderer& renderer, int x, int y)
	{
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
		_vbo.destroy();
		_ibo.destroy();
	}

	Texture stbTextureLoad(std::filesystem::path file, int* w, int* h)
	{
		Texture texture;
		int channels;
		VkFormat format;
		uint8_t* data = nullptr;
		std::string filename = file.string();

		if(!std::filesystem::exists(std::move(file)))
			core::error::report(e_kind::fatal_error, "Image : file not found '%s'", filename.c_str());
		if(stbi_is_hdr(filename.c_str()))
		{
			data = (uint8_t*)stbi_loadf(filename.c_str(), w, h, &channels, 4);
			format = VK_FORMAT_R32G32B32A32_SFLOAT;
		}
		else
		{
			data = stbi_load(filename.c_str(), w, h, &channels, 4);
			format = VK_FORMAT_R8G8B8A8_UNORM;
		}
		texture.create(data, *w, *h, format);
		stbi_image_free(data);
		return texture;
	}
}
