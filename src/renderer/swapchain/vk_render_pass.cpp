/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_render_pass.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 18:21:36 by maldavid          #+#    #+#             */
/*   Updated: 2022/12/19 00:06:34 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vk_render_pass.h"
#include <renderer/core/render_core.h>
#include <renderer/renderer.h>

namespace mlx
{
	static const VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };

	void RenderPass::init(Renderer* renderer)
	{
		_renderer = renderer;

		VkAttachmentDescription colorAttachment{};
		colorAttachment.format = renderer->getSwapChain()._swapChainImageFormat;
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference colorAttachmentRef{};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;

		VkRenderPassCreateInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = 1;
		renderPassInfo.pAttachments = &colorAttachment;
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;

		if(vkCreateRenderPass(Render_Core::get().getDevice().get(), &renderPassInfo, nullptr, &_renderPass) != VK_SUCCESS)
			core::error::report(e_kind::fatal_error, "Vulkan : failed to create render pass");
	}

	void RenderPass::begin()
	{
		if(_is_running)
			return;

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = _renderPass;
		renderPassInfo.framebuffer = _renderer->getSwapChain()._framebuffers[_renderer->getImageIndex()].get();
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = _renderer->getSwapChain()._swapChainExtent;
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;

		vkCmdBeginRenderPass(_renderer->getActiveCmdBuffer().get(), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		_is_running = true;
	}

	void RenderPass::end()
	{
		if(!_is_running)
			return;

		vkCmdEndRenderPass(_renderer->getActiveCmdBuffer().get());
		_is_running = false;
	}

	void RenderPass::destroy() noexcept
	{
		vkDestroyRenderPass(Render_Core::get().getDevice().get(), _renderPass, nullptr);
	}
}
