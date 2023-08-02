/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_buffer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 18:55:57 by maldavid          #+#    #+#             */
/*   Updated: 2023/04/23 12:37:32 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vk_buffer.h"
#include <renderer/command/vk_cmd_pool.h>
#include <renderer/core/render_core.h>
#include <cstring>

namespace mlx
{
	void Buffer::create(Buffer::kind type, VkDeviceSize size, VkBufferUsageFlags usage, const void* data)
	{
		if(type == Buffer::kind::constant)
		{
			if(data == nullptr)
			{
				core::error::report(e_kind::warning, "Vulkan : trying to create constant buffer without data (constant buffers cannot be modified after creation)");
				return;
			}
			_usage = usage | VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
			_flags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
		}
		else if(type == Buffer::kind::uniform)
		{
			_usage = usage;
			_flags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
		}
		else
		{
			_usage = usage;
			_flags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
		}

		_size = size;

		createBuffer(_usage, _flags);

		if(type == Buffer::kind::constant || data != nullptr)
		{
			void* mapped = nullptr;
			mapMem(&mapped);
				std::memcpy(mapped, data, _size);
			unmapMem();

			if(type == Buffer::kind::constant)
				pushToGPU();
		}
	}

	void Buffer::destroy() noexcept
	{
		vkDestroyBuffer(Render_Core::get().getDevice().get(), _buffer, nullptr);
		vkFreeMemory(Render_Core::get().getDevice().get(), _memory, nullptr);
	}

	void Buffer::createBuffer(VkBufferUsageFlags usage, VkMemoryPropertyFlags properties)
	{
		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = _size;
		bufferInfo.usage = usage;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		auto device = Render_Core::get().getDevice().get();

		if(vkCreateBuffer(device, &bufferInfo, nullptr, &_buffer) != VK_SUCCESS)
			core::error::report(e_kind::fatal_error, "Vulkan : failed to create buffer");

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(device, _buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = *RCore::findMemoryType(memRequirements.memoryTypeBits, properties);

        if(vkAllocateMemory(device, &allocInfo, nullptr, &_memory) != VK_SUCCESS)
            core::error::report(e_kind::fatal_error, "Vulkan : failed to allocate buffer memory");
		if(vkBindBufferMemory(device, _buffer, _memory, _offset) != VK_SUCCESS)
			core::error::report(e_kind::fatal_error, "Vulkan : unable to bind device memory to a buffer object");
	}

	void Buffer::pushToGPU() noexcept
	{
		Buffer newBuffer;
		newBuffer._size = _size;
		newBuffer._usage = (this->_usage & 0xFFFFFFFC) | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		newBuffer._flags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
		newBuffer.createBuffer(newBuffer._usage, newBuffer._flags);

		CmdPool cmdpool;
		cmdpool.init();
		auto device = Render_Core::get().getDevice().get();

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = cmdpool.get();
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		VkBufferCopy copyRegion{};
		copyRegion.size = _size;
		vkCmdCopyBuffer(commandBuffer, _buffer, newBuffer._buffer, 1, &copyRegion);

		vkEndCommandBuffer(commandBuffer);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		auto graphicsQueue = Render_Core::get().getQueue().getGraphic();

		vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(graphicsQueue);

		cmdpool.destroy();

		this->swap(newBuffer);

		newBuffer.destroy();
	}

	void Buffer::swap(Buffer& buffer) noexcept
	{
		VkBuffer temp_b = _buffer;
		_buffer = buffer._buffer;
		buffer._buffer = temp_b;

		VkDeviceSize temp_size = buffer._size;
		buffer._size = _size;
		_size = temp_size;

		VkDeviceSize temp_offset = buffer._offset;
		buffer._offset = _offset;
		_offset = temp_offset;

		VkDeviceMemory temp_memory = buffer._memory;
		buffer._memory = _memory;
		_memory = temp_memory;

		VkBufferUsageFlags temp_u = _usage;
		_usage = buffer._usage;
		buffer._usage = temp_u;

		VkMemoryPropertyFlags temp_f = _flags;
		_flags = buffer._flags;
		buffer._flags = temp_f;
	}

	void Buffer::flush(VkDeviceSize size, VkDeviceSize offset)
	{
		VkMappedMemoryRange mappedRange{};
		mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
		mappedRange.memory = _memory;
		mappedRange.offset = offset;
		mappedRange.size = size;
		vkFlushMappedMemoryRanges(Render_Core::get().getDevice().get(), 1, &mappedRange);
	}
}
