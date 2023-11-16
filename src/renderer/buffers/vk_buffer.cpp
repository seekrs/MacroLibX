/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_buffer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 18:55:57 by maldavid          #+#    #+#             */
/*   Updated: 2023/11/16 13:54:25 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vk_buffer.h"
#include <renderer/command/vk_cmd_pool.h>
#include <renderer/command/vk_cmd_buffer.h>
#include <renderer/core/render_core.h>
#include <vma.h>
#include <cstring>
#include <iostream>

namespace mlx
{
	void Buffer::create(Buffer::kind type, VkDeviceSize size, VkBufferUsageFlags usage, const char* name, const void* data)
	{
		_usage = usage;
		if(type == Buffer::kind::constant)
		{
			if(data == nullptr)
			{
				core::error::report(e_kind::warning, "Vulkan : trying to create constant buffer without data (constant buffers cannot be modified after creation)");
				return;
			}
			_usage |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		}

		VmaAllocationCreateInfo alloc_info{};
		alloc_info.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;
		alloc_info.usage = VMA_MEMORY_USAGE_AUTO;

		createBuffer(_usage, alloc_info, size, name);

		if(data != nullptr)
		{
			void* mapped = nullptr;
			mapMem(&mapped);
				std::memcpy(mapped, data, size);
			unmapMem();
			if(type == Buffer::kind::constant)
				pushToGPU();
		}
	}

	void Buffer::destroy() noexcept
	{
		if(_is_mapped)
			unmapMem();
		if(_buffer != VK_NULL_HANDLE)
			Render_Core::get().getAllocator().destroyBuffer(_allocation, _buffer);
		_buffer = VK_NULL_HANDLE;
	}

	void Buffer::createBuffer(VkBufferUsageFlags usage, VmaAllocationCreateInfo info, VkDeviceSize size, const char* name)
	{
		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = usage;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		#ifdef DEBUG
			_name = name;
			std::string alloc_name = _name;
			if(usage & VK_BUFFER_USAGE_INDEX_BUFFER_BIT)
				alloc_name.append("_index_buffer");
			else if(usage & VK_BUFFER_USAGE_VERTEX_BUFFER_BIT)
				alloc_name.append("_vertex_buffer");
			else if((usage & VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT) != 1)
				alloc_name.append("_buffer");
			_allocation = Render_Core::get().getAllocator().createBuffer(&bufferInfo, &info, _buffer, alloc_name.c_str());
		#else
			_allocation = Render_Core::get().getAllocator().createBuffer(&bufferInfo, &info, _buffer, nullptr);
		#endif
		_size = size;
	}

	void Buffer::pushToGPU() noexcept
	{
		VmaAllocationCreateInfo alloc_info{};
		alloc_info.usage = VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE;

		Buffer newBuffer;
		newBuffer._usage = (_usage & 0xFFFFFFFC) | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		#ifdef DEBUG
			std::string new_name = _name + "_GPU";
			newBuffer.createBuffer(newBuffer._usage, alloc_info, _size, new_name.c_str());
		#else
			newBuffer.createBuffer(newBuffer._usage, alloc_info, _size, nullptr);
		#endif

		CmdPool cmdpool;
		cmdpool.init();
		CmdBuffer cmdBuffer;
		cmdBuffer.init(&cmdpool);

		cmdBuffer.beginRecord(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

		VkBufferCopy copyRegion{};
		copyRegion.size = _size;
		vkCmdCopyBuffer(cmdBuffer.get(), _buffer, newBuffer._buffer, 1, &copyRegion);

		cmdBuffer.endRecord();
		cmdBuffer.submitIdle();

		cmdBuffer.destroy();
		cmdpool.destroy();

		this->swap(newBuffer);

		newBuffer.destroy();
	}

	void Buffer::swap(Buffer& buffer) noexcept
	{
		VkBuffer temp_b = _buffer;
		_buffer = buffer._buffer;
		buffer._buffer = temp_b;

		VmaAllocation temp_a = buffer._allocation;
		buffer._allocation = _allocation;
		_allocation = temp_a;

		VkDeviceSize temp_size = buffer._size;
		buffer._size = _size;
		_size = temp_size;

		VkDeviceSize temp_offset = buffer._offset;
		buffer._offset = _offset;
		_offset = temp_offset;

		VkBufferUsageFlags temp_u = _usage;
		_usage = buffer._usage;
		buffer._usage = temp_u;
	}

	void Buffer::flush(VkDeviceSize size, VkDeviceSize offset)
	{
		Render_Core::get().getAllocator().flush(_allocation, size, offset);
	}
}
