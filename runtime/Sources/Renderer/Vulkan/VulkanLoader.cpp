#include "Renderer/Vulkan/VulkanPrototypes.h"
#include "vulkan/vulkan_core.h"
#include <PreCompiled.h>
#include <Renderer/Vulkan/VulkanLoader.h>

#ifdef MLX_PLAT_WINDOWS
	__declspec(dllimport) HMODULE __stdcall LoadLibraryA(LPCSTR);
	__declspec(dllimport) FARPROC __stdcall GetProcAddress(HMODULE, LPCSTR);
	__declspec(dllimport) int __stdcall FreeLibrary(HMODULE);
	using LibModule = HMODULE;
#else
	using LibModule = Handle;
#endif

#if defined(MLX_COMPILER_GCC)
	#define DISABLE_GCC_PEDANTIC_WARNINGS \
		_Pragma("GCC diagnostic push") \
		_Pragma("GCC diagnostic ignored \"-Wpedantic\"")
	#define RESTORE_GCC_PEDANTIC_WARNINGS \
		_Pragma("GCC diagnostic pop")
#else
	#define DISABLE_GCC_PEDANTIC_WARNINGS
	#define RESTORE_GCC_PEDANTIC_WARNINGS
#endif

namespace mlx
{
	namespace Internal
	{
		static inline PFN_vkVoidFunction vkGetInstanceProcAddrStub(Handle context, const char* name)
		{
			PFN_vkVoidFunction function = vkGetInstanceProcAddr(static_cast<VkInstance>(context), name);
			if(!function)
				FatalError("Vulkan loader : could not load '%'", name);
			DebugLog("Vulkan loader : loaded %", name);
			return function;
		}

		static inline PFN_vkVoidFunction vkGetDeviceProcAddrStub(Handle context, const char* name)
		{
			PFN_vkVoidFunction function = vkGetDeviceProcAddr(static_cast<VkDevice>(context), name);
			if(!function)
				FatalError("Vulkan loader : could not load '%'", name);
			DebugLog("Vulkan loader : loaded %", name);
			return function;
		}

		static inline LibModule LoadLib(const char* libname)
		{
			#ifdef MLX_PLAT_WINDOWS
				return LoadLibraryA(libname);
			#else
				return dlopen(libname, RTLD_NOW | RTLD_LOCAL);
			#endif
		}

		static inline void* GetSymbol(LibModule module, const char* name)
		{
			#ifdef MLX_PLAT_WINDOWS
				return (void*)(void(*)(void))GetProcAddress(module, name);
			#else
				return dlsym(module, name);
			#endif
		}
	}

	VulkanLoader::VulkanLoader()
	{
		#if defined(MLX_PLAT_WINDOWS)
			std::array libnames{
				"vulkan-1.dll"
			};
		#elif defined(MLX_PLAT_MACOS)
			std::array libnames{
				"libvulkan.dylib",
				"libvulkan.1.dylib",
				"libMoltenVK.dylib",
				"vulkan.framework/vulkan",
				"MoltenVK.framework/MoltenVK",
				"/usr/local/lib/libvulkan.dylib",
			};
		#else
			std::array libnames{
				"libvulkan.so.1",
				"libvulkan.so"
			};
		#endif

		for(auto libname : libnames)
		{
			p_module = Internal::LoadLib(libname);
			if(p_module != nullptr)
				break;
		}
		if(!p_module)
			FatalError("Vulkan loader : failed to load libvulkan");

		DISABLE_GCC_PEDANTIC_WARNINGS
		vkGetInstanceProcAddr = reinterpret_cast<PFN_vkGetInstanceProcAddr>(Internal::GetSymbol(p_module, "vkGetInstanceProcAddr"));
		RESTORE_GCC_PEDANTIC_WARNINGS

		if(!vkGetInstanceProcAddr)
			FatalError("Vulkan loader : could not get symbol for 'vkGetInstanceProcAddr'");
		DebugLog("Vulkan loader : libvulkan loaded");
		LoadGlobalFunctions(NULL, Internal::vkGetInstanceProcAddrStub);
	}

	void VulkanLoader::LoadInstance(VkInstance instance)
	{
		LoadInstanceFunctions(instance, Internal::vkGetInstanceProcAddrStub);
	}

	void VulkanLoader::LoadDevice(VkDevice device)
	{
		LoadDeviceFunctions(device, Internal::vkGetDeviceProcAddrStub);
	}

	void VulkanLoader::LoadGlobalFunctions(void* context, PFN_vkVoidFunction (*load)(void*, const char*)) noexcept
	{
		#ifdef VK_VERSION_1_0
			vkCreateInstance = reinterpret_cast<PFN_vkCreateInstance>(load(context, "vkCreateInstance"));
			vkEnumerateInstanceExtensionProperties = reinterpret_cast<PFN_vkEnumerateInstanceExtensionProperties>(load(context, "vkEnumerateInstanceExtensionProperties"));
			vkEnumerateInstanceLayerProperties = reinterpret_cast<PFN_vkEnumerateInstanceLayerProperties>(load(context, "vkEnumerateInstanceLayerProperties"));
		#endif
		DebugLog("Vulkan loader : global functions loaded");
	}

	void VulkanLoader::LoadInstanceFunctions(void* context, PFN_vkVoidFunction (*load)(void*, const char*)) noexcept
	{
		#ifdef VK_VERSION_1_0
			vkCreateDevice = reinterpret_cast<PFN_vkCreateDevice>(load(context, "vkCreateDevice"));
			vkDestroyInstance = reinterpret_cast<PFN_vkDestroyInstance>(load(context, "vkDestroyInstance"));
			vkEnumerateDeviceExtensionProperties = reinterpret_cast<PFN_vkEnumerateDeviceExtensionProperties>(load(context, "vkEnumerateDeviceExtensionProperties"));
			vkEnumerateDeviceLayerProperties = reinterpret_cast<PFN_vkEnumerateDeviceLayerProperties>(load(context, "vkEnumerateDeviceLayerProperties"));
			vkEnumeratePhysicalDevices = reinterpret_cast<PFN_vkEnumeratePhysicalDevices>(load(context, "vkEnumeratePhysicalDevices"));
			vkGetDeviceProcAddr = reinterpret_cast<PFN_vkGetDeviceProcAddr>(load(context, "vkGetDeviceProcAddr"));
			vkGetPhysicalDeviceFeatures = reinterpret_cast<PFN_vkGetPhysicalDeviceFeatures>(load(context, "vkGetPhysicalDeviceFeatures"));
			vkGetPhysicalDeviceFormatProperties = reinterpret_cast<PFN_vkGetPhysicalDeviceFormatProperties>(load(context, "vkGetPhysicalDeviceFormatProperties"));
			vkGetPhysicalDeviceImageFormatProperties = reinterpret_cast<PFN_vkGetPhysicalDeviceImageFormatProperties>(load(context, "vkGetPhysicalDeviceImageFormatProperties"));
			vkGetPhysicalDeviceMemoryProperties = reinterpret_cast<PFN_vkGetPhysicalDeviceMemoryProperties>(load(context, "vkGetPhysicalDeviceMemoryProperties"));
			vkGetPhysicalDeviceProperties = reinterpret_cast<PFN_vkGetPhysicalDeviceProperties>(load(context, "vkGetPhysicalDeviceProperties"));
			vkGetPhysicalDeviceQueueFamilyProperties = reinterpret_cast<PFN_vkGetPhysicalDeviceQueueFamilyProperties>(load(context, "vkGetPhysicalDeviceQueueFamilyProperties"));
			vkGetPhysicalDeviceSparseImageFormatProperties = reinterpret_cast<PFN_vkGetPhysicalDeviceSparseImageFormatProperties>(load(context, "vkGetPhysicalDeviceSparseImageFormatProperties"));
		#endif
		#ifdef VK_KHR_surface
			vkDestroySurfaceKHR = reinterpret_cast<PFN_vkDestroySurfaceKHR>(load(context, "vkDestroySurfaceKHR"));
			vkGetPhysicalDeviceSurfaceCapabilitiesKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR>(load(context, "vkGetPhysicalDeviceSurfaceCapabilitiesKHR"));
			vkGetPhysicalDeviceSurfaceFormatsKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceSurfaceFormatsKHR>(load(context, "vkGetPhysicalDeviceSurfaceFormatsKHR"));
			vkGetPhysicalDeviceSurfacePresentModesKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceSurfacePresentModesKHR>(load(context, "vkGetPhysicalDeviceSurfacePresentModesKHR"));
			vkGetPhysicalDeviceSurfaceSupportKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceSurfaceSupportKHR>(load(context, "vkGetPhysicalDeviceSurfaceSupportKHR"));
		#endif
		DebugLog("Vulkan loader : instance functions loaded");
	}

	void VulkanLoader::LoadDeviceFunctions(void* context, PFN_vkVoidFunction (*load)(void*, const char*)) noexcept
	{
		#ifdef VK_VERSION_1_0
			vkAllocateCommandBuffers = reinterpret_cast<PFN_vkAllocateCommandBuffers>(load(context, "vkAllocateCommandBuffers"));
			vkAllocateDescriptorSets = reinterpret_cast<PFN_vkAllocateDescriptorSets>(load(context, "vkAllocateDescriptorSets"));
			vkAllocateMemory = reinterpret_cast<PFN_vkAllocateMemory>(load(context, "vkAllocateMemory"));
			vkBeginCommandBuffer = reinterpret_cast<PFN_vkBeginCommandBuffer>(load(context, "vkBeginCommandBuffer"));
			vkBindBufferMemory = reinterpret_cast<PFN_vkBindBufferMemory>(load(context, "vkBindBufferMemory"));
			vkBindImageMemory = reinterpret_cast<PFN_vkBindImageMemory>(load(context, "vkBindImageMemory"));
			vkCmdBeginQuery = reinterpret_cast<PFN_vkCmdBeginQuery>(load(context, "vkCmdBeginQuery"));
			vkCmdBeginRenderPass = reinterpret_cast<PFN_vkCmdBeginRenderPass>(load(context, "vkCmdBeginRenderPass"));
			vkCmdBindDescriptorSets = reinterpret_cast<PFN_vkCmdBindDescriptorSets>(load(context, "vkCmdBindDescriptorSets"));
			vkCmdBindIndexBuffer = reinterpret_cast<PFN_vkCmdBindIndexBuffer>(load(context, "vkCmdBindIndexBuffer"));
			vkCmdBindPipeline = reinterpret_cast<PFN_vkCmdBindPipeline>(load(context, "vkCmdBindPipeline"));
			vkCmdBindVertexBuffers = reinterpret_cast<PFN_vkCmdBindVertexBuffers>(load(context, "vkCmdBindVertexBuffers"));
			vkCmdBlitImage = reinterpret_cast<PFN_vkCmdBlitImage>(load(context, "vkCmdBlitImage"));
			vkCmdClearAttachments = reinterpret_cast<PFN_vkCmdClearAttachments>(load(context, "vkCmdClearAttachments"));
			vkCmdClearColorImage = reinterpret_cast<PFN_vkCmdClearColorImage>(load(context, "vkCmdClearColorImage"));
			vkCmdClearDepthStencilImage = reinterpret_cast<PFN_vkCmdClearDepthStencilImage>(load(context, "vkCmdClearDepthStencilImage"));
			vkCmdCopyBuffer = reinterpret_cast<PFN_vkCmdCopyBuffer>(load(context, "vkCmdCopyBuffer"));
			vkCmdCopyBufferToImage = reinterpret_cast<PFN_vkCmdCopyBufferToImage>(load(context, "vkCmdCopyBufferToImage"));
			vkCmdCopyImage = reinterpret_cast<PFN_vkCmdCopyImage>(load(context, "vkCmdCopyImage"));
			vkCmdCopyImageToBuffer = reinterpret_cast<PFN_vkCmdCopyImageToBuffer>(load(context, "vkCmdCopyImageToBuffer"));
			vkCmdCopyQueryPoolResults = reinterpret_cast<PFN_vkCmdCopyQueryPoolResults>(load(context, "vkCmdCopyQueryPoolResults"));
			vkCmdDispatch = reinterpret_cast<PFN_vkCmdDispatch>(load(context, "vkCmdDispatch"));
			vkCmdDispatchIndirect = reinterpret_cast<PFN_vkCmdDispatchIndirect>(load(context, "vkCmdDispatchIndirect"));
			vkCmdDraw = reinterpret_cast<PFN_vkCmdDraw>(load(context, "vkCmdDraw"));
			vkCmdDrawIndexed = reinterpret_cast<PFN_vkCmdDrawIndexed>(load(context, "vkCmdDrawIndexed"));
			vkCmdDrawIndexedIndirect = reinterpret_cast<PFN_vkCmdDrawIndexedIndirect>(load(context, "vkCmdDrawIndexedIndirect"));
			vkCmdDrawIndirect = reinterpret_cast<PFN_vkCmdDrawIndirect>(load(context, "vkCmdDrawIndirect"));
			vkCmdEndQuery = reinterpret_cast<PFN_vkCmdEndQuery>(load(context, "vkCmdEndQuery"));
			vkCmdEndRenderPass = reinterpret_cast<PFN_vkCmdEndRenderPass>(load(context, "vkCmdEndRenderPass"));
			vkCmdExecuteCommands = reinterpret_cast<PFN_vkCmdExecuteCommands>(load(context, "vkCmdExecuteCommands"));
			vkCmdFillBuffer = reinterpret_cast<PFN_vkCmdFillBuffer>(load(context, "vkCmdFillBuffer"));
			vkCmdNextSubpass = reinterpret_cast<PFN_vkCmdNextSubpass>(load(context, "vkCmdNextSubpass"));
			vkCmdPipelineBarrier = reinterpret_cast<PFN_vkCmdPipelineBarrier>(load(context, "vkCmdPipelineBarrier"));
			vkCmdPushConstants = reinterpret_cast<PFN_vkCmdPushConstants>(load(context, "vkCmdPushConstants"));
			vkCmdResetEvent = reinterpret_cast<PFN_vkCmdResetEvent>(load(context, "vkCmdResetEvent"));
			vkCmdResetQueryPool = reinterpret_cast<PFN_vkCmdResetQueryPool>(load(context, "vkCmdResetQueryPool"));
			vkCmdResolveImage = reinterpret_cast<PFN_vkCmdResolveImage>(load(context, "vkCmdResolveImage"));
			vkCmdSetBlendConstants = reinterpret_cast<PFN_vkCmdSetBlendConstants>(load(context, "vkCmdSetBlendConstants"));
			vkCmdSetDepthBias = reinterpret_cast<PFN_vkCmdSetDepthBias>(load(context, "vkCmdSetDepthBias"));
			vkCmdSetDepthBounds = reinterpret_cast<PFN_vkCmdSetDepthBounds>(load(context, "vkCmdSetDepthBounds"));
			vkCmdSetEvent = reinterpret_cast<PFN_vkCmdSetEvent>(load(context, "vkCmdSetEvent"));
			vkCmdSetLineWidth = reinterpret_cast<PFN_vkCmdSetLineWidth>(load(context, "vkCmdSetLineWidth"));
			vkCmdSetScissor = reinterpret_cast<PFN_vkCmdSetScissor>(load(context, "vkCmdSetScissor"));
			vkCmdSetStencilCompareMask = reinterpret_cast<PFN_vkCmdSetStencilCompareMask>(load(context, "vkCmdSetStencilCompareMask"));
			vkCmdSetStencilReference = reinterpret_cast<PFN_vkCmdSetStencilReference>(load(context, "vkCmdSetStencilReference"));
			vkCmdSetStencilWriteMask = reinterpret_cast<PFN_vkCmdSetStencilWriteMask>(load(context, "vkCmdSetStencilWriteMask"));
			vkCmdSetViewport = reinterpret_cast<PFN_vkCmdSetViewport>(load(context, "vkCmdSetViewport"));
			vkCmdUpdateBuffer = reinterpret_cast<PFN_vkCmdUpdateBuffer>(load(context, "vkCmdUpdateBuffer"));
			vkCmdWaitEvents = reinterpret_cast<PFN_vkCmdWaitEvents>(load(context, "vkCmdWaitEvents"));
			vkCmdWriteTimestamp = reinterpret_cast<PFN_vkCmdWriteTimestamp>(load(context, "vkCmdWriteTimestamp"));
			vkCreateBuffer = reinterpret_cast<PFN_vkCreateBuffer>(load(context, "vkCreateBuffer"));
			vkCreateBufferView = reinterpret_cast<PFN_vkCreateBufferView>(load(context, "vkCreateBufferView"));
			vkCreateCommandPool = reinterpret_cast<PFN_vkCreateCommandPool>(load(context, "vkCreateCommandPool"));
			vkCreateComputePipelines = reinterpret_cast<PFN_vkCreateComputePipelines>(load(context, "vkCreateComputePipelines"));
			vkCreateDescriptorPool = reinterpret_cast<PFN_vkCreateDescriptorPool>(load(context, "vkCreateDescriptorPool"));
			vkCreateDescriptorSetLayout = reinterpret_cast<PFN_vkCreateDescriptorSetLayout>(load(context, "vkCreateDescriptorSetLayout"));
			vkCreateEvent = reinterpret_cast<PFN_vkCreateEvent>(load(context, "vkCreateEvent"));
			vkCreateFence = reinterpret_cast<PFN_vkCreateFence>(load(context, "vkCreateFence"));
			vkCreateFramebuffer = reinterpret_cast<PFN_vkCreateFramebuffer>(load(context, "vkCreateFramebuffer"));
			vkCreateGraphicsPipelines = reinterpret_cast<PFN_vkCreateGraphicsPipelines>(load(context, "vkCreateGraphicsPipelines"));
			vkCreateImage = reinterpret_cast<PFN_vkCreateImage>(load(context, "vkCreateImage"));
			vkCreateImageView = reinterpret_cast<PFN_vkCreateImageView>(load(context, "vkCreateImageView"));
			vkCreatePipelineCache = reinterpret_cast<PFN_vkCreatePipelineCache>(load(context, "vkCreatePipelineCache"));
			vkCreatePipelineLayout = reinterpret_cast<PFN_vkCreatePipelineLayout>(load(context, "vkCreatePipelineLayout"));
			vkCreateQueryPool = reinterpret_cast<PFN_vkCreateQueryPool>(load(context, "vkCreateQueryPool"));
			vkCreateRenderPass = reinterpret_cast<PFN_vkCreateRenderPass>(load(context, "vkCreateRenderPass"));
			vkCreateSampler = reinterpret_cast<PFN_vkCreateSampler>(load(context, "vkCreateSampler"));
			vkCreateSemaphore = reinterpret_cast<PFN_vkCreateSemaphore>(load(context, "vkCreateSemaphore"));
			vkCreateShaderModule = reinterpret_cast<PFN_vkCreateShaderModule>(load(context, "vkCreateShaderModule"));
			vkDestroyBuffer = reinterpret_cast<PFN_vkDestroyBuffer>(load(context, "vkDestroyBuffer"));
			vkDestroyBufferView = reinterpret_cast<PFN_vkDestroyBufferView>(load(context, "vkDestroyBufferView"));
			vkDestroyCommandPool = reinterpret_cast<PFN_vkDestroyCommandPool>(load(context, "vkDestroyCommandPool"));
			vkDestroyDescriptorPool = reinterpret_cast<PFN_vkDestroyDescriptorPool>(load(context, "vkDestroyDescriptorPool"));
			vkDestroyDescriptorSetLayout = reinterpret_cast<PFN_vkDestroyDescriptorSetLayout>(load(context, "vkDestroyDescriptorSetLayout"));
			vkDestroyDevice = reinterpret_cast<PFN_vkDestroyDevice>(load(context, "vkDestroyDevice"));
			vkDestroyEvent = reinterpret_cast<PFN_vkDestroyEvent>(load(context, "vkDestroyEvent"));
			vkDestroyFence = reinterpret_cast<PFN_vkDestroyFence>(load(context, "vkDestroyFence"));
			vkDestroyFramebuffer = reinterpret_cast<PFN_vkDestroyFramebuffer>(load(context, "vkDestroyFramebuffer"));
			vkDestroyImage = reinterpret_cast<PFN_vkDestroyImage>(load(context, "vkDestroyImage"));
			vkDestroyImageView = reinterpret_cast<PFN_vkDestroyImageView>(load(context, "vkDestroyImageView"));
			vkDestroyPipeline = reinterpret_cast<PFN_vkDestroyPipeline>(load(context, "vkDestroyPipeline"));
			vkDestroyPipelineCache = reinterpret_cast<PFN_vkDestroyPipelineCache>(load(context, "vkDestroyPipelineCache"));
			vkDestroyPipelineLayout = reinterpret_cast<PFN_vkDestroyPipelineLayout>(load(context, "vkDestroyPipelineLayout"));
			vkDestroyQueryPool = reinterpret_cast<PFN_vkDestroyQueryPool>(load(context, "vkDestroyQueryPool"));
			vkDestroyRenderPass = reinterpret_cast<PFN_vkDestroyRenderPass>(load(context, "vkDestroyRenderPass"));
			vkDestroySampler = reinterpret_cast<PFN_vkDestroySampler>(load(context, "vkDestroySampler"));
			vkDestroySemaphore = reinterpret_cast<PFN_vkDestroySemaphore>(load(context, "vkDestroySemaphore"));
			vkDestroyShaderModule = reinterpret_cast<PFN_vkDestroyShaderModule>(load(context, "vkDestroyShaderModule"));
			vkDeviceWaitIdle = reinterpret_cast<PFN_vkDeviceWaitIdle>(load(context, "vkDeviceWaitIdle"));
			vkEndCommandBuffer = reinterpret_cast<PFN_vkEndCommandBuffer>(load(context, "vkEndCommandBuffer"));
			vkFlushMappedMemoryRanges = reinterpret_cast<PFN_vkFlushMappedMemoryRanges>(load(context, "vkFlushMappedMemoryRanges"));
			vkFreeCommandBuffers = reinterpret_cast<PFN_vkFreeCommandBuffers>(load(context, "vkFreeCommandBuffers"));
			vkFreeDescriptorSets = reinterpret_cast<PFN_vkFreeDescriptorSets>(load(context, "vkFreeDescriptorSets"));
			vkFreeMemory = reinterpret_cast<PFN_vkFreeMemory>(load(context, "vkFreeMemory"));
			vkGetBufferMemoryRequirements = reinterpret_cast<PFN_vkGetBufferMemoryRequirements>(load(context, "vkGetBufferMemoryRequirements"));
			vkGetDeviceMemoryCommitment = reinterpret_cast<PFN_vkGetDeviceMemoryCommitment>(load(context, "vkGetDeviceMemoryCommitment"));
			vkGetDeviceQueue = reinterpret_cast<PFN_vkGetDeviceQueue>(load(context, "vkGetDeviceQueue"));
			vkGetEventStatus = reinterpret_cast<PFN_vkGetEventStatus>(load(context, "vkGetEventStatus"));
			vkGetFenceStatus = reinterpret_cast<PFN_vkGetFenceStatus>(load(context, "vkGetFenceStatus"));
			vkGetImageMemoryRequirements = reinterpret_cast<PFN_vkGetImageMemoryRequirements>(load(context, "vkGetImageMemoryRequirements"));
			vkGetImageSparseMemoryRequirements = reinterpret_cast<PFN_vkGetImageSparseMemoryRequirements>(load(context, "vkGetImageSparseMemoryRequirements"));
			vkGetImageSubresourceLayout = reinterpret_cast<PFN_vkGetImageSubresourceLayout>(load(context, "vkGetImageSubresourceLayout"));
			vkGetPipelineCacheData = reinterpret_cast<PFN_vkGetPipelineCacheData>(load(context, "vkGetPipelineCacheData"));
			vkGetQueryPoolResults = reinterpret_cast<PFN_vkGetQueryPoolResults>(load(context, "vkGetQueryPoolResults"));
			vkGetRenderAreaGranularity = reinterpret_cast<PFN_vkGetRenderAreaGranularity>(load(context, "vkGetRenderAreaGranularity"));
			vkInvalidateMappedMemoryRanges = reinterpret_cast<PFN_vkInvalidateMappedMemoryRanges>(load(context, "vkInvalidateMappedMemoryRanges"));
			vkMapMemory = reinterpret_cast<PFN_vkMapMemory>(load(context, "vkMapMemory"));
			vkMergePipelineCaches = reinterpret_cast<PFN_vkMergePipelineCaches>(load(context, "vkMergePipelineCaches"));
			vkQueueBindSparse = reinterpret_cast<PFN_vkQueueBindSparse>(load(context, "vkQueueBindSparse"));
			vkQueueSubmit = reinterpret_cast<PFN_vkQueueSubmit>(load(context, "vkQueueSubmit"));
			vkQueueWaitIdle = reinterpret_cast<PFN_vkQueueWaitIdle>(load(context, "vkQueueWaitIdle"));
			vkResetCommandBuffer = reinterpret_cast<PFN_vkResetCommandBuffer>(load(context, "vkResetCommandBuffer"));
			vkResetCommandPool = reinterpret_cast<PFN_vkResetCommandPool>(load(context, "vkResetCommandPool"));
			vkResetDescriptorPool = reinterpret_cast<PFN_vkResetDescriptorPool>(load(context, "vkResetDescriptorPool"));
			vkResetEvent = reinterpret_cast<PFN_vkResetEvent>(load(context, "vkResetEvent"));
			vkResetFences = reinterpret_cast<PFN_vkResetFences>(load(context, "vkResetFences"));
			vkSetEvent = reinterpret_cast<PFN_vkSetEvent>(load(context, "vkSetEvent"));
			vkUnmapMemory = reinterpret_cast<PFN_vkUnmapMemory>(load(context, "vkUnmapMemory"));
			vkUpdateDescriptorSets = reinterpret_cast<PFN_vkUpdateDescriptorSets>(load(context, "vkUpdateDescriptorSets"));
			vkWaitForFences = reinterpret_cast<PFN_vkWaitForFences>(load(context, "vkWaitForFences"));
		#endif
		#ifdef VK_KHR_swapchain
			vkAcquireNextImageKHR = reinterpret_cast<PFN_vkAcquireNextImageKHR>(load(context, "vkAcquireNextImageKHR"));
			vkCreateSwapchainKHR = reinterpret_cast<PFN_vkCreateSwapchainKHR>(load(context, "vkCreateSwapchainKHR"));
			vkDestroySwapchainKHR = reinterpret_cast<PFN_vkDestroySwapchainKHR>(load(context, "vkDestroySwapchainKHR"));
			vkGetSwapchainImagesKHR = reinterpret_cast<PFN_vkGetSwapchainImagesKHR>(load(context, "vkGetSwapchainImagesKHR"));
			vkQueuePresentKHR = reinterpret_cast<PFN_vkQueuePresentKHR>(load(context, "vkQueuePresentKHR"));
		#endif

		DebugLog("Vulkan loader : device functions loaded");
	}

	VulkanLoader::~VulkanLoader()
	{
		#ifdef MLX_PLAT_WINDOWS
			FreeLibrary(p_module);
		#else
			dlclose(p_module);
		#endif
		p_module = nullptr;
		DebugLog("Vulkan loader : libvulkan unloaded");
	}
}

#ifdef VK_VERSION_1_0
	PFN_vkAllocateCommandBuffers vkAllocateCommandBuffers;
	PFN_vkAllocateDescriptorSets vkAllocateDescriptorSets;
	PFN_vkAllocateMemory vkAllocateMemory;
	PFN_vkBeginCommandBuffer vkBeginCommandBuffer;
	PFN_vkBindBufferMemory vkBindBufferMemory;
	PFN_vkBindImageMemory vkBindImageMemory;
	PFN_vkCmdBeginQuery vkCmdBeginQuery;
	PFN_vkCmdBeginRenderPass vkCmdBeginRenderPass;
	PFN_vkCmdBindDescriptorSets vkCmdBindDescriptorSets;
	PFN_vkCmdBindIndexBuffer vkCmdBindIndexBuffer;
	PFN_vkCmdBindPipeline vkCmdBindPipeline;
	PFN_vkCmdBindVertexBuffers vkCmdBindVertexBuffers;
	PFN_vkCmdBlitImage vkCmdBlitImage;
	PFN_vkCmdClearAttachments vkCmdClearAttachments;
	PFN_vkCmdClearColorImage vkCmdClearColorImage;
	PFN_vkCmdClearDepthStencilImage vkCmdClearDepthStencilImage;
	PFN_vkCmdCopyBuffer vkCmdCopyBuffer;
	PFN_vkCmdCopyBufferToImage vkCmdCopyBufferToImage;
	PFN_vkCmdCopyImage vkCmdCopyImage;
	PFN_vkCmdCopyImageToBuffer vkCmdCopyImageToBuffer;
	PFN_vkCmdCopyQueryPoolResults vkCmdCopyQueryPoolResults;
	PFN_vkCmdDispatch vkCmdDispatch;
	PFN_vkCmdDispatchIndirect vkCmdDispatchIndirect;
	PFN_vkCmdDraw vkCmdDraw;
	PFN_vkCmdDrawIndexed vkCmdDrawIndexed;
	PFN_vkCmdDrawIndexedIndirect vkCmdDrawIndexedIndirect;
	PFN_vkCmdDrawIndirect vkCmdDrawIndirect;
	PFN_vkCmdEndQuery vkCmdEndQuery;
	PFN_vkCmdEndRenderPass vkCmdEndRenderPass;
	PFN_vkCmdExecuteCommands vkCmdExecuteCommands;
	PFN_vkCmdFillBuffer vkCmdFillBuffer;
	PFN_vkCmdNextSubpass vkCmdNextSubpass;
	PFN_vkCmdPipelineBarrier vkCmdPipelineBarrier;
	PFN_vkCmdPushConstants vkCmdPushConstants;
	PFN_vkCmdResetEvent vkCmdResetEvent;
	PFN_vkCmdResetQueryPool vkCmdResetQueryPool;
	PFN_vkCmdResolveImage vkCmdResolveImage;
	PFN_vkCmdSetBlendConstants vkCmdSetBlendConstants;
	PFN_vkCmdSetDepthBias vkCmdSetDepthBias;
	PFN_vkCmdSetDepthBounds vkCmdSetDepthBounds;
	PFN_vkCmdSetEvent vkCmdSetEvent;
	PFN_vkCmdSetLineWidth vkCmdSetLineWidth;
	PFN_vkCmdSetScissor vkCmdSetScissor;
	PFN_vkCmdSetStencilCompareMask vkCmdSetStencilCompareMask;
	PFN_vkCmdSetStencilReference vkCmdSetStencilReference;
	PFN_vkCmdSetStencilWriteMask vkCmdSetStencilWriteMask;
	PFN_vkCmdSetViewport vkCmdSetViewport;
	PFN_vkCmdUpdateBuffer vkCmdUpdateBuffer;
	PFN_vkCmdWaitEvents vkCmdWaitEvents;
	PFN_vkCmdWriteTimestamp vkCmdWriteTimestamp;
	PFN_vkCreateBuffer vkCreateBuffer;
	PFN_vkCreateBufferView vkCreateBufferView;
	PFN_vkCreateCommandPool vkCreateCommandPool;
	PFN_vkCreateComputePipelines vkCreateComputePipelines;
	PFN_vkCreateDescriptorPool vkCreateDescriptorPool;
	PFN_vkCreateDescriptorSetLayout vkCreateDescriptorSetLayout;
	PFN_vkCreateDevice vkCreateDevice;
	PFN_vkCreateEvent vkCreateEvent;
	PFN_vkCreateFence vkCreateFence;
	PFN_vkCreateFramebuffer vkCreateFramebuffer;
	PFN_vkCreateGraphicsPipelines vkCreateGraphicsPipelines;
	PFN_vkCreateImage vkCreateImage;
	PFN_vkCreateImageView vkCreateImageView;
	PFN_vkCreateInstance vkCreateInstance;
	PFN_vkCreatePipelineCache vkCreatePipelineCache;
	PFN_vkCreatePipelineLayout vkCreatePipelineLayout;
	PFN_vkCreateQueryPool vkCreateQueryPool;
	PFN_vkCreateRenderPass vkCreateRenderPass;
	PFN_vkCreateSampler vkCreateSampler;
	PFN_vkCreateSemaphore vkCreateSemaphore;
	PFN_vkCreateShaderModule vkCreateShaderModule;
	PFN_vkDestroyBuffer vkDestroyBuffer;
	PFN_vkDestroyBufferView vkDestroyBufferView;
	PFN_vkDestroyCommandPool vkDestroyCommandPool;
	PFN_vkDestroyDescriptorPool vkDestroyDescriptorPool;
	PFN_vkDestroyDescriptorSetLayout vkDestroyDescriptorSetLayout;
	PFN_vkDestroyDevice vkDestroyDevice;
	PFN_vkDestroyEvent vkDestroyEvent;
	PFN_vkDestroyFence vkDestroyFence;
	PFN_vkDestroyFramebuffer vkDestroyFramebuffer;
	PFN_vkDestroyImage vkDestroyImage;
	PFN_vkDestroyImageView vkDestroyImageView;
	PFN_vkDestroyInstance vkDestroyInstance;
	PFN_vkDestroyPipeline vkDestroyPipeline;
	PFN_vkDestroyPipelineCache vkDestroyPipelineCache;
	PFN_vkDestroyPipelineLayout vkDestroyPipelineLayout;
	PFN_vkDestroyQueryPool vkDestroyQueryPool;
	PFN_vkDestroyRenderPass vkDestroyRenderPass;
	PFN_vkDestroySampler vkDestroySampler;
	PFN_vkDestroySemaphore vkDestroySemaphore;
	PFN_vkDestroyShaderModule vkDestroyShaderModule;
	PFN_vkDeviceWaitIdle vkDeviceWaitIdle;
	PFN_vkEndCommandBuffer vkEndCommandBuffer;
	PFN_vkEnumerateDeviceExtensionProperties vkEnumerateDeviceExtensionProperties;
	PFN_vkEnumerateDeviceLayerProperties vkEnumerateDeviceLayerProperties;
	PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties;
	PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties;
	PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices;
	PFN_vkFlushMappedMemoryRanges vkFlushMappedMemoryRanges;
	PFN_vkFreeCommandBuffers vkFreeCommandBuffers;
	PFN_vkFreeDescriptorSets vkFreeDescriptorSets;
	PFN_vkFreeMemory vkFreeMemory;
	PFN_vkGetBufferMemoryRequirements vkGetBufferMemoryRequirements;
	PFN_vkGetDeviceMemoryCommitment vkGetDeviceMemoryCommitment;
	PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr;
	PFN_vkGetDeviceQueue vkGetDeviceQueue;
	PFN_vkGetEventStatus vkGetEventStatus;
	PFN_vkGetFenceStatus vkGetFenceStatus;
	PFN_vkGetImageMemoryRequirements vkGetImageMemoryRequirements;
	PFN_vkGetImageSparseMemoryRequirements vkGetImageSparseMemoryRequirements;
	PFN_vkGetImageSubresourceLayout vkGetImageSubresourceLayout;
	PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;
	PFN_vkGetPhysicalDeviceFeatures vkGetPhysicalDeviceFeatures;
	PFN_vkGetPhysicalDeviceFormatProperties vkGetPhysicalDeviceFormatProperties;
	PFN_vkGetPhysicalDeviceImageFormatProperties vkGetPhysicalDeviceImageFormatProperties;
	PFN_vkGetPhysicalDeviceMemoryProperties vkGetPhysicalDeviceMemoryProperties;
	PFN_vkGetPhysicalDeviceProperties vkGetPhysicalDeviceProperties;
	PFN_vkGetPhysicalDeviceQueueFamilyProperties vkGetPhysicalDeviceQueueFamilyProperties;
	PFN_vkGetPhysicalDeviceSparseImageFormatProperties vkGetPhysicalDeviceSparseImageFormatProperties;
	PFN_vkGetPipelineCacheData vkGetPipelineCacheData;
	PFN_vkGetQueryPoolResults vkGetQueryPoolResults;
	PFN_vkGetRenderAreaGranularity vkGetRenderAreaGranularity;
	PFN_vkInvalidateMappedMemoryRanges vkInvalidateMappedMemoryRanges;
	PFN_vkMapMemory vkMapMemory;
	PFN_vkMergePipelineCaches vkMergePipelineCaches;
	PFN_vkQueueBindSparse vkQueueBindSparse;
	PFN_vkQueueSubmit vkQueueSubmit;
	PFN_vkQueueWaitIdle vkQueueWaitIdle;
	PFN_vkResetCommandBuffer vkResetCommandBuffer;
	PFN_vkResetCommandPool vkResetCommandPool;
	PFN_vkResetDescriptorPool vkResetDescriptorPool;
	PFN_vkResetEvent vkResetEvent;
	PFN_vkResetFences vkResetFences;
	PFN_vkSetEvent vkSetEvent;
	PFN_vkUnmapMemory vkUnmapMemory;
	PFN_vkUpdateDescriptorSets vkUpdateDescriptorSets;
	PFN_vkWaitForFences vkWaitForFences;
#endif
#ifdef VK_KHR_swapchain
	PFN_vkAcquireNextImageKHR vkAcquireNextImageKHR;
	PFN_vkCreateSwapchainKHR vkCreateSwapchainKHR;
	PFN_vkDestroySwapchainKHR vkDestroySwapchainKHR;
	PFN_vkGetSwapchainImagesKHR vkGetSwapchainImagesKHR;
	PFN_vkQueuePresentKHR vkQueuePresentKHR;
#endif
#ifdef VK_KHR_surface
	PFN_vkDestroySurfaceKHR vkDestroySurfaceKHR;
	PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR vkGetPhysicalDeviceSurfaceCapabilitiesKHR;
	PFN_vkGetPhysicalDeviceSurfaceFormatsKHR vkGetPhysicalDeviceSurfaceFormatsKHR;
	PFN_vkGetPhysicalDeviceSurfacePresentModesKHR vkGetPhysicalDeviceSurfacePresentModesKHR;
	PFN_vkGetPhysicalDeviceSurfaceSupportKHR vkGetPhysicalDeviceSurfaceSupportKHR;
#endif
