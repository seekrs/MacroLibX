/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_queues.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 19:02:42 by maldavid          #+#    #+#             */
/*   Updated: 2022/10/08 19:02:52 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render_core.h"

namespace mlx
{
	Queues::QueueFamilyIndices Queues::findQueueFamilies(VkPhysicalDevice device)
    {
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		_families = Queues::QueueFamilyIndices{};

		int i = 0;
		for(const auto& queueFamily : queueFamilies)
		{
			if(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
				_families->graphicsFamily = i;

			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(device, i, Render_Core::get().getSurface().get(), &presentSupport);

			if(presentSupport)
				_families->presentFamily = i;

			if(_families->isComplete())
				break;
			i++;
		}
		return *_families;
	}

	void Queues::init()
	{
		if(!_families.has_value())
			findQueueFamilies(Render_Core::get().getDevice().getPhysicalDevice());
		vkGetDeviceQueue(Render_Core::get().getDevice().get(), _families->graphicsFamily.value(), 0, &_graphicsQueue);
		vkGetDeviceQueue(Render_Core::get().getDevice().get(), _families->presentFamily.value(), 0, &_presentQueue);
	}
}
