/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_queues.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 19:02:42 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/14 17:01:10 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render_core.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>

namespace mlx
{
	Queues::QueueFamilyIndices Queues::findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface)
    {
		std::uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		_families = Queues::QueueFamilyIndices{};
		int i = 0;
		for(const auto& queueFamily : queueFamilies)
		{
			if(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
				_families->graphics_family = i;

			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

			if(presentSupport)
				_families->present_family = i;

			if(_families->isComplete())
				return *_families;
			i++;
		}

		return *_families;
	}

	void Queues::init()
	{
		if(!_families.has_value())
		{
			SDL_Window* window = SDL_CreateWindow("", 0, 0, 1, 1, SDL_WINDOW_VULKAN | SDL_WINDOW_HIDDEN);
			if(!window)
				core::error::report(e_kind::fatal_error, "Vulkan : failed to create a window to init queues");

			VkSurfaceKHR surface = VK_NULL_HANDLE;
			if(SDL_Vulkan_CreateSurface(window, Render_Core::get().getInstance().get(), &surface) != SDL_TRUE)
				core::error::report(e_kind::fatal_error, "Vulkan : failed to create a surface to init queues");

			findQueueFamilies(Render_Core::get().getDevice().getPhysicalDevice(), surface);

			vkDestroySurfaceKHR(Render_Core::get().getInstance().get(), surface, nullptr);
			SDL_DestroyWindow(window);
		}
		vkGetDeviceQueue(Render_Core::get().getDevice().get(), _families->graphics_family.value(), 0, &_graphics_queue);
		vkGetDeviceQueue(Render_Core::get().getDevice().get(), _families->present_family.value(), 0, &_present_queue);
		#ifdef DEBUG
			core::error::report(e_kind::message, "Vulkan : got graphics and present queues");
		#endif
	}
}
