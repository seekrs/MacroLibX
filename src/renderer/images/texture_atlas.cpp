/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_atlas.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 16:40:09 by maldavid          #+#    #+#             */
/*   Updated: 2024/01/18 10:18:08 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <renderer/images/texture_atlas.h>

#ifdef IMAGE_OPTIMIZED
	#define TILING VK_IMAGE_TILING_OPTIMAL
#else
	#define TILING VK_IMAGE_TILING_LINEAR
#endif

namespace mlx
{
	void TextureAtlas::create(std::uint8_t* pixels, std::uint32_t width, std::uint32_t height, VkFormat format, const char* name, bool dedicated_memory)
	{
		Image::create(width, height, format, TILING, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, name, dedicated_memory);
		Image::createImageView(VK_IMAGE_VIEW_TYPE_2D, VK_IMAGE_ASPECT_COLOR_BIT);
		Image::createSampler();
		transitionLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		if(pixels == nullptr)
		{
			core::error::report(e_kind::warning, "Renderer : creating an empty texture atlas. They cannot be updated after creation, this might be a mistake or a bug, please report");
			return;
		}
		Buffer staging_buffer;
		std::size_t size = width * height * formatSize(format);
		staging_buffer.create(Buffer::kind::dynamic, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, name, pixels);
		Image::copyFromBuffer(staging_buffer);
		staging_buffer.destroy();
	}

	void TextureAtlas::render(Renderer& renderer, int x, int y, std::uint32_t ibo_size) const
	{
		auto cmd = renderer.getActiveCmdBuffer().get();

		glm::vec2 translate(x, y);
		vkCmdPushConstants(cmd, renderer.getPipeline().getPipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(translate), &translate);
		vkCmdDrawIndexed(cmd, ibo_size / sizeof(std::uint16_t), 1, 0, 0, 0);
	}

	void TextureAtlas::destroy() noexcept
	{
		Image::destroy();
		_set.destroy();
	}
}
