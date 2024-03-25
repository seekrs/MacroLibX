/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/18 17:25:16 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/14 16:34:43 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <renderer/renderer.h>
#include <renderer/images/texture.h>
#include <renderer/core/render_core.h>
#include <core/profiler.h>

namespace mlx
{
	void Renderer::init(Texture* render_target)
	{
		MLX_PROFILE_FUNCTION();
		if(render_target == nullptr)
		{
			_surface.create(*this);
			_swapchain.init(this);
			_pass.init(_swapchain.getImagesFormat(), VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
			for(std::size_t i = 0; i < _swapchain.getImagesNumber(); i++)
				_framebuffers.emplace_back().init(_pass, _swapchain.getImage(i));
		}
		else
		{
			_render_target = render_target;
			_render_target->transitionLayout(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
			_pass.init(_render_target->getFormat(), _render_target->getLayout());
			_framebuffers.emplace_back().init(_pass, *static_cast<Image*>(_render_target));
		}
		_cmd.init();

		for(std::size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
			_semaphores[i].init();

		_uniform_buffer.reset(new UBO);
		#ifdef DEBUG
			_uniform_buffer->create(this, sizeof(glm::mat4), "__mlx_matrices_uniform_buffer_");
		#else
			_uniform_buffer->create(this, sizeof(glm::mat4), nullptr);
		#endif

		_vert_layout.init({
				{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER}
			}, VK_SHADER_STAGE_VERTEX_BIT);
		_frag_layout.init({
				{0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER}
			}, VK_SHADER_STAGE_FRAGMENT_BIT);

		_vert_set.init(this, &Render_Core::get().getDescriptorPool(), &_vert_layout);
		_frag_set.init(this, &Render_Core::get().getDescriptorPool(), &_frag_layout);

		_vert_set.writeDescriptor(0, _uniform_buffer.get());

		_pipeline.init(*this);

		_framebuffer_resized = false;
	}

	bool Renderer::beginFrame()
	{
		MLX_PROFILE_FUNCTION();
		auto device = Render_Core::get().getDevice().get();

		if(_render_target == nullptr)
		{
			_cmd.getCmdBuffer(_current_frame_index).waitForExecution();
			VkResult result = vkAcquireNextImageKHR(device, _swapchain(), UINT64_MAX, _semaphores[_current_frame_index].getImageSemaphore(), VK_NULL_HANDLE, &_image_index);

			if(result == VK_ERROR_OUT_OF_DATE_KHR)
			{
				recreateRenderData();
				return false;
			}
			else if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
				core::error::report(e_kind::fatal_error, "Vulkan error : failed to acquire swapchain image");
		}
		else
		{
			_image_index = 0;
			if(_render_target->getLayout() != VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
				_render_target->transitionLayout(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
		}

		_cmd.getCmdBuffer(_current_frame_index).reset();
		_cmd.getCmdBuffer(_current_frame_index).beginRecord();
		auto& fb = _framebuffers[_image_index];
		_pass.begin(getActiveCmdBuffer(), fb);

		_pipeline.bindPipeline(_cmd.getCmdBuffer(_current_frame_index));

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(fb.getWidth());
		viewport.height = static_cast<float>(fb.getHeight());
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(_cmd.getCmdBuffer(_current_frame_index).get(), 0, 1, &viewport);

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = { fb.getWidth(), fb.getHeight()};
		vkCmdSetScissor(_cmd.getCmdBuffer(_current_frame_index).get(), 0, 1, &scissor);

		return true;
	}

	void Renderer::endFrame()
	{
		MLX_PROFILE_FUNCTION();
		_pass.end(getActiveCmdBuffer());
		_cmd.getCmdBuffer(_current_frame_index).endRecord();

		if(_render_target == nullptr)
		{
			_cmd.getCmdBuffer(_current_frame_index).submit(&_semaphores[_current_frame_index]);

			VkSwapchainKHR swapchain = _swapchain();
			VkSemaphore signalSemaphores[] = { _semaphores[_current_frame_index].getRenderImageSemaphore() };

			VkPresentInfoKHR presentInfo{};
			presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
			presentInfo.waitSemaphoreCount = 1;
			presentInfo.pWaitSemaphores = signalSemaphores;
			presentInfo.swapchainCount = 1;
			presentInfo.pSwapchains = &swapchain;
			presentInfo.pImageIndices = &_image_index;

			VkResult result = vkQueuePresentKHR(Render_Core::get().getQueue().getPresent(), &presentInfo);

			if(result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || _framebuffer_resized)
			{
				_framebuffer_resized = false;
				recreateRenderData();
			}
			else if(result != VK_SUCCESS)
				core::error::report(e_kind::fatal_error, "Vulkan error : failed to present swap chain image");
			_current_frame_index = (_current_frame_index + 1) % MAX_FRAMES_IN_FLIGHT;
		}
		else
		{
			_cmd.getCmdBuffer(_current_frame_index).submitIdle(true);
			_current_frame_index = 0;
		}
	}

	void Renderer::recreateRenderData()
	{
		_swapchain.recreate();
		_pass.destroy();
		_pass.init(_swapchain.getImagesFormat(), VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
		for(auto& fb : _framebuffers)
			fb.destroy();
		_framebuffers.clear();
		for(std::size_t i = 0; i < _swapchain.getImagesNumber(); i++)
			_framebuffers.emplace_back().init(_pass, _swapchain.getImage(i));
	}

	void Renderer::destroy()
	{
		MLX_PROFILE_FUNCTION();
		vkDeviceWaitIdle(Render_Core::get().getDevice().get());

		_pipeline.destroy();
		_uniform_buffer->destroy();
		_vert_layout.destroy();
		_frag_layout.destroy();
		_frag_set.destroy();
		_vert_set.destroy();
		_cmd.destroy();
		_pass.destroy();
		if(_render_target == nullptr)
		{
			_swapchain.destroy();
			_surface.destroy();
		}
		for(auto& fb : _framebuffers)
			fb.destroy();
		for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
			_semaphores[i].destroy();
	}
}
