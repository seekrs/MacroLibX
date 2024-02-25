/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_cmd_buffer.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 18:26:06 by maldavid          #+#    #+#             */
/*   Updated: 2024/02/25 08:02:26 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vk_cmd_buffer.h"
#include <renderer/core/cmd_resource.h>
#include <renderer/core/render_core.h>
#include <renderer/command/cmd_manager.h>
#include <renderer/core/vk_semaphore.h>
#include <renderer/buffers/vk_buffer.h>
#include <renderer/images/vk_image.h>
#include <core/profiler.h>

namespace mlx
{
	bool vector_push_back_if_not_found(std::vector<CmdResource*>& vector, CmdResource* res)
	{
		auto it = std::find_if(vector.begin(), vector.end(), [=](const CmdResource* vres)
		{
			return vres->getUUID() == res->getUUID();
		});

		if(it == vector.end())
		{
			vector.push_back(res);
			return true;
		}
		return false;
	}

	void CmdBuffer::init(kind type, CmdManager* manager)
	{
		init(type, &manager->getCmdPool());
	}

	void CmdBuffer::init(kind type, CmdPool* pool)
	{
		MLX_PROFILE_FUNCTION();
		_type = type;
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
		MLX_PROFILE_FUNCTION();
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

	void CmdBuffer::bindVertexBuffer(Buffer& buffer) noexcept
	{
		MLX_PROFILE_FUNCTION();
		if(!isRecording())
		{
			core::error::report(e_kind::warning, "Vulkan : trying to bind a vertex buffer to a non recording command buffer");
			return;
		}
		VkDeviceSize offset[] = { buffer.getOffset() };
		vkCmdBindVertexBuffers(_cmd_buffer, 0, 1, &buffer.get(), offset);

		buffer.recordedInCmdBuffer();
		vector_push_back_if_not_found(_cmd_resources, &buffer);
	}

	void CmdBuffer::bindIndexBuffer(Buffer& buffer) noexcept
	{
		MLX_PROFILE_FUNCTION();
		if(!isRecording())
		{
			core::error::report(e_kind::warning, "Vulkan : trying to bind a index buffer to a non recording command buffer");
			return;
		}
		vkCmdBindIndexBuffer(_cmd_buffer, buffer.get(), buffer.getOffset(), VK_INDEX_TYPE_UINT16);

		buffer.recordedInCmdBuffer();
		vector_push_back_if_not_found(_cmd_resources, &buffer);
	}

	void CmdBuffer::copyBuffer(Buffer& dst, Buffer& src) noexcept
	{
		MLX_PROFILE_FUNCTION();
		if(!isRecording())
		{
			core::error::report(e_kind::warning, "Vulkan : trying to do a buffer to buffer copy in a non recording command buffer");
			return;
		}

		preTransferBarrier();

		VkBufferCopy copyRegion{};
		copyRegion.size = src.getSize();
		vkCmdCopyBuffer(_cmd_buffer, src.get(), dst.get(), 1, &copyRegion);

		postTransferBarrier();

		dst.recordedInCmdBuffer();
		src.recordedInCmdBuffer();
		vector_push_back_if_not_found(_cmd_resources, &dst);
		vector_push_back_if_not_found(_cmd_resources, &src);
	}

	void CmdBuffer::copyBufferToImage(Buffer& buffer, Image& image) noexcept
	{
		MLX_PROFILE_FUNCTION();
		if(!isRecording())
		{
			core::error::report(e_kind::warning, "Vulkan : trying to do a buffer to image copy in a non recording command buffer");
			return;
		}
		
		preTransferBarrier();

		VkBufferImageCopy region{};
		region.bufferOffset = 0;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;
		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;
		region.imageOffset = { 0, 0, 0 };
		region.imageExtent = { image.getWidth(), image.getHeight(), 1 };

		vkCmdCopyBufferToImage(_cmd_buffer, buffer.get(), image.get(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

		postTransferBarrier();

		image.recordedInCmdBuffer();
		buffer.recordedInCmdBuffer();
		vector_push_back_if_not_found(_cmd_resources, &image);
		vector_push_back_if_not_found(_cmd_resources, &buffer);
	}

	void CmdBuffer::copyImagetoBuffer(Image& image, Buffer& buffer) noexcept
	{
		MLX_PROFILE_FUNCTION();
		if(!isRecording())
		{
			core::error::report(e_kind::warning, "Vulkan : trying to do an image to buffer copy in a non recording command buffer");
			return;
		}

		preTransferBarrier();

		VkBufferImageCopy region{};
		region.bufferOffset = 0;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;
		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;
		region.imageOffset = { 0, 0, 0 };
		region.imageExtent = { image.getWidth(), image.getHeight(), 1 };

		vkCmdCopyImageToBuffer(_cmd_buffer, image.get(), VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, buffer.get(), 1, &region);

		postTransferBarrier();

		image.recordedInCmdBuffer();
		buffer.recordedInCmdBuffer();
		vector_push_back_if_not_found(_cmd_resources, &buffer);
		vector_push_back_if_not_found(_cmd_resources, &image);
	}

	void CmdBuffer::transitionImageLayout(Image& image, VkImageLayout new_layout) noexcept
	{
		MLX_PROFILE_FUNCTION();
		if(!isRecording())
		{
			core::error::report(e_kind::warning, "Vulkan : trying to do an image layout transition in a non recording command buffer");
			return;
		}

		VkImageMemoryBarrier barrier{};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout = image.getLayout();
		barrier.newLayout = new_layout;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image = image.get();
		barrier.subresourceRange.aspectMask = isDepthFormat(image.getFormat()) ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = 1;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;
		barrier.srcAccessMask = layoutToAccessMask(image.getLayout(), false);
		barrier.dstAccessMask = layoutToAccessMask(new_layout, true);
		if(isStencilFormat(image.getFormat()))
			barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;

		VkPipelineStageFlags sourceStage = 0;
		if(barrier.oldLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR)
			sourceStage = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
		else if(barrier.srcAccessMask != 0)
			sourceStage = RCore::accessFlagsToPipelineStage(barrier.srcAccessMask, VK_PIPELINE_STAGE_VERTEX_SHADER_BIT | VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT);
		else
			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;

		VkPipelineStageFlags destinationStage = 0;
		if(barrier.newLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR)
			destinationStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		else if(barrier.dstAccessMask != 0)
			destinationStage = RCore::accessFlagsToPipelineStage(barrier.dstAccessMask, VK_PIPELINE_STAGE_VERTEX_SHADER_BIT | VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT);
		else
			destinationStage = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;

		vkCmdPipelineBarrier(_cmd_buffer, sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);

		image.recordedInCmdBuffer();
		vector_push_back_if_not_found(_cmd_resources, &image);
	}

	void CmdBuffer::endRecord()
	{
		MLX_PROFILE_FUNCTION();
		if(!isInit())
			core::error::report(e_kind::fatal_error, "Vulkan : ending record on un uninit command buffer");
		if(_state != state::recording)
			return;
		if(vkEndCommandBuffer(_cmd_buffer) != VK_SUCCESS)
			core::error::report(e_kind::fatal_error, "Vulkan : failed to end recording command buffer");

		_state = state::idle;
	}

	void CmdBuffer::submitIdle(bool shouldWaitForExecution) noexcept
	{
		MLX_PROFILE_FUNCTION();
		if(_type != kind::single_time)
		{
			core::error::report(e_kind::error, "Vulkan : try to perform an idle submit on a command buffer that is not single-time, this is not allowed");
			return;
		}

		_fence.reset();

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &_cmd_buffer;

		VkResult res = vkQueueSubmit(Render_Core::get().getQueue().getGraphic(), 1, &submitInfo, _fence.get());
		if(res != VK_SUCCESS)
			core::error::report(e_kind::fatal_error, "Vulkan error : failed to submit a single time command buffer, %s", RCore::verbaliseResultVk(res));
		_state = state::submitted;

		if(shouldWaitForExecution)
			waitForExecution();
	}

	void CmdBuffer::submit(Semaphore* semaphores) noexcept
	{
		MLX_PROFILE_FUNCTION();
		std::array<VkSemaphore, 1> signalSemaphores;
		std::array<VkSemaphore, 1> waitSemaphores;

		if(semaphores != nullptr)
		{
			signalSemaphores[0] = semaphores->getRenderImageSemaphore();
			waitSemaphores[0] = semaphores->getImageSemaphore();
		}
		else
		{
			signalSemaphores[0] = VK_NULL_HANDLE;
			waitSemaphores[0] = VK_NULL_HANDLE;
		}
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

		_fence.reset();

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

	void CmdBuffer::updateSubmitState() noexcept
	{
		MLX_PROFILE_FUNCTION();
		if(!_fence.isReady())
			return;

		for(CmdResource* res : _cmd_resources)
			res->removedFromCmdBuffer();
		_cmd_resources.clear();
		_state = state::ready;
	}

	void CmdBuffer::preTransferBarrier() noexcept
	{
		MLX_PROFILE_FUNCTION();
		VkMemoryBarrier memoryBarrier{};
		memoryBarrier.sType = VK_STRUCTURE_TYPE_MEMORY_BARRIER;
		memoryBarrier.pNext = nullptr;
		memoryBarrier.srcAccessMask = 0U;
		memoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

		vkCmdPipelineBarrier(_cmd_buffer, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 1, &memoryBarrier, 0, nullptr, 0, nullptr);
	}

	void CmdBuffer::postTransferBarrier() noexcept
	{
		MLX_PROFILE_FUNCTION();
		VkMemoryBarrier memoryBarrier{};
		memoryBarrier.sType = VK_STRUCTURE_TYPE_MEMORY_BARRIER;
		memoryBarrier.pNext = nullptr;
		memoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		memoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_UNIFORM_READ_BIT;

		vkCmdPipelineBarrier(_cmd_buffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT | VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_VERTEX_SHADER_BIT, 0, 1, &memoryBarrier, 0, nullptr, 0, nullptr);
	}

	void CmdBuffer::destroy() noexcept
	{
		MLX_PROFILE_FUNCTION();
		_fence.destroy();
		_cmd_buffer = VK_NULL_HANDLE;
		_state = state::uninit;
		#ifdef DEBUG
			core::error::report(e_kind::message, "Vulkan : destroyed command buffer");
		#endif
	}
}
