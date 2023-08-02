/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_atlas.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 16:40:09 by maldavid          #+#    #+#             */
/*   Updated: 2023/04/23 12:55:22 by maldavid         ###   ########.fr       */
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
			{ VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT }
		);

		Image::createImageView(VK_IMAGE_VIEW_TYPE_2D, VK_IMAGE_ASPECT_COLOR_BIT);
		Image::createSampler();

		if(pixels != nullptr)
		{
			Buffer staging_buffer;
			std::size_t size = width * height * formatSize(format);
			staging_buffer.create(Buffer::kind::dynamic, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, pixels);
			Image::copyFromBuffer(staging_buffer);
			staging_buffer.destroy();
		}
	}

	void TextureAtlas::render(Renderer& renderer, int x, int y, uint32_t ibo_size)
	{
		auto cmd = renderer.getActiveCmdBuffer().get();

		glm::vec2 translate(x, y);
		vkCmdPushConstants(cmd, renderer.getPipeline().getPipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(translate), &translate);
		vkCmdDrawIndexed(cmd, ibo_size / sizeof(uint16_t), 1, 0, 0, 0);
	}

	void TextureAtlas::destroy() noexcept
	{
		Image::destroy();
	}
}
