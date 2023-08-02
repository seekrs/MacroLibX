/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/18 17:25:16 by maldavid          #+#    #+#             */
/*   Updated: 2023/04/21 20:56:51 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <mutex>
#include <renderer/renderer.h>
#include <renderer/core/render_core.h>

namespace mlx
{
	void Renderer::init()
	{
		_surface.create(*this);
		_swapchain.init(this);
		_pass.init(this);
		_swapchain.initFB();
		_cmd.init();
		for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
			_semaphores[i].init();
	
		_uniform_buffer.reset(new UBO);
		_uniform_buffer->create(this, sizeof(glm::mat4));

		VkDescriptorPoolSize pool_sizes[] = {
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 4096 },
			{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 4096 }
		};
		_desc_pool.init(2, pool_sizes);
		
		_vert_layout.init({
				{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER}
			}, VK_SHADER_STAGE_VERTEX_BIT);
		_frag_layout.init({
				{0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER}
			}, VK_SHADER_STAGE_FRAGMENT_BIT);
		
		_vert_set.init(this, &_desc_pool, &_vert_layout);
		_frag_set.init(this, &_desc_pool, &_frag_layout);

		_vert_set.writeDescriptor(0, _uniform_buffer.get());

		_pipeline.init(*this);

		_framebufferResized = false;
	}
	
	bool Renderer::beginFrame()
	{
		auto device = Render_Core::get().getDevice().get();

		_cmd.getCmdBuffer(_active_image_index).waitForExecution();
		_cmd.getCmdBuffer(_active_image_index).reset();

		VkResult result = vkAcquireNextImageKHR(device, _swapchain(), UINT64_MAX, _semaphores[_active_image_index].getImageSemaphore(), VK_NULL_HANDLE, &_image_index);

		if(result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			_swapchain.recreate();
			return false;
		}
		else if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
			core::error::report(e_kind::fatal_error, "Vulkan error : failed to acquire swapchain image");

		_cmd.getCmdBuffer(_active_image_index).beginRecord();
		_pass.begin();

		_pipeline.bindPipeline(_cmd.getCmdBuffer(_active_image_index));

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = (float)_swapchain._swapChainExtent.width;
		viewport.height = (float)_swapchain._swapChainExtent.height;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(_cmd.getCmdBuffer(_active_image_index).get(), 0, 1, &viewport);

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = _swapchain._swapChainExtent;
		vkCmdSetScissor(_cmd.getCmdBuffer(_active_image_index).get(), 0, 1, &scissor);

		return true;
	}

	void Renderer::endFrame()
	{
		_pass.end();
		_cmd.getCmdBuffer(_active_image_index).endRecord();
		_cmd.getCmdBuffer(_active_image_index).submit(_semaphores[_active_image_index]);

		VkSwapchainKHR swapchain = _swapchain();
		VkSemaphore signalSemaphores[] = { _semaphores[_active_image_index].getRenderImageSemaphore() };

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &swapchain;
		presentInfo.pImageIndices = &_image_index;

		VkResult result = vkQueuePresentKHR(Render_Core::get().getQueue().getPresent(), &presentInfo);

		if(result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || _framebufferResized)
		{
			_framebufferResized = false;
			_swapchain.recreate();
		}
		else if(result != VK_SUCCESS)
			core::error::report(e_kind::fatal_error, "Vulkan error : failed to present swap chain image");

		_active_image_index = (_active_image_index + 1) % MAX_FRAMES_IN_FLIGHT;
	}

	void Renderer::destroy()
	{
        vkDeviceWaitIdle(Render_Core::get().getDevice().get());

		_pipeline.destroy();
		_uniform_buffer->destroy();
		_vert_layout.destroy();
		_frag_layout.destroy();
		_cmd.destroy();
		_desc_pool.destroy();
		_swapchain.destroyFB();
		_pass.destroy();
		_swapchain.destroy();
		for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
			_semaphores[i].destroy();
		_surface.destroy();
	}
}
