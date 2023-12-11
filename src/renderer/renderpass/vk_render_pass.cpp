/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_render_pass.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 18:21:36 by maldavid          #+#    #+#             */
/*   Updated: 2023/12/10 22:32:27 by kbz_8            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vk_render_pass.h"
#include <renderer/core/render_core.h>
#include <renderer/renderer.h>
#include <renderer/renderpass/vk_framebuffer.h>
#include <vulkan/vulkan_core.h>

namespace mlx
{
	static const VkClearValue clearColor = {{{ 0.0f, 0.0f, 0.0f, 1.0f }}}; // wtf, this mess to satisfy a warning

	void RenderPass::init(VkFormat attachement_format)
	{
		VkAttachmentDescription colorAttachment{};
		colorAttachment.format = attachement_format;
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
		#ifdef DEBUG
			core::error::report(e_kind::message, "Vulkan : created new render pass");
		#endif
	}

	void RenderPass::begin(class CmdBuffer& cmd, class FrameBuffer& fb)
	{
		if(_is_running)
			return;

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = _renderPass;
		renderPassInfo.framebuffer = fb.get();
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = { fb.getWidth(), fb.getHeight() };
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;

		vkCmdBeginRenderPass(cmd.get(), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		_is_running = true;
	}

	void RenderPass::end(class CmdBuffer& cmd)
	{
		if(!_is_running)
			return;

		vkCmdEndRenderPass(cmd.get());
		_is_running = false;
	}

	void RenderPass::destroy() noexcept
	{
		vkDestroyRenderPass(Render_Core::get().getDevice().get(), _renderPass, nullptr);
		_renderPass = VK_NULL_HANDLE;
	}
}
