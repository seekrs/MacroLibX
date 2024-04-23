/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Queues.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 19:02:42 by maldavid          #+#    #+#             */
/*   Updated: 2024/04/23 18:51:21 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <PreCompiled.h>

#include <Renderer/Core/RenderCore.h>

namespace mlx
{
	Queues::QueueFamilyIndices Queues::findQueueFamilies(VkPhysicalDevice device)
    {
		std::uint32_t queue_family_count = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, nullptr);

		std::vector<VkQueueFamilyProperties> queue_families(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families.data());

		n_families = Queues::QueueFamilyIndices{};
		int i = 0;
		for(const auto& queue_family : queue_families)
		{
			if(queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT)
				m_families->graphics_family = i;

			if(glfwGetPhysicalDevicePresentationSupport(RenderCore::Get().GetInstance().Get(), device, i))
				m_families->present_family = i;

			if(m_families->IsComplete())
				return *m_families;
			i++;
		}

		return *m_families;
	}

	void Queues::Init()
	{
		if(!m_families.has_value())
			FindQueueFamilies(RenderCore::Get().GetDevice().GetPhysicalDevice());
		vkGetDeviceQueue(RenderCore::Get().GetDevice().Get(), m_families->graphics_family.value(), 0, &m_graphics_queue);
		vkGetDeviceQueue(RenderCore::Get().GetDevice().Get(), m_families->present_family.value(), 0, &m_present_queue);
		DebugLog("Vulkan : got graphics and present queues");
	}
}
