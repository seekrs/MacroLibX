/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Framebuffer.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 18:18:06 by maldavid          #+#    #+#             */
/*   Updated: 2024/04/23 22:28:07 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <PreCompiled.h>

#include <Renderer/Core/RenderCore.h>
#include <Renderer/Renderer.h>
#include <Renderer/Images/Image.h>

namespace mlx
{
	void FrameBuffer::Init(RenderPass& renderpass, Image& image)
	{
		VkImageView attachments[] = { image.GetImageView() };

		m_width = image.GetWidth();
		m_height = image.GetHeight();

		VkFramebufferCreateInfo framebuffer_info{};
		framebuffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebuffer_info.renderPass = renderpass.get();
		framebuffer_info.attachmentCount = 1;
		framebuffer_info.pAttachments = attachments;
		framebuffer_info.width = _width;
		framebuffer_info.height = _height;
		framebuffer_info.layers = 1;

		VkResult res = vkCreateFramebuffer(RenderCore::Get().GetDevice().Get(), &framebuffer_info, nullptr, &m_framebuffer);
		if(res != VK_SUCCESS)	
			FatalError("Vulkan : failed to create a framebuffer, %s", RCore::verbaliseResultVk(res));
		DebugLog("Vulkan : created new framebuffer");
	}

	void FrameBuffer::Destroy() noexcept
	{
		vkDestroyFramebuffer(RenderCore::Get().GetDevice().Get(), m_framebuffer, nullptr);
		m_framebuffer = VK_NULL_HANDLE;
		DebugLog("Vulkan : destroyed a framebuffer");
	}
}
