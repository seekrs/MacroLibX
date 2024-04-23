/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Renderpass.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 18:21:36 by maldavid          #+#    #+#             */
/*   Updated: 2024/04/23 22:31:09 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pre_compiled.h>

#include "vk_render_pass.h"
#include <renderer/core/render_core.h>
#include <renderer/renderer.h>
#include <renderer/renderpass/vk_framebuffer.h>
#include <core/profiler.h>

namespace mlx
{
	static const VkClearValue clear_color = {{{ 0.f, 0.f, 0.f, 1.0f }}}; // wtf, this mess to satisfy a warning

	void RenderPass::Init(VkFormat attachement_format, VkImageLayout layout)
	{
		VkAttachmentDescription color_attachment{};
		color_attachment.format = attachement_format;
		color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
		color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		color_attachment.finalLayout = layout;

		VkAttachmentReference color_attachment_ref{};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = (layout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR ? VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL : layout);

		VkSubpassDescription subpass1{};
		subpass1.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass1.colorAttachmentCount = 1;
		subpass1.pColorAttachments = &colorAttachmentRef;

		VkSubpassDescription subpasses[] = { subpass1 };

		std::vector<VkSubpassDependency> subpasses_deps;
		subpasses_deps.emplace_back();
		subpasses_deps.back().srcSubpass = VK_SUBPASS_EXTERNAL;
		subpasses_deps.back().dstSubpass = 0;
		subpasses_deps.back().srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
		subpasses_deps.back().dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		subpasses_deps.back().srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
		subpasses_deps.back().dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		subpasses_deps.back().dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

		subpasses_deps.emplace_back();
		subpasses_deps.back().srcSubpass = 0;
		subpasses_deps.back().dstSubpass = VK_SUBPASS_EXTERNAL;
		subpasses_deps.back().srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		subpasses_deps.back().dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
		subpasses_deps.back().srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		subpasses_deps.back().dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
		subpasses_deps.back().dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

		VkRenderPassCreateInfo render_pass_info{};
		render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		render_pass_info.attachmentCount = 1;
		render_pass_info.pAttachments = &color_attachment;
		render_pass_info.subpassCount = sizeof(subpasses) / sizeof(VkSubpassDescription);
		render_pass_info.pSubpasses = subpasses;
		render_pass_info.dependencyCount = static_cast<std::uint32_t>(subpasses_deps.size());
		render_pass_info.pDependencies = subpasses_deps.data();

		VkResult res = vkCreateRenderPass(RenderCore::Get().GetDevice().Get(), &render_pass_info, nullptr, &m_render_pass);
		if(res != VK_SUCCESS)
			FatalError("Vulkan : failed to create render pass, %", VerbaliseVkResult(res));
		DebugLog("Vulkan : created new render pass");
	}

	void RenderPass::Begin(class CommandBuffer& cmd, class FrameBuffer& fb)
	{
		MLX_PROFILE_FUNCTION();
		if(m_is_running)
			return;

		VkRenderPassBeginInfo render_pass_info{};
		render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		render_pass_info.renderPass = m_render_pass;
		render_pass_info.framebuffer = fb.Get();
		render_pass_info.renderArea.offset = { 0, 0 };
		render_pass_info.renderArea.extent = { fb.GetWidth(), fb.GetHeight() };
		render_pass_info.clearValueCount = 1;
		render_pass_info.pClearValues = &clear_color;

		vkCmdBeginRenderPass(cmd.Get(), &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);

		m_is_running = true;
	}

	void RenderPass::End(class CommandBuffer& cmd)
	{
		MLX_PROFILE_FUNCTION();
		if(!m_is_running)
			return;
		vkCmdEndRenderPass(cmdd.Get());
		m_is_running = false;
	}

	void RenderPass::Destroy() noexcept
	{
		vkDestroyRenderPass(RenderCore::Get().GetDevice().Get(), m_render_pass, nullptr);
		m_render_pass = VK_NULL_HANDLE;
		DebugLog("Vulkan : destroyed a renderpass");
	}
}
