/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/18 17:25:16 by maldavid          #+#    #+#             */
/*   Updated: 2022/12/19 15:27:44 by maldavid         ###   ########.fr       */
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
		_cmd_pool.init();
		
		for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
			_cmd_buffers[i].init(this);

		_semaphore.init(*this);
		_pipeline.init(*this);

		_framebufferResized = false;
	}
	
	bool Renderer::beginFrame()
	{
		auto device = Render_Core::get().getDevice().get();

		vkWaitForFences(device, 1, &_semaphore.getInFlightFence(_active_image_index), VK_TRUE, UINT64_MAX);

		_image_index = 0;
		VkResult result = vkAcquireNextImageKHR(device, _swapchain(), UINT64_MAX, _semaphore.getImageSemaphore(_active_image_index), VK_NULL_HANDLE, &_image_index);

		if(result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			_swapchain.recreate();
			return false;
		}
		else if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
			core::error::report(e_kind::fatal_error, "Vulkan error : failed to acquire swapchain image");

		vkResetFences(device, 1, &_semaphore.getInFlightFence(_active_image_index));

		vkResetCommandBuffer(_cmd_buffers[_active_image_index].get(), 0);

		_cmd_buffers[_active_image_index].beginRecord();
		_pass.begin();

		_pipeline.bindPipeline(_cmd_buffers[_active_image_index]);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = (float)_swapchain._swapChainExtent.width;
		viewport.height = (float)_swapchain._swapChainExtent.height;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(_cmd_buffers[_active_image_index].get(), 0, 1, &viewport);

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = _swapchain._swapChainExtent;
		vkCmdSetScissor(_cmd_buffers[_active_image_index].get(), 0, 1, &scissor);

		return true;
	}

	void Renderer::endFrame()
	{
		_pass.end();
		_cmd_buffers[_active_image_index].endRecord();

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { _semaphore.getImageSemaphore(_active_image_index) };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;

		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &_cmd_buffers[_active_image_index].get();

		VkSemaphore signalSemaphores[] = { _semaphore.getRenderImageSemaphore(_active_image_index) };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		if(vkQueueSubmit(Render_Core::get().getQueue().getGraphic(), 1, &submitInfo, _semaphore.getInFlightFence(_active_image_index)) != VK_SUCCESS)
			core::error::report(e_kind::fatal_error, "Vulkan error : failed to submit draw command buffer");

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &_swapchain();

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
		std::mutex mutex;
        std::unique_lock<std::mutex> watchdog(mutex, std::try_to_lock);

        vkDeviceWaitIdle(Render_Core::get().getDevice().get());

		for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
			_cmd_buffers[i].destroy();

		_pipeline.destroy();
		_swapchain.destroyFB();
		_pass.destroy();
		_swapchain.destroy();
		_semaphore.destroy();
		_cmd_pool.destroy();
		_surface.destroy();
	}
}
