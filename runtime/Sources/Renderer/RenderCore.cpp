#include <PreCompiled.h>
#include <Renderer/RenderCore.h>
#include <Core/Memory.h>

#define KVF_IMPLEMENTATION
#ifdef DEBUG
	#define KVF_ENABLE_VALIDATION_LAYERS
#endif

#define KVF_ASSERT(x) (mlx::Assert(x, "internal kvf assertion " #x))
#define KVF_MALLOC(x) (mlx::MemManager::Get().Malloc(x))
#define KVF_REALLOC(p, x) (mlx::MemManager::Get().Realloc(p, x))
#define KVF_FREE(x) (mlx::MemManager::Get().Free(x))

#if defined(MLX_COMPILER_GCC)
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
		#include <kvf.h>
	#pragma GCC diagnostic pop
#elif defined(MLX_COMPILER_CLANG)
	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Wmissing-field-initializers"
		#include <kvf.h>
	#pragma clang diagnostic pop
#else
	#include <kvf.h>
#endif

#include <mlx.h>
#include <Renderer/Vulkan/VulkanLoader.h>
#include <Core/SDLManager.h>
#include <Platform/Window.h>
#include <Maths/Mat4.h>


namespace mlx
{
	static std::unique_ptr<VulkanLoader> loader;

	void ErrorCallback(const char* message) noexcept
	{
		Logs::Report(LogType::FatalError, 0, "", "", message);
		std::cout << std::endl;
	}

	void ValidationErrorCallback(const char* message) noexcept
	{
		Logs::Report(LogType::Error, 0, "", "", message);
		std::cout << std::endl;
	}

	void WarningCallback(const char* message) noexcept
	{
		Logs::Report(LogType::Warning, 0, "", "", message);
		std::cout << std::endl;
	}

	void* VulkanAllocationFunction(void*, std::size_t size, std::size_t alignment, VkSystemAllocationScope)
	{
		return MemManager::AlignedMalloc(alignment, size);
	}

	void* VulkanReallocationFunction(void*, void* ptr, std::size_t size, std::size_t alignment, VkSystemAllocationScope)
	{
		return MemManager::AlignedRealloc(ptr, alignment, size);
	}

	void VulkanFreeFunction(void*, void* ptr)
	{
		MemManager::Free(ptr);
	}

	RenderCore* RenderCore::s_instance = nullptr;

	RenderCore::RenderCore()
	{
		if(s_instance != nullptr)
			return;
		s_instance = this;

		loader = std::make_unique<VulkanLoader>();

		LoadKVFGlobalVulkanFunctionPointers();

		kvfSetErrorCallback(&ErrorCallback);
		kvfSetWarningCallback(&WarningCallback);
		kvfSetValidationErrorCallback(&ValidationErrorCallback);
		kvfSetValidationWarningCallback(&WarningCallback);

		mlx_window_create_info info{};
		info.title = "";
		info.width = 1;
		info.height = 1;
		Window window(&info, true);
		std::vector<const char*> instance_extensions = window.GetRequiredVulkanInstanceExtentions();
		#ifdef MLX_PLAT_MACOS
			instance_extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
		#endif

		m_instance = kvfCreateInstance(instance_extensions.data(), instance_extensions.size());
		DebugLog("Vulkan: instance created");

		loader->LoadInstance(m_instance);
		LoadKVFInstanceVulkanFunctionPointers();

		VkSurfaceKHR surface = window.CreateVulkanSurface(m_instance);

		m_physical_device = kvfPickGoodDefaultPhysicalDevice(m_instance, surface);

		// just for style
		VkPhysicalDeviceProperties props;
		vkGetPhysicalDeviceProperties(m_physical_device, &props);
		DebugLog("Vulkan: physical device picked '%'", props.deviceName);

		const char* device_extensions[] = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
		VkPhysicalDeviceFeatures features{};
		vkGetPhysicalDeviceFeatures(m_physical_device, &features);
		m_device = kvfCreateDevice(m_physical_device, device_extensions, sizeof(device_extensions) / sizeof(device_extensions[0]), &features);
		DebugLog("Vulkan: logical device created");

		loader->LoadDevice(m_device);
		LoadKVFDeviceVulkanFunctionPointers();

		vkDestroySurfaceKHR(m_instance, surface, nullptr);

		VkAllocationCallbacks callbacks;
		callbacks.pUserData = nullptr;
		callbacks.pfnAllocation = VulkanAllocationFunction;
		callbacks.pfnReallocation = VulkanReallocationFunction;
		callbacks.pfnFree = VulkanFreeFunction;
		callbacks.pfnInternalAllocation = nullptr;
		callbacks.pfnInternalFree = nullptr;

		kvfSetAllocationCallbacks(m_device, &callbacks);

		m_allocator.Init(&callbacks);
	}

#undef MLX_LOAD_FUNCTION
#define MLX_LOAD_FUNCTION(fn) pfns.fn = this->fn

	void RenderCore::LoadKVFGlobalVulkanFunctionPointers() const noexcept
	{
		KvfGlobalVulkanFunctions pfns;
		MLX_LOAD_FUNCTION(vkCreateInstance);
		MLX_LOAD_FUNCTION(vkEnumerateInstanceExtensionProperties);
		MLX_LOAD_FUNCTION(vkEnumerateInstanceLayerProperties);
		MLX_LOAD_FUNCTION(vkGetInstanceProcAddr);
		kvfPassGlobalVulkanFunctionPointers(&pfns);
	}

	void RenderCore::LoadKVFInstanceVulkanFunctionPointers() const noexcept
	{
		KvfInstanceVulkanFunctions pfns;
		MLX_LOAD_FUNCTION(vkCreateDevice);
		MLX_LOAD_FUNCTION(vkDestroyInstance);
		MLX_LOAD_FUNCTION(vkEnumerateDeviceExtensionProperties);
		MLX_LOAD_FUNCTION(vkEnumeratePhysicalDevices);
		MLX_LOAD_FUNCTION(vkGetPhysicalDeviceFeatures);
		MLX_LOAD_FUNCTION(vkGetPhysicalDeviceFormatProperties);
		MLX_LOAD_FUNCTION(vkGetPhysicalDeviceImageFormatProperties);
		MLX_LOAD_FUNCTION(vkGetPhysicalDeviceMemoryProperties);
		MLX_LOAD_FUNCTION(vkGetPhysicalDeviceProperties);
		MLX_LOAD_FUNCTION(vkGetPhysicalDeviceQueueFamilyProperties);
		MLX_LOAD_FUNCTION(vkDestroySurfaceKHR);
		MLX_LOAD_FUNCTION(vkGetPhysicalDeviceSurfaceCapabilitiesKHR);
		MLX_LOAD_FUNCTION(vkGetPhysicalDeviceSurfaceFormatsKHR);
		MLX_LOAD_FUNCTION(vkGetPhysicalDeviceSurfacePresentModesKHR);
		MLX_LOAD_FUNCTION(vkGetPhysicalDeviceSurfaceSupportKHR);
		kvfPassInstanceVulkanFunctionPointers(&pfns);
	}

	void RenderCore::LoadKVFDeviceVulkanFunctionPointers() const noexcept
	{
		KvfDeviceVulkanFunctions pfns;
		MLX_LOAD_FUNCTION(vkAllocateCommandBuffers);
		MLX_LOAD_FUNCTION(vkAllocateDescriptorSets);
		MLX_LOAD_FUNCTION(vkBeginCommandBuffer);
		MLX_LOAD_FUNCTION(vkCmdBeginRenderPass);
		MLX_LOAD_FUNCTION(vkCmdCopyBuffer);
		MLX_LOAD_FUNCTION(vkCmdCopyBufferToImage);
		MLX_LOAD_FUNCTION(vkCmdCopyImage);
		MLX_LOAD_FUNCTION(vkCmdCopyImageToBuffer);
		MLX_LOAD_FUNCTION(vkCmdEndRenderPass);
		MLX_LOAD_FUNCTION(vkCmdPipelineBarrier);
		MLX_LOAD_FUNCTION(vkCreateBuffer);
		MLX_LOAD_FUNCTION(vkCreateCommandPool);
		MLX_LOAD_FUNCTION(vkCreateDescriptorPool);
		MLX_LOAD_FUNCTION(vkCreateDescriptorSetLayout);
		MLX_LOAD_FUNCTION(vkCreateFence);
		MLX_LOAD_FUNCTION(vkCreateFramebuffer);
		MLX_LOAD_FUNCTION(vkCreateGraphicsPipelines);
		MLX_LOAD_FUNCTION(vkCreateImage);
		MLX_LOAD_FUNCTION(vkCreateImageView);
		MLX_LOAD_FUNCTION(vkCreatePipelineLayout);
		MLX_LOAD_FUNCTION(vkCreateRenderPass);
		MLX_LOAD_FUNCTION(vkCreateSampler);
		MLX_LOAD_FUNCTION(vkCreateSemaphore);
		MLX_LOAD_FUNCTION(vkCreateShaderModule);
		MLX_LOAD_FUNCTION(vkDestroyBuffer);
		MLX_LOAD_FUNCTION(vkDestroyCommandPool);
		MLX_LOAD_FUNCTION(vkDestroyDescriptorPool);
		MLX_LOAD_FUNCTION(vkDestroyDescriptorSetLayout);
		MLX_LOAD_FUNCTION(vkDestroyDevice);
		MLX_LOAD_FUNCTION(vkDestroyFence);
		MLX_LOAD_FUNCTION(vkDestroyFramebuffer);
		MLX_LOAD_FUNCTION(vkDestroyImage);
		MLX_LOAD_FUNCTION(vkDestroyImageView);
		MLX_LOAD_FUNCTION(vkDestroyPipeline);
		MLX_LOAD_FUNCTION(vkDestroyPipelineLayout);
		MLX_LOAD_FUNCTION(vkDestroyRenderPass);
		MLX_LOAD_FUNCTION(vkDestroySampler);
		MLX_LOAD_FUNCTION(vkDestroySemaphore);
		MLX_LOAD_FUNCTION(vkDestroyShaderModule);
		MLX_LOAD_FUNCTION(vkDeviceWaitIdle);
		MLX_LOAD_FUNCTION(vkEndCommandBuffer);
		MLX_LOAD_FUNCTION(vkFreeCommandBuffers);
		MLX_LOAD_FUNCTION(vkGetDeviceQueue);
		MLX_LOAD_FUNCTION(vkGetImageSubresourceLayout);
		MLX_LOAD_FUNCTION(vkQueueSubmit);
		MLX_LOAD_FUNCTION(vkResetCommandBuffer);
		MLX_LOAD_FUNCTION(vkResetDescriptorPool);
		MLX_LOAD_FUNCTION(vkResetEvent);
		MLX_LOAD_FUNCTION(vkResetFences);
		MLX_LOAD_FUNCTION(vkUpdateDescriptorSets);
		MLX_LOAD_FUNCTION(vkWaitForFences);
		MLX_LOAD_FUNCTION(vkCreateSwapchainKHR);
		MLX_LOAD_FUNCTION(vkDestroySwapchainKHR);
		MLX_LOAD_FUNCTION(vkGetSwapchainImagesKHR);
		MLX_LOAD_FUNCTION(vkQueuePresentKHR);
		kvfPassDeviceVulkanFunctionPointers(m_physical_device, m_device, &pfns);
	}

#undef MLX_LOAD_FUNCTION

	RenderCore::~RenderCore()
	{
		if(s_instance == nullptr)
			return;
		WaitDeviceIdle();
		m_descriptor_pool_manager.Destroy();
		m_allocator.Destroy();
		kvfDestroyDevice(m_device);
		DebugLog("Vulkan: logical device destroyed");
		kvfDestroyInstance(m_instance);
		DebugLog("Vulkan: instance destroyed");
		loader.reset();

		s_instance = nullptr;
	}
}
