/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_framebuffer.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 18:18:06 by maldavid          #+#    #+#             */
/*   Updated: 2022/12/18 20:01:51 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <volk.h>
#include <renderer/core/render_core.h>
#include <renderer/renderer.h>

namespace mlx
{
	void FrameBuffer::init(Renderer& renderer, ImageView& image)
	{
		VkImageView attachments[] = { image() };

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = renderer.getRenderPass().get();
		framebufferInfo.attachmentCount = 1;
		framebufferInfo.pAttachments = attachments;
		framebufferInfo.width = renderer.getSwapChain()._swapChainExtent.width;
		framebufferInfo.height = renderer.getSwapChain()._swapChainExtent.height;
		framebufferInfo.layers = 1;

		if(vkCreateFramebuffer(Render_Core::get().getDevice().get(), &framebufferInfo, nullptr, &_framebuffer) != VK_SUCCESS)	
			core::error::report(e_kind::fatal_error, "Vulkan : failed to create a framebuffer");
	}

	void FrameBuffer::destroy() noexcept
	{
	    vkDestroyFramebuffer(Render_Core::get().getDevice().get(), _framebuffer, nullptr);
	}
}
