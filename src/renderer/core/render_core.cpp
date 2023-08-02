/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_core.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/17 23:33:34 by maldavid          #+#    #+#             */
/*   Updated: 2023/04/23 19:09:21 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define VOLK_IMPLEMENTATION

#if defined(_WIN32) || defined(_WIN64)
	#define VK_USE_PLATFORM_WIN32_KHR
#elif defined(__APPLE__) || defined(__MACH__)
	#define VK_USE_PLATFORM_MACOS_MVK
#else
	#define VK_USE_PLATFORM_XLIB_KHR
#endif

#include "render_core.h"
#include <mutex>

#ifdef DEBUG
	#warning "MLX is being compiled in debug mode, this activates Vulkan's validation layers and debug messages and may impact rendering performances"
#endif

namespace mlx
{
	namespace RCore
	{
		std::optional<uint32_t> findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, bool error)
		{
			VkPhysicalDeviceMemoryProperties memProperties;
			vkGetPhysicalDeviceMemoryProperties(Render_Core::get().getDevice().getPhysicalDevice(), &memProperties);

			for(uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
			{
				if((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
					return i;
			}
			if(error)
				core::error::report(e_kind::fatal_error, "Vulkan : failed to find suitable memory type");
			return std::nullopt;
		}
	}

	void Render_Core::init()
	{
		volkInitialize();

		_instance.init();
		volkLoadInstance(_instance.get());
		_layers.init();
		_device.init();
		volkLoadDevice(_device.get());
		_queues.init();
		_is_init = true;
	}

	void Render_Core::destroy()
	{
		if(!_is_init)
			return;

		vkDeviceWaitIdle(_device());

		_device.destroy();
		_layers.destroy();
		_instance.destroy();

		_is_init = false;
	}
}
