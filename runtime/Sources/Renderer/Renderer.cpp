/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Renderer.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/18 17:25:16 by maldavid          #+#    #+#             */
/*   Updated: 2024/04/24 01:58:51 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <PreCompiled.h>

#include <Renderer/Renderer.h>
#include <Renderer/Images/Texture.h>
#include <Renderer/Core/RenderCore.h>

namespace mlx
{
	void Renderer::Init(NonOwningPtr<Texture> render_target)
	{
		MLX_PROFILE_FUNCTION();
		if(!render_target)
		{
			m_surface.Create(*this);
			m_swapchain.Init(this);
			m_pass.Init(m_swapchain.GetImagesFormat(), VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
			for(std::size_t i = 0; i < m_swapchain.GetImagesNumber(); i++)
				m_framebuffers.emplace_back().Init(m_pass, m_swapchain.GetImage(i));
		}
		else
		{
			m_render_target = render_target;
			m_render_target->TransitionLayout(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
			m_pass.Init(m_render_target->GetFormat(), m_render_target->GetLayout());
			m_framebuffers.emplace_back().Init(m_pass, *static_cast<Image*>(m_render_target));
		}
		m_cmd.Init();

		for(std::size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			m_render_finished_semaphores[i].Init();
			m_image_available_semaphores[i].Init();
		}

		m_uniform_buffer.reset(new UniformBuffer);
		#ifdef DEBUG
			m_uniform_buffer->Create(this, sizeof(glm::mat4), "__mlx_matrices_uniform_buffer_");
		#else
			m_uniform_buffer->Create(this, sizeof(glm::mat4), nullptr);
		#endif

		DescriptorSetLayout vert_layout;
		DescriptorSetLayout frag_layout;

		vert_layout.Init({
				{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER}
			}, VK_SHADER_STAGE_VERTEX_BIT);
		frag_layout.Init({
				{0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER}
			}, VK_SHADER_STAGE_FRAGMENT_BIT);

		m_vert_set.Init(this, &RenderCore::Get().GetDescriptorPool(), std::move(vert_layout));
		m_frag_set.Init(this, &RenderCore::Get().GetDescriptorPool(), std::move(frag_layout));

		m_vert_set.WriteDescriptor(0, m_uniform_buffer.Get());

		m_pipeline.Init(*this);

		m_framebuffer_resized = false;
	}

	bool Renderer::BeginFrame()
	{
		MLX_PROFILE_FUNCTION();
		auto device = RenderCore::Get().GetDevice().Get();

		if(!m_render_target)
		{
			m_cmd.GetCmdBuffer(m_current_frame_index).WaitForExecution();
			VkResult result = vkAcquireNextImageKHR(device, m_swapchain.Get(), UINT64_MAX, m_image_available_semaphores[m_current_frame_index].Get(), VK_NULL_HANDLE, &m_image_index);

			if(result == VK_ERROR_OUT_OF_DATE_KHR)
			{
				RecreateRenderData();
				return false;
			}
			else if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
				FatalError("Vulkan error : failed to acquire swapchain image");
		}
		else
		{
			m_image_index = 0;
			if(m_render_target->GetLayout() != VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
				m_render_target->TransitionLayout(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
		}

		m_cmd.GetCmdBuffer(m_current_frame_index).Reset();
		m_cmd.GetCmdBuffer(m_current_frame_index).BeginRecord();
		auto& fb = _framebuffers[_image_index];
		m_pass.Begin(GetActiveCmdBuffer(), fb);

		m_pipeline.BindPipeline(m_cmd.GetCmdBuffer(m_current_frame_index));

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(fb.GetWidth());
		viewport.height = static_cast<float>(fb.GetHeight());
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(m_cmd.GetCmdBuffer(m_current_frame_index).Get(), 0, 1, &viewport);

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = { fb.GetWidth(), fb.GetHeight()};
		vkCmdSetScissor(m_cmd.GetCmdBuffer(m_current_frame_index).Get(), 0, 1, &scissor);

		return true;
	}

	void Renderer::EndFrame()
	{
		MLX_PROFILE_FUNCTION();
		m_pass.End(GetActiveCmdBuffer());
		m_cmd.GetCmdBuffer(m_current_frame_index).EndRecord();

		if(!m_render_target)
		{
			m_cmd.GetCmdBuffer(m_current_frame_index).Submit(&m_render_finished_semaphores[m_current_frame_index]);

			VkSwapchainKHR swapchain = m_swapchain.Get();
			VkSemaphore signal_semaphores[] = { m_render_finished_semaphores[m_current_frame_index].Get() };

			VkPresentInfoKHR present_info{};
			present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
			present_info.waitSemaphoreCount = 1;
			present_info.pWaitSemaphores = signal_semaphores;
			present_info.swapchainCount = 1;
			present_info.pSwapchains = &swapchain;
			present_info.pImageIndices = &m_image_index;
			VkResult result = vkQueuePresentKHR(RenderCore::Get().GetQueue().GetPresent(), &present_info);
			if(result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_framebuffer_resized)
			{
				m_framebuffer_resized = false;
				RecreateRenderData();
			}
			else if(result != VK_SUCCESS)
				FatalError("Vulkan error : failed to present swap chain image");
			m_current_frame_index = (m_current_frame_index + 1) % MAX_FRAMES_IN_FLIGHT;
		}
		else
		{
			m_cmd.GetCmdBuffer(m_current_frame_index).SubmitIdle(true);
			m_current_frame_index = 0;
		}
	}

	void Renderer::RecreateRenderData()
	{
		m_swapchain.Recreate();
		m_pass.Destroy();
		m_pass.Init(m_swapchain.GetImagesFormat(), VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
		for(auto& fb : m_framebuffers)
			fb.Destroy();
		m_framebuffers.clear();
		for(std::size_t i = 0; i < m_swapchain.GetImagesNumber(); i++)
			m_framebuffers.emplace_back().Init(m_pass, m_swapchain.GetImage(i));
	}

	void Renderer::Destroy()
	{
		MLX_PROFILE_FUNCTION();
		vkDeviceWaitIdle(RenderCore::Get().GetDevice().Get());

		m_ipeline.Destroy();
		muniform_buffer->Destroy();
		mvert_layout.Destroy();
		mfrag_layout.Destroy();
		mfrag_set.Destroy();
		mvert_set.Destroy();
		mcmd.Destroy();
		mpass.Destroy();
		if(!m_render_target)
		{
			m_swapchain.Destroy();
			m_surface.Destroy();
		}
		for(auto& fb : m_framebuffers)
			fb.Destroy();
		for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
			m_semaphores[i].Destroy();
	}
}
