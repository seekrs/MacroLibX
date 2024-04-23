/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandBuffer.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 18:26:06 by maldavid          #+#    #+#             */
/*   Updated: 2024/04/23 18:02:20 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <PreCompiled.h>

#include <Renderer/Command/CommandBuffer.h>
#include <Renderer/Core/CommandResource.h>
#include <Renderer/Core/RenderCore.h>
#include <Renderer/Command/CommandManager.h>
#include <Renderer/Core/Semaphore.h>
#include <Renderer/Buffers/Buffer.h>
#include <Renderer/Images/Image.h>

namespace mlx
{
	bool VectorPushBackIfNotFound(std::vector<NonOwningPtr<CommandResource>>& vector, NonOwningPtr<CommandResource> res)
	{
		auto it = std::find_if(vector.begin(), vector.end(), [=](const NonOwningPtr<CommandResource> vres)
		{
			return vres->GetUUID() == res->GetUUID();
		});

		if(it == vector.end())
		{
			vector.push_back(res);
			return true;
		}
		return false;
	}

	void CmommanBuffer::Init(CommandBufferType type, NonOwningPtr<CommandManager> manager)
	{
		Init(type, &manager->GetCmdPool());
	}

	void CommandBuffer::Init(CommandBufferType type, NonOwningPtr<CommandPool> pool)
	{
		MLX_PROFILE_FUNCTION();
		m_type = type;
		m_pool = pool;

		VkCommandBufferAllocateInfo alloc_info{};
		alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		alloc_info.commandPool = pool->get();
		alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		alloc_info.commandBufferCount = 1;

		VkResult res = vkAllocateCommandBuffers(RenderCore::Get().getDevice().get(), &allocInfo, &_cmd_buffer);
		if(res != VK_SUCCESS)
			FatalError("Vulkan : failed to allocate command buffer, %s", RCore::verbaliseResultVk(res));
		#ifdef DEBUG
			Message("Vulkan : created new command buffer");
		#endif

		m_fence.init();
		state = CommandBufferState::Idle;
	}

	void CommandBuffer::BeginRecord(VkCommandBufferUsageFlags usage)
	{
		MLX_PROFILE_FUNCTION();
		if(!IsInit())
			FatalError("Vulkan : begenning record on un uninit command buffer");
		if(m_state == CommandBufferState::Recording)
			return;

		VkCommandBufferBeginInfo begin_info{};
		begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		begin_info.flags = usage;
		if(vkBeginCommandBuffer(m_cmd_buffer, &begin_info) != VK_SUCCESS)
			FatalError("Vulkan : failed to begin recording command buffer");

		m_state = CommandBufferState::Recording;
	}

	void CommandBuffer::BindVertexBuffer(Buffer& buffer) noexcept
	{
		MLX_PROFILE_FUNCTION();
		if(!IsRecording())
		{
			Warning("Vulkan : trying to bind a vertex buffer to a non recording command buffer");
			return;
		}
		VkDeviceSize offset[] = { buffer.GetOffset() };
		vkCmdBindVertexBuffers(m_cmd_buffer, 0, 1, &buffer.Get(), offset);

		buffer.RecordedInCommandBuffer();
		VectorPushBackIfNotFound(m_cmd_resources, &buffer);
	}

	void CommandBuffer::NindIndexBuffer(Buffer& buffer) noexcept
	{
		MLX_PROFILE_FUNCTION();
		if(!IsRecording())
		{
			Warning("Vulkan : trying to bind a index buffer to a non recording command buffer");
			return;
		}
		vkCmdBindIndexBuffer(m_cmd_buffer, buffer.Get(), buffer.GetOffset(), VK_INDEX_TYPE_UINT16);

		buffer.RecordedInCommandBuffer();
		VectorPushBackIfNotFound(m_cmd_resources, &buffer);
	}

	void CommandBuffer::CopyBuffer(Buffer& dst, Buffer& src) noexcept
	{
		MLX_PROFILE_FUNCTION();
		if(!IsRecording())
		{
			Warning("Vulkan : trying to do a buffer to buffer copy in a non recording command buffer");
			return;
		}

		PreTransferBarrier();

		VkBufferCopy copy_region{};
		copy_region.size = src.GetSize();
		vkCmdCopyBuffer(m_cmd_buffer, src.Get(), dst.Get(), 1, &copy_region);

		PostTransferBarrier();

		dst.RecordedInCommandBuffer();
		src.RecordedInCommandBuffer();
		VectorPushBackIfNotFound(m_cmd_resources, &dst);
		VectorPushBackIfNotFound(m_cmd_resources, &src);
	}

	void CommandBuffer::CopyBufferToImage(Buffer& buffer, Image& image) noexcept
	{
		MLX_PROFILE_FUNCTION();
		if(!IsRecording())
		{
			Warning("Vulkan : trying to do a buffer to image copy in a non recording command buffer");
			return;
		}

		PreTransferBarrier();

		VkBufferImageCopy region{};
		region.bufferOffset = 0;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;
		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;
		region.imageOffset = { 0, 0, 0 };
		region.imageExtent = { image.GetWidth(), image.GetHeight(), 1 };

		vkCmdCopyBufferToImage(m_cmd_buffer, buffer.Get(), image.Get(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

		PostTransferBarrier();

		image.RecordedInCommandBuffer();
		buffer.RecordedInCommandBuffer();
		VectorPushBackIfNotFound(m_cmd_resources, &image);
		VectorPushBackIfNotFound(m_cmd_resources, &buffer);
	}

	void CommandBuffer::CopyImagetoBuffer(Image& image, Buffer& buffer) noexcept
	{
		MLX_PROFILE_FUNCTION();
		if(!IsRecording())
		{
			Warning("Vulkan : trying to do an image to buffer copy in a non recording command buffer");
			return;
		}

		PreTransferBarrier();

		VkBufferImageCopy region{};
		region.bufferOffset = 0;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;
		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;
		region.imageOffset = { 0, 0, 0 };
		region.imageExtent = { image.GetWidth(), image.GetHeight(), 1 };

		vkCmdCopyImageToBuffer(m_cmd_buffer, image.Get(), VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, buffer.Get(), 1, &region);

		PostTransferBarrier();

		image.RecordedInCommandBuffer();
		buffer.RecordedInCommandBuffer();
		VectorPushBackIfNotFound(m_cmd_resources, &buffer);
		VectorPushBackIfNotFound(m_cmd_resources, &image);
	}

	void CommandBuffer::TransitionImageLayout(Image& image, VkImageLayout new_layout) noexcept
	{
		MLX_PROFILE_FUNCTION();
		if(!IsRecording())
		{
			Warning("Vulkan : trying to do an image layout transition in a non recording command buffer");
			return;
		}

		VkImageMemoryBarrier barrier{};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout = image.GetLayout();
		barrier.newLayout = new_layout;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image = image.Get();
		barrier.subresourceRange.aspectMask = IsDepthFormat(image.GetFormat()) ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = 1;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;
		barrier.srcAccessMask = LayoutToAccessMask(image.GetLayout(), false);
		barrier.dstAccessMask = LayoutToAccessMask(new_layout, true);
		if(IsStencilFormat(image.GetFormat()))
			barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;

		VkPipelineStageFlags source_stage = 0;
		if(barrier.oldLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR)
			source_stage = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
		else if(barrier.srcAccessMask != 0)
			source_stage = AccessFlagsToPipelineStage(barrier.srcAccessMask, VK_PIPELINE_STAGE_VERTEX_SHADER_BIT | VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT);
		else
			source_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;

		VkPipelineStageFlags destination_stage = 0;
		if(barrier.newLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR)
			destination_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		else if(barrier.dstAccessMask != 0)
			destination_stage = AccessFlagsToPipelineStage(barrier.dstAccessMask, VK_PIPELINE_STAGE_VERTEX_SHADER_BIT | VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT);
		else
			destination_stage = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;

		vkCmdPipelineBarrier(m_cmd_buffer, source_stage, destination_stage, 0, 0, nullptr, 0, nullptr, 1, &barrier);

		image.RecordedInCommandBuffer();
		VectorPushBackIfNotFound(m_cmd_resources, &image);
	}

	void CommandBuffer::EndRecord()
	{
		MLX_PROFILE_FUNCTION();
		if(!IsInit())
			FatalError("Vulkan : ending record on un uninit command buffer");
		if(m_state != CommandBufferState::Recording)
			return;
		if(vkEndCommandBuffer(m_cmd_buffer) != VK_SUCCESS)
			FatalError("Vulkan : failed to end recording command buffer");

		m_state = CommandBufferState::Idle;
	}

	void CommandBuffer::SubmitIdle(bool should_wait_for_execution) noexcept
	{
		MLX_PROFILE_FUNCTION();
		if(m_type != CommandBufferType::SingleTime)
		{
			Error("Vulkan : try to perform an idle submit on a command buffer that is not single-time, this is not allowed");
			return;
		}

		m_fence.Reset();

		VkSubmitInfo submit_info{};
		submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submit_info.commandBufferCount = 1;
		submit_info.pCommandBuffers = &m_cmd_buffer;

		VkResult res = vkQueueSubmit(RenderCore::Get().GetQueue().GetGraphic(), 1, &submit_info, m_fence.Get());
		if(res != VK_SUCCESS)
			FatalError("Vulkan error : failed to submit a single time command buffer, %", VerbaliseVkResult(res));
		m_state = CommandBufferState::Submitted;

		if(should_wait_for_execution)
			WaitForExecution();
	}

	void CommandBuffer::Submit(NonOwningPtr<Semaphore> signal, NonOwningPtr<Semaphore> wait) noexcept
	{
		MLX_PROFILE_FUNCTION();
		std::array<VkSemaphore, 1> signal_semaphores;
		std::array<VkSemaphore, 1> wait_semaphores;

		signal_semaphores[0] = (signal ? signal->Get() : VK_NULL_HANDLE);

		wait_semaphores[0] = (wait ? wait->Get() : VK_NULL_HANDLE);
		VkPipelineStageFlags wait_stages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

		m_fence.Reset();

		VkSubmitInfo submit_info{};
		submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submit_info.waitSemaphoreCount = (!wait ? 0 : wait_semaphores.size());
		submit_info.pWaitSemaphores = wait_semaphores.data();
		submit_info.pWaitDstStageMask = wait_stages;
		submit_info.commandBufferCount = 1;
		submit_info.pCommandBuffers = &m_cmd_buffer;
		submit_info.signalSemaphoreCount = (!signal ? 0 : signal_semaphores.size());
		submit_info.pSignalSemaphores = signal_semaphores.data();

		VkResult res = vkQueueSubmit(RenderCore::Get().GetQueue().GetGraphic(), 1, &submit_info, m_fence.get());
		if(res != VK_SUCCESS)
			FatalError("Vulkan error : failed to submit draw command buffer, %", VerbaliseVkResult(res));
		m_state = CommandBufferState::Submitted;
	}

	void CommandBuffer::UpdateSubmitState() noexcept
	{
		MLX_PROFILE_FUNCTION();
		if(!m_fence.IsReady())
			return;

		for(NonOwningPtr<CommandResource> res : m_cmd_resources)
		{
			if(res)
				res->RemovedFromCommandBuffer();
		}
		m_cmd_resources.clear();
		m_state = CommandBufferState::Ready;
	}

	void CommandBuffer::PreTransferBarrier() noexcept
	{
		MLX_PROFILE_FUNCTION();
		VkMemoryBarrier memory_barrier{};
		memory_barrier.sType = VK_STRUCTURE_TYPE_MEMORY_BARRIER;
		memory_barrier.pNext = nullptr;
		memory_barrier.srcAccessMask = 0U;
		memory_barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

		vkCmdPipelineBarrier(m_cmd_buffer, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 1, &memory_barrier, 0, nullptr, 0, nullptr);
	}

	void CommandBuffer::PostTransferBarrier() noexcept
	{
		MLX_PROFILE_FUNCTION();
		VkMemoryBarrier memory_barrier{};
		memory_barrier.sType = VK_STRUCTURE_TYPE_MEMORY_BARRIER;
		memory_barrier.pNext = nullptr;
		memory_barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		memory_barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_UNIFORM_READ_BIT;

		vkCmdPipelineBarrier(m_cmd_buffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT | VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_VERTEX_SHADER_BIT, 0, 1, &memory_barrier, 0, nullptr, 0, nullptr);
	}

	void CommandBuffer::Destroy() noexcept
	{
		MLX_PROFILE_FUNCTION();
		m_fence.Destroy();
		m_cmd_buffer = VK_NULL_HANDLE;
		m_state = CommandBufferState::Uninit;
		#ifdef DEBUG
			Message("Vulkan : destroyed command buffer");
		#endif
	}
}
