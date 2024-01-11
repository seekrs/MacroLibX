/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_framebuffer.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 18:18:06 by maldavid          #+#    #+#             */
/*   Updated: 2024/01/10 21:52:51 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <volk.h>
#include <renderer/core/render_core.h>
#include <renderer/renderer.h>
#include <renderer/images/vk_image.h>

namespace mlx
{
	void FrameBuffer::init(RenderPass& renderpass, Image& image)
	{
		VkImageView attachments[] = { image.getImageView() };

		_width = image.getWidth();
		_height = image.getHeight();

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = renderpass.get();
		framebufferInfo.attachmentCount = 1;
		framebufferInfo.pAttachments = attachments;
		framebufferInfo.width = _width;
		framebufferInfo.height = _height;
		framebufferInfo.layers = 1;

		VkResult res = vkCreateFramebuffer(Render_Core::get().getDevice().get(), &framebufferInfo, nullptr, &_framebuffer);
		if(res != VK_SUCCESS)	
			core::error::report(e_kind::fatal_error, "Vulkan : failed to create a framebuffer, %s", RCore::verbaliseResultVk(res));
		#ifdef DEBUG
			core::error::report(e_kind::message, "Vulkan : created new framebuffer");
		#endif
	}

	void FrameBuffer::destroy() noexcept
	{
		vkDestroyFramebuffer(Render_Core::get().getDevice().get(), _framebuffer, nullptr);
		_framebuffer = VK_NULL_HANDLE;
		#ifdef DEBUG
			core::error::report(e_kind::message, "Vulkan : destroyed a framebuffer");
		#endif
	}
}
