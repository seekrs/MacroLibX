/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_cmd_buffer.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 18:26:06 by maldavid          #+#    #+#             */
/*   Updated: 2024/01/03 13:12:58 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vk_cmd_buffer.h"
#include <renderer/core/render_core.h>
#include <renderer/command/cmd_manager.h>
#include <renderer/core/vk_semaphore.h>

namespace mlx
{
	void CmdBuffer::init(CmdManager* manager)
	{
		init(&manager->getCmdPool());
	}

	void CmdBuffer::init(CmdPool* pool)
	{
		_pool = pool;

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = pool->get();
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = 1;

		VkResult res = vkAllocateCommandBuffers(Render_Core::get().getDevice().get(), &allocInfo, &_cmd_buffer);
		if(res != VK_SUCCESS)
			core::error::report(e_kind::fatal_error, "Vulkan : failed to allocate command buffer, %s", RCore::verbaliseResultVk(res));
		#ifdef DEBUG
			core::error::report(e_kind::message, "Vulkan : created new command buffer");
		#endif

		_fence.init();
		_state = state::idle;
	}

	void CmdBuffer::beginRecord(VkCommandBufferUsageFlags usage)
	{
		if(!isInit())
			core::error::report(e_kind::fatal_error, "Vulkan : begenning record on un uninit command buffer");
		if(_state == state::recording)
			return;

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = usage;
		if(vkBeginCommandBuffer(_cmd_buffer, &beginInfo) != VK_SUCCESS)
			core::error::report(e_kind::fatal_error, "Vulkan : failed to begin recording command buffer");

		_state = state::recording;
	}

	void CmdBuffer::endRecord()
	{
		if(!isInit())
			core::error::report(e_kind::fatal_error, "Vulkan : ending record on un uninit command buffer");
		if(_state != state::recording)
			return;
		if(vkEndCommandBuffer(_cmd_buffer) != VK_SUCCESS)
			core::error::report(e_kind::fatal_error, "Vulkan : failed to end recording command buffer");

		_state = state::idle;
	}

	void CmdBuffer::submitIdle() noexcept
	{
		auto device = Render_Core::get().getDevice().get();

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &_cmd_buffer;

		VkFenceCreateInfo fenceCreateInfo = {};
		fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;

		VkFence fence;
		vkCreateFence(device, &fenceCreateInfo, nullptr, &fence);
		vkResetFences(device, 1, &fence);
		VkResult res = vkQueueSubmit(Render_Core::get().getQueue().getGraphic(), 1, &submitInfo, fence);
		if(res != VK_SUCCESS)
			core::error::report(e_kind::fatal_error, "Vulkan error : failed to submit a single time command buffer, %s", RCore::verbaliseResultVk(res));
		_state = state::submitted;
		vkWaitForFences(device, 1, &fence, VK_TRUE, UINT64_MAX);
		vkDestroyFence(device, fence, nullptr);
		_state = state::ready;
	}

	void CmdBuffer::submit(Semaphore* semaphores) noexcept
	{
		std::array<VkSemaphore, 1> signalSemaphores;
		std::array<VkSemaphore, 1> waitSemaphores;

		if(semaphores != nullptr)
		{
			signalSemaphores[0] = semaphores->getRenderImageSemaphore();
			waitSemaphores[0] = semaphores->getImageSemaphore();
		}
		else
		{
			signalSemaphores[0] = nullptr;
			waitSemaphores[0] = nullptr;
		}
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.waitSemaphoreCount = (semaphores == nullptr ? 0 : waitSemaphores.size());
		submitInfo.pWaitSemaphores = waitSemaphores.data();
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &_cmd_buffer;
		submitInfo.signalSemaphoreCount = (semaphores == nullptr ? 0 : signalSemaphores.size());
		submitInfo.pSignalSemaphores = signalSemaphores.data();

		VkResult res = vkQueueSubmit(Render_Core::get().getQueue().getGraphic(), 1, &submitInfo, _fence.get());
		if(res != VK_SUCCESS)
			core::error::report(e_kind::fatal_error, "Vulkan error : failed to submit draw command buffer, %s", RCore::verbaliseResultVk(res));
		_state = state::submitted;
	}

	void CmdBuffer::destroy() noexcept
	{
		_fence.destroy();
		_cmd_buffer = VK_NULL_HANDLE;
		_state = state::uninit;
	}
}
