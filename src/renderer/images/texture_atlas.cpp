/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_atlas.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 16:40:09 by maldavid          #+#    #+#             */
/*   Updated: 2023/04/08 18:41:44 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <renderer/images/texture_atlas.h>

namespace mlx
{
	void TextureAtlas::create(uint8_t* pixels, uint32_t width, uint32_t height, VkFormat format)
	{
		Image::create(width, height, format,
			VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
		);

		Image::createImageView(VK_IMAGE_VIEW_TYPE_2D, VK_IMAGE_ASPECT_COLOR_BIT);
		Image::createSampler();

		_color = 0xFFFFFFFF;

		float red = (_color & 0x00FF0000) / 255;
		float green = (_color & 0x0000FF00) / 255;
		float blue = (_color & 0x000000FF) / 255;

		std::vector<Vertex> vertexData = {
			{{0, 0},	{red, green, blue, 1.f},	{0.0f, 0.0f}},
			{{20, 0},	{red, green, blue, 1.f},	{1.0f, 0.0f}},
			{{20, 20},	{red, green, blue, 1.f},	{1.0f, 1.0f}},
			{{0, 20},	{red, green, blue, 1.f},	{0.0f, 1.0f}}
		};

		std::vector<uint16_t> indexData = { 0, 1, 2, 2, 3, 0 };

		_vbo.create(sizeof(Vertex) * vertexData.size());
		_vbo.setData(sizeof(Vertex) * vertexData.size(), vertexData.data());
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

	void TextureAtlas::render(Renderer& renderer, int x, int y, std::array<glm::vec2, 4> pos, std::array<glm::vec2, 4> uv)
	{
		auto cmd = renderer.getActiveCmdBuffer().get();

		float red = (_color & 0x00FF0000) / 255;
		float green = (_color & 0x0000FF00) / 255;
		float blue = (_color & 0x000000FF) / 255;

		std::vector<Vertex> vertexData = {
			{pos[0], {red, green, blue, 1.f}, uv[0]},
			{pos[1], {red, green, blue, 1.f}, uv[1]},
			{pos[2], {red, green, blue, 1.f}, uv[2]},
			{pos[3], {red, green, blue, 1.f}, uv[3]}
		};
		_vbo.setData(sizeof(Vertex) * vertexData.size(), vertexData.data());

		_vbo.bind(renderer);
		_ibo.bind(renderer);
		glm::vec2 translate(x, y);
		vkCmdPushConstants(cmd, renderer.getPipeline().getPipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(translate), &translate);
		vkCmdDrawIndexed(cmd, static_cast<uint32_t>(_ibo.getSize() / sizeof(uint16_t)), 1, 0, 0, 0);
	}

	void TextureAtlas::destroy() noexcept
	{
		Image::destroy();
		_vbo.destroy();
		_ibo.destroy();
	}
}
