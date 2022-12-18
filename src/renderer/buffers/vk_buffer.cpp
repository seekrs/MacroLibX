/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_buffer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 18:55:57 by maldavid          #+#    #+#             */
/*   Updated: 2022/12/18 02:10:37 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vk_buffer.h"
#include <cstring>

namespace mlx
{
	void Buffer::create(Buffer::kind type, VkDeviceSize size, VkBufferUsageFlags usage, const void* data)
	{
		if(type == Buffer::kind::uniform)
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
	}

	void Buffer::destroy() noexcept
	{
		vkDestroyBuffer(Render_Core::get().getDevice().get(), _buffer, nullptr);
		vkFreeMemory(Render_Core::get().getDevice().get(), _memory, nullptr);
	}

	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
    {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(Render_Core::get().getDevice().getPhysicalDevice(), &memProperties);

        for(uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
        {
            if((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
                return i;
        }

        core::error::report(e_kind::fatal_error, "Vulkan : failed to find suitable memory type");
		return -1; // just to avoid warning
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
        allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

        if(vkAllocateMemory(device, &allocInfo, nullptr, &_memory) != VK_SUCCESS)
            core::error::report(e_kind::fatal_error, "Vulkan : failed to allocate buffer memory");
		if(vkBindBufferMemory(device, _buffer, _memory, _offset) != VK_SUCCESS)
			core::error::report(e_kind::fatal_error, "Vulkan : unable to bind device memory to a buffer object");
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
