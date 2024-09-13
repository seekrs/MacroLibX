/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbz_8 <kbz_8.dev@akel-engine.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 22:02:37 by kbz_8             #+#    #+#             */
/*   Updated: 2024/09/14 00:04:16 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <mlx_profile.h>
#include <core/errors.h>
#include <core/profiler.h>

#define VMA_STATIC_VULKAN_FUNCTIONS 0
#define VMA_DYNAMIC_VULKAN_FUNCTIONS 0
#define VMA_VULKAN_VERSION 1002000
#define VMA_ASSERT(expr) ((void)0)
#define VMA_IMPLEMENTATION

#ifdef MLX_COMPILER_CLANG
	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Weverything"
		#include <renderer/core/memory.h>
	#pragma clang diagnostic pop
#elif defined(MLX_COMPILER_GCC)
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wimplicit-fallthrough"
	#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
	#pragma GCC diagnostic ignored "-Wunused-parameter"
	#pragma GCC diagnostic ignored "-Wunused-variable"
	#pragma GCC diagnostic ignored "-Wunused-function"
	#pragma GCC diagnostic ignored "-Wparentheses"
	#pragma GCC diagnostic ignored "-Wparentheses"
		#include <renderer/core/memory.h>
	#pragma GCC diagnostic pop
#else
	#include <renderer/core/memory.h>
#endif

#include <renderer/core/render_core.h>
#include <fstream>

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

		VkResult res = vmaCreateAllocator(&allocatorCreateInfo, &_allocator);
		if(res != VK_SUCCESS)
			core::error::report(e_kind::fatal_error, "Graphics allocator : failed to create graphics memory allocator, %s", RCore::verbaliseResultVk(res));
		#ifdef DEBUG
			core::error::report(e_kind::message, "Graphics allocator : created new allocator");
		#endif
	}

	VmaAllocation GPUallocator::createBuffer(const VkBufferCreateInfo* binfo, const VmaAllocationCreateInfo* vinfo, VkBuffer& buffer, const char* name) noexcept
	{
		MLX_PROFILE_FUNCTION();
		VmaAllocation allocation;
		VkResult res = vmaCreateBuffer(_allocator, binfo, vinfo, &buffer, &allocation, nullptr);
		if(res != VK_SUCCESS)
			core::error::report(e_kind::fatal_error, "Graphics allocator : failed to allocate a buffer, %s", RCore::verbaliseResultVk(res));
		if(name != nullptr)
		{
			Render_Core::get().getLayers().setDebugUtilsObjectNameEXT(VK_OBJECT_TYPE_BUFFER, (std::uint64_t)buffer, name);
			vmaSetAllocationName(_allocator, allocation, name);
		}
		#ifdef DEBUG
			core::error::report(e_kind::message, "Graphics Allocator : created new buffer '%s'", name);
		#endif
		_active_buffers_allocations++;
		return allocation;
	}

	void GPUallocator::destroyBuffer(VmaAllocation allocation, VkBuffer buffer) noexcept
	{
		MLX_PROFILE_FUNCTION();
		vkDeviceWaitIdle(Render_Core::get().getDevice().get());
		vmaDestroyBuffer(_allocator, buffer, allocation);
		#ifdef DEBUG
			core::error::report(e_kind::message, "Graphics Allocator : destroyed buffer");
		#endif
		_active_buffers_allocations--;
	}

	VmaAllocation GPUallocator::createImage(const VkImageCreateInfo* iminfo, const VmaAllocationCreateInfo* vinfo, VkImage& image, const char* name) noexcept
	{
		MLX_PROFILE_FUNCTION();
		VmaAllocation allocation;
		VkResult res = vmaCreateImage(_allocator, iminfo, vinfo, &image, &allocation, nullptr);
		if(res != VK_SUCCESS)
			core::error::report(e_kind::fatal_error, "Graphics allocator : failed to allocate an image, %s", RCore::verbaliseResultVk(res));
		if(name != nullptr)
		{
			Render_Core::get().getLayers().setDebugUtilsObjectNameEXT(VK_OBJECT_TYPE_IMAGE, (std::uint64_t)image, name);
			vmaSetAllocationName(_allocator, allocation, name);
		}
		#ifdef DEBUG
			core::error::report(e_kind::message, "Graphics Allocator : created new image '%s'", name);
		#endif
		_active_images_allocations++;
		return allocation;
	}

	void GPUallocator::destroyImage(VmaAllocation allocation, VkImage image) noexcept
	{
		MLX_PROFILE_FUNCTION();
		vkDeviceWaitIdle(Render_Core::get().getDevice().get());
		vmaDestroyImage(_allocator, image, allocation);
		#ifdef DEBUG
			core::error::report(e_kind::message, "Graphics Allocator : destroyed image");
		#endif
		_active_images_allocations--;
	}

	void GPUallocator::mapMemory(VmaAllocation allocation, void** data) noexcept
	{
		MLX_PROFILE_FUNCTION();
		VkResult res = vmaMapMemory(_allocator, allocation, data);
		if(res != VK_SUCCESS)
			core::error::report(e_kind::fatal_error, "Graphics allocator : unable to map GPU memory to CPU memory, %s", RCore::verbaliseResultVk(res));
	}

	void GPUallocator::unmapMemory(VmaAllocation allocation) noexcept
	{
		MLX_PROFILE_FUNCTION();
		vmaUnmapMemory(_allocator, allocation);
	}

	void GPUallocator::dumpMemoryToJson()
	{
		static std::uint32_t id = 0;
		std::string name("memory_dump");
		name.append(std::to_string(id) + ".json");
		std::ofstream file(name);
		if(!file.is_open())
		{
			core::error::report(e_kind::error, "Graphics allocator : unable to dump memory to a json file");
			return;
		}
		char* str = nullptr;
		vmaBuildStatsString(_allocator, &str, true);
			file << str;
		vmaFreeStatsString(_allocator, str);
		file.close();
		id++;
	}

	void GPUallocator::flush(VmaAllocation allocation, VkDeviceSize size, VkDeviceSize offset) noexcept
	{
		MLX_PROFILE_FUNCTION();
		vmaFlushAllocation(_allocator, allocation, offset, size);
	}

	void GPUallocator::destroy() noexcept
	{
		if(_active_images_allocations != 0)
			core::error::report(e_kind::error, "Graphics allocator : some user-dependant allocations were not freed before destroying the display (%d active allocations). You may have not destroyed all the MLX resources you've created", _active_images_allocations);
		else if(_active_buffers_allocations != 0)
			core::error::report(e_kind::error, "Graphics allocator : some MLX-dependant allocations were not freed before destroying the display (%d active allocations). This is an error in the MLX, please report this should not happen", _active_buffers_allocations);
		if(_active_images_allocations < 0 || _active_buffers_allocations < 0)
			core::error::report(e_kind::warning, "Graphics allocator : the impossible happened, the MLX has freed more allocations than it has made (wtf)");
		vmaDestroyAllocator(_allocator);
		_active_buffers_allocations = 0;
		_active_images_allocations = 0;
		#ifdef DEBUG
			core::error::report(e_kind::message, "Vulkan : destroyed a graphics allocator");
		#endif
	}
}
