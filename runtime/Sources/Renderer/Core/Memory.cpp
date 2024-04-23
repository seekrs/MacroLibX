/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Memory.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbz_8 <kbz_8.dev@akel-engine.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 22:02:37 by kbz_8             #+#    #+#             */
/*   Updated: 2024/04/23 18:49:10 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <mlx_profile.h>

#include <cstdio>

#define VK_NO_PROTOTYPES
#define VMA_STATIC_VULKAN_FUNCTIONS 0
#define VMA_DYNAMIC_VULKAN_FUNCTIONS 0
#define VMA_ASSERT(expr) ((void)0)
#define VMA_IMPLEMENTATION

#ifdef MLX_COMPILER_CLANG
	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Weverything"
		#include <vma.h>
	#pragma clang diagnostic pop
#elif defined(MLX_COMPILER_GCC)
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wimplicit-fallthrough"
	#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
	#pragma GCC diagnostic ignored "-Wunused-parameter"
	#pragma GCC diagnostic ignored "-Wunused-variable"
	#pragma GCC diagnostic ignored "-Wparentheses"
		#include <vma.h>
	#pragma GCC diagnostic pop
#else
	#include <vma.h>
#endif

#include <PreCompiled.h>
#include <Renderer/Core/RenderCore.h>

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
#if VMA_DEDICATED_ALLOCATION || VMA_VULKAN_VERSION >= 1001000
		vma_vulkan_func.vkGetBufferMemoryRequirements2KHR = vkGetBufferMemoryRequirements2,
		vma_vulkan_func.vkGetImageMemoryRequirements2KHR  = vkGetImageMemoryRequirements2,
#endif
#if VMA_BIND_MEMORY2 || VMA_VULKAN_VERSION >= 1001000
		vma_vulkan_func.vkBindBufferMemory2KHR = vkBindBufferMemory2,
		vma_vulkan_func.vkBindImageMemory2KHR  = vkBindImageMemory2,
#endif
#if VMA_MEMORY_BUDGET || VMA_VULKAN_VERSION >= 1001000
		vma_vulkan_func.vkGetPhysicalDeviceMemoryProperties2KHR = vkGetPhysicalDeviceMemoryProperties2,
#endif
#if VMA_VULKAN_VERSION >= 1003000
		vma_vulkan_func.vkGetDeviceBufferMemoryRequirements = vkGetDeviceBufferMemoryRequirements,
		vma_vulkan_func.vkGetDeviceImageMemoryRequirements = vkGetDeviceImageMemoryRequirements,
#endif

		VmaAllocatorCreateInfo allocator_create_info{};
		allocator_create_info.vulkanApiVersion = RenderCore::Get().GetInstance().GetInstanceVersion();
		allocator_create_info.physicalDevice = RenderCore::Get().GetDevice().GetPhysicalDevice();
		allocator_create_info.device = RenderCore::Get().GetDevice().Get();
		allocator_create_info.instance = RenderCore::Get().GetInstance().Get();
		allocator_create_info.pVulkanFunctions = &vma_vulkan_func;

		VkResult res = vmaCreateAllocator(&allocator_create_info, &m_allocator);
		if(res != VK_SUCCESS)
			FatalError("Graphics allocator : failed to create graphics memory allocator, %", VerbaliseVkResult(res));
		DebugLog("Graphics allocator : created new allocator");
	}

	VmaAllocation GPUallocator::CreateBuffer(const VkBufferCreateInfo* binfo, const VmaAllocationCreateInfo* vinfo, VkBuffer& buffer, const char* name) noexcept
	{
		MLX_PROFILE_FUNCTION();
		VmaAllocation allocation;
		VkResult res = vmaCreateBuffer(m_allocator, binfo, vinfo, &buffer, &allocation, nullptr);
		if(res != VK_SUCCESS)
			FatalError("Graphics allocator : failed to allocate a buffer, %s", RCore::verbaliseResultVk(res));
		if(name != nullptr)
		{
			RenderCore::Get().GetLayers().SetDebugUtilsObjectNameEXT(VK_OBJECT_TYPE_BUFFER, (std::uint64_t)buffer, name);
			vmaSetAllocationName(m_allocator, allocation, name);
		}
		DebugLog("Graphics Allocator : created new buffer '%s'", name);
		m_active_buffers_allocations++;
		return allocation;
	}

	void GPUallocator::DestroyBuffer(VmaAllocation allocation, VkBuffer buffer) noexcept
	{
		MLX_PROFILE_FUNCTION();
		vkDeviceWaitIdle(RenderCore::Get().GetDevice().Get());
		vmaDestroyBuffer(m_allocator, buffer, allocation);
		DebugLog("Graphics Allocator : destroyed buffer");
		m_active_buffers_allocations--;
	}

	VmaAllocation GPUallocator::CreateImage(const VkImageCreateInfo* iminfo, const VmaAllocationCreateInfo* vinfo, VkImage& image, const char* name) noexcept
	{
		MLX_PROFILE_FUNCTION();
		VmaAllocation allocation;
		VkResult res = vmaCreateImage(m_allocator, iminfo, vinfo, &image, &allocation, nullptr);
		if(res != VK_SUCCESS)
			FatalError("Graphics allocator : failed to allocate an image, %", VerbaliseVkResult(res));
		if(name != nullptr)
		{
			RenderCore::Get().GetLayers().SetDebugUtilsObjectNameEXT(VK_OBJECT_TYPE_IMAGE, (std::uint64_t)image, name);
			vmaSetAllocationName(m_allocator, allocation, name);
		}
		DebugLog("Graphics Allocator : created new image '%s'", name);
		m_active_images_allocations++;
		return allocation;
	}

	void GPUallocator::DestroyImage(VmaAllocation allocation, VkImage image) noexcept
	{
		MLX_PROFILE_FUNCTION();
		vkDeviceWaitIdle(RenderCore::Get().GetDevice().Get());
		vmaDestroyImage(m_allocator, image, allocation);
		DebugLog("Graphics Allocator : destroyed image");
		m_active_images_allocations--;
	}

	void GPUallocator::MapMemory(VmaAllocation allocation, void** data) noexcept
	{
		MLX_PROFILE_FUNCTION();
		VkResult res = vmaMapMemory(m_allocator, allocation, data);
		if(res != VK_SUCCESS)
			FatalError("Graphics allocator : unable to map GPU memory to CPU memory, %", VerbaliseVkResult(res));
	}

	void GPUallocator::unmapMemory(VmaAllocation allocation) noexcept
	{
		MLX_PROFILE_FUNCTION();
		vmaUnmapMemory(m_allocator, allocation);
	}

	void GPUallocator::dumpMemoryToJson()
	{
		static std::uint32_t id = 0;
		std::string name("memory_dump");
		name.append(std::to_string(id) + ".json");
		std::ofstream file(name);
		if(!file.is_open())
		{
			Error("Graphics allocator : unable to dump memory to a json file");
			return;
		}
		char* str = nullptr;
		vmaBuildStatsString(m_allocator, &str, true);
			file << str;
		vmaFreeStatsString(m_allocator, str);
		file.close();
		id++;
	}

	void GPUallocator::Flush(VmaAllocation allocation, VkDeviceSize size, VkDeviceSize offset) noexcept
	{
		MLX_PROFILE_FUNCTION();
		vmaFlushAllocation(m_allocator, allocation, offset, size);
	}

	void GPUallocator::Destroy() noexcept
	{
		if(m_active_images_allocations != 0)
			Error("Graphics allocator : some user-dependant allocations were not freed before destroying the display (% active allocations). You may have not destroyed all the MLX resources you've created", m_active_images_allocations);
		else if(m_active_buffers_allocations != 0)
			Error("Graphics allocator : some MLX-dependant allocations were not freed before destroying the display (% active allocations). This is an error in the MLX, please report this should not happen", m_active_buffers_allocations);
		if(m_active_images_allocations < 0 || m_active_buffers_allocations < 0)
			Warning("Graphics allocator : the impossible happened, the MLX has freed more allocations than it has made (wtf)");
		vmaDestroyAllocator(m_allocator);
		m_active_buffers_allocations = 0;
		m_active_images_allocations = 0;
		DebugLog("Vulkan : destroyed a graphics allocator");
	}
}
