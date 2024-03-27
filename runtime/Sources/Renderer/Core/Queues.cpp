/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_queues.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 19:02:42 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/25 22:29:19 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pre_compiled.h>

#include "render_core.h"

namespace mlx
{
	Queues::QueueFamilyIndices Queues::findQueueFamilies(VkPhysicalDevice device)
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

			if(glfwGetPhysicalDevicePresentationSupport(Render_Core::get().getInstance().get(), device, i))
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
			findQueueFamilies(Render_Core::get().getDevice().getPhysicalDevice());
		vkGetDeviceQueue(Render_Core::get().getDevice().get(), _families->graphics_family.value(), 0, &_graphics_queue);
		vkGetDeviceQueue(Render_Core::get().getDevice().get(), _families->present_family.value(), 0, &_present_queue);
		#ifdef DEBUG
			core::error::report(e_kind::message, "Vulkan : got graphics and present queues");
		#endif
	}
}
