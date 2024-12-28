#include <PreCompiled.h>
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
	#pragma GCC diagnostic ignored "-Wunused-function"
	#pragma GCC diagnostic ignored "-Wparentheses"
		#include <vma.h>
	#pragma GCC diagnostic pop
#else
	#include <vma.h>
#endif

#include <Renderer/RenderCore.h>

namespace mlx
{
	void GPUAllocator::Init(const VkAllocationCallbacks* callbacks) noexcept
	{
		MLX_PROFILE_FUNCTION();
		VmaVulkanFunctions vma_vulkan_func{};
		vma_vulkan_func.vkAllocateMemory                    = RenderCore::Get().vkAllocateMemory;
		vma_vulkan_func.vkBindBufferMemory                  = RenderCore::Get().vkBindBufferMemory;
		vma_vulkan_func.vkBindImageMemory                   = RenderCore::Get().vkBindImageMemory;
		vma_vulkan_func.vkCreateBuffer                      = RenderCore::Get().vkCreateBuffer;
		vma_vulkan_func.vkCreateImage                       = RenderCore::Get().vkCreateImage;
		vma_vulkan_func.vkDestroyBuffer                     = RenderCore::Get().vkDestroyBuffer;
		vma_vulkan_func.vkDestroyImage                      = RenderCore::Get().vkDestroyImage;
		vma_vulkan_func.vkFlushMappedMemoryRanges           = RenderCore::Get().vkFlushMappedMemoryRanges;
		vma_vulkan_func.vkFreeMemory                        = RenderCore::Get().vkFreeMemory;
		vma_vulkan_func.vkGetBufferMemoryRequirements       = RenderCore::Get().vkGetBufferMemoryRequirements;
		vma_vulkan_func.vkGetImageMemoryRequirements        = RenderCore::Get().vkGetImageMemoryRequirements;
		vma_vulkan_func.vkGetPhysicalDeviceMemoryProperties = RenderCore::Get().vkGetPhysicalDeviceMemoryProperties;
		vma_vulkan_func.vkGetPhysicalDeviceProperties       = RenderCore::Get().vkGetPhysicalDeviceProperties;
		vma_vulkan_func.vkInvalidateMappedMemoryRanges      = RenderCore::Get().vkInvalidateMappedMemoryRanges;
		vma_vulkan_func.vkMapMemory                         = RenderCore::Get().vkMapMemory;
		vma_vulkan_func.vkUnmapMemory                       = RenderCore::Get().vkUnmapMemory;
		vma_vulkan_func.vkCmdCopyBuffer                     = RenderCore::Get().vkCmdCopyBuffer;

		VmaAllocatorCreateInfo allocator_create_info{};
		allocator_create_info.vulkanApiVersion = VK_API_VERSION_1_0;
		allocator_create_info.physicalDevice = RenderCore::Get().GetPhysicalDevice();
		allocator_create_info.device = RenderCore::Get().GetDevice();
		allocator_create_info.instance = RenderCore::Get().GetInstance();
		allocator_create_info.pAllocationCallbacks = callbacks;
		allocator_create_info.pVulkanFunctions = &vma_vulkan_func;

		kvfCheckVk(vmaCreateAllocator(&allocator_create_info, &m_allocator));
		DebugLog("Graphics Allocator: created new allocator");
	}

	VmaAllocation GPUAllocator::CreateBuffer(const VkBufferCreateInfo* binfo, const VmaAllocationCreateInfo* vinfo, VkBuffer& buffer, const char* name) noexcept
	{
		MLX_PROFILE_FUNCTION();
		VmaAllocation allocation;
		kvfCheckVk(vmaCreateBuffer(m_allocator, binfo, vinfo, &buffer, &allocation, nullptr));
		if(name != nullptr)
		{
			#ifdef MLX_HAS_DEBUG_UTILS_FUNCTIONS
				VkDebugUtilsObjectNameInfoEXT name_info{};
				name_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
				name_info.objectType = VK_OBJECT_TYPE_BUFFER;
				name_info.objectHandle = reinterpret_cast<std::uint64_t>(buffer);
				name_info.pObjectName = name;
				RenderCore::Get().vkSetDebugUtilsObjectNameEXT(RenderCore::Get().GetDevice(), &name_info);
			#endif
			vmaSetAllocationName(m_allocator, allocation, name);
		}
		DebugLog("Graphics Allocator: created new buffer '%'", name);
		m_active_buffers_allocations++;
		return allocation;
	}

	void GPUAllocator::DestroyBuffer(VmaAllocation allocation, VkBuffer buffer, const char* name) noexcept
	{
		MLX_PROFILE_FUNCTION();
		RenderCore::Get().WaitDeviceIdle();
		vmaDestroyBuffer(m_allocator, buffer, allocation);
		if(name != nullptr)
			DebugLog("Graphics Allocator: destroyed buffer '%'", name);
		else
			DebugLog("Graphics Allocator: destroyed buffer");
		m_active_buffers_allocations--;
	}

	VmaAllocation GPUAllocator::CreateImage(const VkImageCreateInfo* iminfo, const VmaAllocationCreateInfo* vinfo, VkImage& image, const char* name) noexcept
	{
		MLX_PROFILE_FUNCTION();
		VmaAllocation allocation;
		kvfCheckVk(vmaCreateImage(m_allocator, iminfo, vinfo, &image, &allocation, nullptr));
		if(name != nullptr)
		{
			#ifdef MLX_HAS_DEBUG_UTILS_FUNCTIONS
				VkDebugUtilsObjectNameInfoEXT name_info{};
				name_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
				name_info.objectType = VK_OBJECT_TYPE_IMAGE;
				name_info.objectHandle = reinterpret_cast<std::uint64_t>(image);
				name_info.pObjectName = name;
				RenderCore::Get().vkSetDebugUtilsObjectNameEXT(RenderCore::Get().GetDevice(), &name_info);
			#endif
			vmaSetAllocationName(m_allocator, allocation, name);
		}
		DebugLog("Graphics Allocator: created new image '%'", name);
		m_active_images_allocations++;
		return allocation;
	}

	void GPUAllocator::DestroyImage(VmaAllocation allocation, VkImage image, const char* name) noexcept
	{
		MLX_PROFILE_FUNCTION();
		RenderCore::Get().WaitDeviceIdle();
		vmaDestroyImage(m_allocator, image, allocation);
		if(name != nullptr)
			DebugLog("Graphics Allocator: destroyed image '%'", name);
		else
			DebugLog("Graphics Allocator: destroyed image");
		m_active_images_allocations--;
	}

	void GPUAllocator::MapMemory(VmaAllocation allocation, void** data) noexcept
	{
		MLX_PROFILE_FUNCTION();
		kvfCheckVk(vmaMapMemory(m_allocator, allocation, data));
	}

	void GPUAllocator::UnmapMemory(VmaAllocation allocation) noexcept
	{
		MLX_PROFILE_FUNCTION();
		vmaUnmapMemory(m_allocator, allocation);
	}

	void GPUAllocator::DumpMemoryToJson()
	{
		static std::uint32_t id = 0;
		std::string name("memory_dump");
		name.append(std::to_string(id) + ".json");
		std::ofstream file(name);
		if(!file.is_open())
		{
			Error("Graphics Allocator: unable to dump memory to a json file");
			return;
		}
		char* str = nullptr;
		vmaBuildStatsString(m_allocator, &str, true);
			file << str;
		vmaFreeStatsString(m_allocator, str);
		file.close();
		id++;
	}

	void GPUAllocator::Flush(VmaAllocation allocation, VkDeviceSize size, VkDeviceSize offset) noexcept
	{
		MLX_PROFILE_FUNCTION();
		vmaFlushAllocation(m_allocator, allocation, offset, size);
	}

	void GPUAllocator::Destroy() noexcept
	{
		MLX_PROFILE_FUNCTION();
		if(m_active_images_allocations != 0)
			Error("Graphics Allocator: some user-dependant allocations were not freed before destroying the display (% active allocations). You may have not destroyed all the MLX resources you've created", m_active_images_allocations);
		else if(m_active_buffers_allocations != 0)
			Error("Graphics Allocator: some MLX-dependant allocations were not freed before destroying the display (% active allocations). This is an error in the MLX, please report this should not happen", m_active_buffers_allocations);
		if(m_active_images_allocations < 0 || m_active_buffers_allocations < 0)
			Warning("Graphics Allocator: the impossible happened, the MLX has freed more allocations than it has made (wtf)");
		vmaDestroyAllocator(m_allocator);
		m_active_buffers_allocations = 0;
		m_active_images_allocations = 0;
		DebugLog("Vulkan: destroyed a graphics allocator");
	}
}
