/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_render_pass.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 18:21:36 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/14 17:06:01 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vk_render_pass.h"
#include <renderer/core/render_core.h>
#include <renderer/renderer.h>
#include <renderer/renderpass/vk_framebuffer.h>
#include <core/profiler.h>

namespace mlx
{
	static const VkClearValue clearColor = {{{ 0.f, 0.f, 0.f, 1.0f }}}; // wtf, this mess to satisfy a warning

	void RenderPass::init(VkFormat attachement_format, VkImageLayout layout)
	{
		VkAttachmentDescription colorAttachment{};
		colorAttachment.format = attachement_format;
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = layout;

		VkAttachmentReference colorAttachmentRef{};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = (layout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR ? VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL : layout);

		VkSubpassDescription subpass1{};
		subpass1.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass1.colorAttachmentCount = 1;
		subpass1.pColorAttachments = &colorAttachmentRef;

		VkSubpassDescription subpasses[] = { subpass1 };

		std::vector<VkSubpassDependency> subpassesDeps;
		subpassesDeps.emplace_back();
		subpassesDeps.back().srcSubpass = VK_SUBPASS_EXTERNAL;
		subpassesDeps.back().dstSubpass = 0;
		subpassesDeps.back().srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
		subpassesDeps.back().dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		subpassesDeps.back().srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
		subpassesDeps.back().dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		subpassesDeps.back().dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

		subpassesDeps.emplace_back();
		subpassesDeps.back().srcSubpass = 0;
		subpassesDeps.back().dstSubpass = VK_SUBPASS_EXTERNAL;
		subpassesDeps.back().srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		subpassesDeps.back().dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
		subpassesDeps.back().srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		subpassesDeps.back().dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
		subpassesDeps.back().dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

		VkRenderPassCreateInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = 1;
		renderPassInfo.pAttachments = &colorAttachment;
		renderPassInfo.subpassCount = sizeof(subpasses) / sizeof(VkSubpassDescription);
		renderPassInfo.pSubpasses = subpasses;
		renderPassInfo.dependencyCount = static_cast<std::uint32_t>(subpassesDeps.size());
		renderPassInfo.pDependencies = subpassesDeps.data();

		VkResult res = vkCreateRenderPass(Render_Core::get().getDevice().get(), &renderPassInfo, nullptr, &_render_pass);
		if(res != VK_SUCCESS)
			core::error::report(e_kind::fatal_error, "Vulkan : failed to create render pass, %s", RCore::verbaliseResultVk(res));
		#ifdef DEBUG
			core::error::report(e_kind::message, "Vulkan : created new render pass");
		#endif
	}

	void RenderPass::begin(class CmdBuffer& cmd, class FrameBuffer& fb)
	{
		MLX_PROFILE_FUNCTION();
		if(_is_running)
			return;

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = _render_pass;
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
		MLX_PROFILE_FUNCTION();
		if(!_is_running)
			return;
		vkCmdEndRenderPass(cmd.get());
		_is_running = false;
	}

	void RenderPass::destroy() noexcept
	{
		vkDestroyRenderPass(Render_Core::get().getDevice().get(), _render_pass, nullptr);
		_render_pass = VK_NULL_HANDLE;
		#ifdef DEBUG
			core::error::report(e_kind::message, "Vulkan : destroyed a renderpass");
		#endif
	}
}
