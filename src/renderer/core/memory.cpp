/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbz_8 <kbz_8.dev@akel-engine.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 22:02:37 by kbz_8             #+#    #+#             */
/*   Updated: 2023/11/10 23:29:56 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <core/profile.h>
#include <cstdio>

#define VMA_STATIC_VULKAN_FUNCTIONS 0
#define VMA_DYNAMIC_VULKAN_FUNCTIONS 0
#define VMA_VULKAN_VERSION 1002000
#define VMA_IMPLEMENTATION

#ifdef MLX_COMPILER_CLANG
	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Wnullability-completeness"
		#include <renderer/core/memory.h>
	#pragma clang diagnostic pop
#else
	#include <renderer/core/memory.h>
#endif

#include <renderer/core/render_core.h>

namespace mlx
{
	void GPUallocator::init() noexcept
	{
		VmaVulkanFunctions vma_vulkan_func{};
		vma_vulkan_func.vkAllocateMemory                        = vkAllocateMemory;
		vma_vulkan_func.vkBindBufferMemory                      = vkBindBufferMemory;
		vma_vulkan_func.vkBindImageMemory                       = vkBindImageMemory;
		vma_vulkan_func.vkCreateBuffer                          = vkCreateBuffer;
		vma_vulkan_func.vkCreateImage                           = vkCreateImage;
		vma_vulkan_func.vkDestroyBuffer                         = vkDestroyBuffer;
		vma_vulkan_func.vkDestroyImage                          = vkDestroyImage;
		vma_vulkan_func.vkFlushMappedMemoryRanges               = vkFlushMappedMemoryRanges;
		vma_vulkan_func.vkFreeMemory                            = vkFreeMemory;
		vma_vulkan_func.vkGetBufferMemoryRequirements           = vkGetBufferMemoryRequirements;
		vma_vulkan_func.vkGetImageMemoryRequirements            = vkGetImageMemoryRequirements;
		vma_vulkan_func.vkGetPhysicalDeviceMemoryProperties     = vkGetPhysicalDeviceMemoryProperties;
		vma_vulkan_func.vkGetPhysicalDeviceProperties           = vkGetPhysicalDeviceProperties;
		vma_vulkan_func.vkInvalidateMappedMemoryRanges          = vkInvalidateMappedMemoryRanges;
		vma_vulkan_func.vkMapMemory                             = vkMapMemory;
		vma_vulkan_func.vkUnmapMemory                           = vkUnmapMemory;
		vma_vulkan_func.vkCmdCopyBuffer                         = vkCmdCopyBuffer;
		vma_vulkan_func.vkGetBufferMemoryRequirements2KHR       = vkGetBufferMemoryRequirements2;
		vma_vulkan_func.vkGetImageMemoryRequirements2KHR        = vkGetImageMemoryRequirements2;
		vma_vulkan_func.vkBindBufferMemory2KHR                  = vkBindBufferMemory2;
		vma_vulkan_func.vkBindImageMemory2KHR                   = vkBindImageMemory2;
		vma_vulkan_func.vkGetPhysicalDeviceMemoryProperties2KHR = vkGetPhysicalDeviceMemoryProperties2;

		VmaAllocatorCreateInfo allocatorCreateInfo{};
		allocatorCreateInfo.vulkanApiVersion = VK_API_VERSION_1_2;
		allocatorCreateInfo.physicalDevice = Render_Core::get().getDevice().getPhysicalDevice();
		allocatorCreateInfo.device = Render_Core::get().getDevice().get();
		allocatorCreateInfo.instance = Render_Core::get().getInstance().get();
		allocatorCreateInfo.pVulkanFunctions = &vma_vulkan_func;

		if(vmaCreateAllocator(&allocatorCreateInfo, &_allocator) != VK_SUCCESS)
			core::error::report(e_kind::fatal_error, "Vulkan : failed to create allocator");
		#ifdef DEBUG
			core::error::report(e_kind::message, "Vulkan : created new allocator");
		#endif
	}

	VmaAllocation GPUallocator::createBuffer(const VkBufferCreateInfo* binfo, const VmaAllocationCreateInfo* vinfo, VkBuffer& buffer, VmaAllocationInfo& allocinfo) noexcept
	{
		VmaAllocation allocation;
		if(vmaCreateBuffer(_allocator, binfo, vinfo, &buffer, &allocation, nullptr) != VK_SUCCESS)
			core::error::report(e_kind::fatal_error, "Vulkan : failed to allocate a buffer");
		#ifdef DEBUG
			core::error::report(e_kind::message, "Graphics Allocator : created new buffer");
		#endif
		vmaGetAllocationInfo(_allocator, allocation, &allocinfo);
		return allocation;
	}

	void GPUallocator::destroyBuffer(VmaAllocation allocation, VkBuffer buffer) noexcept
	{
		vmaDestroyBuffer(_allocator, buffer, allocation);
	}

	VmaAllocation GPUallocator::createImage(const VkImageCreateInfo* iminfo, const VmaAllocationCreateInfo* vinfo, VkImage& image, VmaAllocationInfo& allocinfo) noexcept
	{
		VmaAllocation allocation;
		if(vmaCreateImage(_allocator, iminfo, vinfo, &image, &allocation, nullptr) != VK_SUCCESS)
			core::error::report(e_kind::fatal_error, "Vulkan : failed to allocate an image");
		#ifdef DEBUG
			core::error::report(e_kind::message, "Graphics Allocator : created new image");
		#endif
		vmaGetAllocationInfo(_allocator, allocation, &allocinfo);
		return allocation;
	}

	void GPUallocator::destroyImage(VmaAllocation allocation, VkImage image) noexcept
	{
		vmaDestroyImage(_allocator, image, allocation);
	}

	void GPUallocator::mapMemory(VmaAllocation allocation, void** data) noexcept
	{
		if(vmaMapMemory(_allocator, allocation, data) != VK_SUCCESS)
			core::error::report(e_kind::fatal_error, "Graphics allocator : unable to map GPU memory to CPU memory");
	}

	void GPUallocator::unmapMemory(VmaAllocation allocation) noexcept
	{
		vmaUnmapMemory(_allocator, allocation);
	}
	
	void GPUallocator::flush(VmaAllocation allocation, VkDeviceSize size, VkDeviceSize offset) noexcept
	{
		vmaFlushAllocation(_allocator, allocation, offset, size);
	}

	void GPUallocator::destroy() noexcept
	{
		vmaDestroyAllocator(_allocator);
	}
}
