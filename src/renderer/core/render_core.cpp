/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_core.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/17 23:33:34 by maldavid          #+#    #+#             */
/*   Updated: 2024/10/19 10:50:13 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define VOLK_IMPLEMENTATION

#include <mlx_profile.h>
#include <renderer/core/render_core.h>
#include <renderer/command/vk_cmd_buffer.h>

#ifdef DEBUG
	#ifdef MLX_COMPILER_MSVC
		#pragma NOTE("MLX is being compiled in debug mode, this activates Vulkan's validation layers and debug messages which may impact rendering performances")
	#else
		#warning "MLX is being compiled in debug mode, this activates Vulkan's validation layers and debug messages which may impact rendering performances"
	#endif
#endif

namespace mlx
{
	namespace RCore
	{
		std::optional<std::uint32_t> findMemoryType(std::uint32_t typeFilter, VkMemoryPropertyFlags properties, bool error)
		{
			VkPhysicalDeviceMemoryProperties memProperties;
			vkGetPhysicalDeviceMemoryProperties(Render_Core::get().getDevice().getPhysicalDevice(), &memProperties);

			for(std::uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
			{
				if((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
					return i;
			}
			if(error)
				core::error::report(e_kind::fatal_error, "Vulkan : failed to find suitable memory type");
			return std::nullopt;
		}

		const char* verbaliseResultVk(VkResult result)
		{
			switch(result)
			{
				case VK_SUCCESS: return "Success";
				case VK_NOT_READY: return "A fence or query has not yet completed";
				case VK_TIMEOUT: return "A wait operation has not completed in the specified time";
				case VK_EVENT_SET: return "An event is signaled";
				case VK_EVENT_RESET: return "An event is unsignaled";
				case VK_INCOMPLETE: return "A return array was too small for the result";
				case VK_ERROR_OUT_OF_HOST_MEMORY: return "A host memory allocation has failed";
				case VK_ERROR_OUT_OF_DEVICE_MEMORY: return "A device memory allocation has failed";
				case VK_ERROR_INITIALIZATION_FAILED: return "Initialization of an object could not be completed for implementation-specific reasons";
				case VK_ERROR_DEVICE_LOST: return "The logical or physical device has been lost";
				case VK_ERROR_MEMORY_MAP_FAILED: return "Mapping of a memory object has failed";
				case VK_ERROR_LAYER_NOT_PRESENT: return "A requested layer is not present or could not be loaded";
				case VK_ERROR_EXTENSION_NOT_PRESENT: return "A requested extension is not supported";
				case VK_ERROR_FEATURE_NOT_PRESENT: return "A requested feature is not supported";
				case VK_ERROR_INCOMPATIBLE_DRIVER: return "The requested version of Vulkan is not supported by the driver or is otherwise incompatible";
				case VK_ERROR_TOO_MANY_OBJECTS: return "Too many objects of the type have already been created";
				case VK_ERROR_FORMAT_NOT_SUPPORTED: return "A requested format is not supported on this device";
				case VK_ERROR_SURFACE_LOST_KHR: return "A surface is no longer available";
				case VK_SUBOPTIMAL_KHR: return "A swapchain no longer matches the surface properties exactly, but can still be used";
				case VK_ERROR_OUT_OF_DATE_KHR: return "A surface has changed in such a way that it is no longer compatible with the swapchain";
				case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR: return "The display used by a swapchain does not use the same presentable image layout";
				case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR: return "The requested window is already connected to a VkSurfaceKHR, or to some other non-Vulkan API";
				case VK_ERROR_VALIDATION_FAILED_EXT: return "A validation layer found an error";

				default: return "Unknown Vulkan error";
			}
			return nullptr;
		}

		VkPipelineStageFlags accessFlagsToPipelineStage(VkAccessFlags accessFlags, VkPipelineStageFlags stageFlags)
		{
			VkPipelineStageFlags stages = 0;

			while(accessFlags != 0)
			{
				VkAccessFlagBits AccessFlag = static_cast<VkAccessFlagBits>(accessFlags & (~(accessFlags - 1)));
				if(AccessFlag == 0 || (AccessFlag & (AccessFlag - 1)) != 0)
					core::error::report(e_kind::fatal_error, "Vulkan : an error has been caught during access flag to pipeline stage operation");
				accessFlags &= ~AccessFlag;

				switch(AccessFlag)
				{
					case VK_ACCESS_INDIRECT_COMMAND_READ_BIT: stages |= VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT; break;
					case VK_ACCESS_INDEX_READ_BIT: stages |= VK_PIPELINE_STAGE_VERTEX_INPUT_BIT; break;
					case VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT: stages |= VK_PIPELINE_STAGE_VERTEX_INPUT_BIT; break;
					case VK_ACCESS_UNIFORM_READ_BIT: stages |= stageFlags | VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT; break;
					case VK_ACCESS_INPUT_ATTACHMENT_READ_BIT: stages |= VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT; break;
					case VK_ACCESS_SHADER_READ_BIT: stages |= stageFlags | VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT; break;
					case VK_ACCESS_SHADER_WRITE_BIT: stages |= stageFlags | VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT; break;
					case VK_ACCESS_COLOR_ATTACHMENT_READ_BIT: stages |= VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT; break;
					case VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT: stages |= VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT; break;
					case VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT: stages |= VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT; break;
					case VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT: stages |= VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT; break;
					case VK_ACCESS_TRANSFER_READ_BIT: stages |= VK_PIPELINE_STAGE_TRANSFER_BIT; break;
					case VK_ACCESS_TRANSFER_WRITE_BIT: stages |= VK_PIPELINE_STAGE_TRANSFER_BIT; break;
					case VK_ACCESS_HOST_READ_BIT: stages |= VK_PIPELINE_STAGE_HOST_BIT; break;
					case VK_ACCESS_HOST_WRITE_BIT: stages |= VK_PIPELINE_STAGE_HOST_BIT; break;
					case VK_ACCESS_MEMORY_READ_BIT: break;
					case VK_ACCESS_MEMORY_WRITE_BIT: break;

					default: core::error::report(e_kind::error, "Vulkan : unknown access flag"); break;
				}
			}
			return stages;
		}
	}

	void Render_Core::init()
	{
		if(_is_init)
			return;
		if(volkInitialize() != VK_SUCCESS)
			core::error::report(e_kind::fatal_error, "Vulkan loader : cannot load %s, are you sure Vulkan is installed on your system ?", VULKAN_LIB_NAME);

		_instance.init();
		volkLoadInstance(_instance.get());
		_layers.init();
		_device.init();
		volkLoadDevice(_device.get());
		_queues.init();
		_allocator.init();
		_cmd_manager.init();
		_is_init = true;
	}

	void Render_Core::destroy()
	{
		if(!_is_init)
			return;

		vkDeviceWaitIdle(_device());

		_pool_manager.destroyAllPools();
		_cmd_manager.destroy();
		_allocator.destroy();
		_device.destroy();
		_layers.destroy();
		_instance.destroy();

		volkFinalize();

		_is_init = false;
	}
}
