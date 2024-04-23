/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TextureAtlas.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 16:40:09 by maldavid          #+#    #+#             */
/*   Updated: 2024/04/23 21:54:05 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <PreCompiled.h>

#include <Renderer/Images/TextureAtlas.h>

#ifdef IMAGE_OPTIMIZED
	#define TILING VK_IMAGE_TILING_OPTIMAL
#else
	#define TILING VK_IMAGE_TILING_LINEAR
#endif

namespace mlx
{
	void TextureAtlas::Create(std::uint8_t* pixels, std::uint32_t width, std::uint32_t height, VkFormat format, const char* name, bool dedicated_memory)
	{
		Image::Create(width, height, format, TILING, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, name, dedicated_memory);
		Image::CreateImageView(VK_IMAGE_VIEW_TYPE_2D, VK_IMAGE_ASPECT_COLOR_BIT);
		Image::CreateSampler();
		TransitionLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		if(pixels == nullptr)
		{
			Warning("Renderer : creating an empty texture atlas. They cannot be updated after creation, this might be a mistake or a bug, please report");
			return;
		}
		Buffer staging_buffer;
		std::size_t size = width * height * FormatSize(format);
		staging_buffer.Create(BufferType::HighDynamic, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, name, pixels);
		Image::CopyFromBuffer(staging_buffer);
		staging_buffer.Destroy();
	}

	void TextureAtlas::Render(Renderer& renderer, int x, int y, std::uint32_t ibo_size) const
	{
		auto cmd = renderer.GetActiveCmdBuffer().Get();

		glm::vec2 translate(x, y);
		vkCmdPushConstants(cmd, renderer.GetPipeline().GetPipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(translate), &translate);
		vkCmdDrawIndexed(cmd, ibo_size / sizeof(std::uint16_t), 1, 0, 0, 0);
	}

	void TextureAtlas::Destroy() noexcept
	{
		Image::Destroy();
		m_set.Destroy();
	}
}
