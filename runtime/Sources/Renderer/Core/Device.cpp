/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Device.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 19:14:29 by maldavid          #+#    #+#             */
/*   Updated: 2024/04/23 18:10:08 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <PreCmpiled.h>

#include <Renderer/Core/RenderCore.h>

namespace mlx
{
	const std::vector<const char*> device_extensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

	void Device::Init()
	{
		PickPhysicalDevice();

		Queues::QueueFamilyIndices indices = RenderCore::Get().GetQueue().GetFamilies();

		std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
		std::set<std::uint32_t> unique_queue_families = { indices.graphics_family.value(), indices.present_family.value() };

		float queue_priority = 1.0f;
		for(std::uint32_t queue_family : unique_queue_families)
		{
			VkDeviceQueueCreateInfo queue_create_info{};
			queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queue_create_info.queueFamilyIndex = queue_family;
			queue_create_info.queueCount = 1;
			queue_create_info.pQueuePriorities = &queue_priority;
			queue_create_infos.push_back(queue_create_info);
		}

		VkPhysicalDeviceFeatures device_features{};

		VkDeviceCreateInfo create_info{};
		create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		create_info.queueCreateInfoCount = static_cast<std::uint32_t>(queue_create_infos.size());
		create_info.pQueueCreateInfos = queue_create_infos.data();
		create_info.pEnabledFeatures = &device_features;
		create_info.enabledExtensionCount = static_cast<std::uint32_t>(device_extensions.size());
		create_info.ppEnabledExtensionNames = device_extensions.data();
		create_info.enabledLayerCount = 0;

		VkResult res;
		if((res = vkCreateDevice(m_physical_device, &create_info, nullptr, &m_device)) != VK_SUCCESS)
			FatalError("Vulkan : failed to create logcal device, %", VerbaliseVkResult(res));
		DebugLog("Vulkan : created new logical device");
	}

	void Device::PickPhysicalDevice()
	{
		std::uint32_t device_count = 0;
		vkEnumeratePhysicalDevices(RenderCore::Get().GetInstance().Get(), &device_count, nullptr);

		if(device_count == 0)
			FatalError("Vulkan : failed to find GPUs with Vulkan support");

		std::vector<VkPhysicalDevice> devices(device_count);
		vkEnumeratePhysicalDevices(RenderCore::Get().GetInstance().Get(), &device_count, devices.data());

		std::multimap<int, VkPhysicalDevice> devices_score;

		for(const auto& device : devices)
		{
			int score = DeviceScore(device);
			devices_score.insert(std::make_pair(score, device));
		}

		if(devices_score.rbegin()->first > 0)
			m_physical_device = devices_score.rbegin()->second;
		else
			FatalError("Vulkan : failed to find a suitable GPU");

		#ifdef DEBUG
			VkPhysicalDeviceProperties props;
			vkGetPhysicalDeviceProperties(m_physical_device, &props);
			DebugLog("Vulkan : picked a physical device, %s", props.deviceName);
		#endif
		RenderCore::Get().GetQueue().FindQueueFamilies(m_physical_device); // update queue indicies to current physical device
	}

	int Device::DeviceScore(VkPhysicalDevice device)
	{
		Queues::QueueFamilyIndices indices = RenderCore::Get().GetQueue().FindQueueFamilies(device);
		bool extensions_supported = CheckDeviceExtensionSupport(device);

		VkPhysicalDeviceProperties props;
		vkGetPhysicalDeviceProperties(device, &props);
		if(!indices.IsComplete() || !extensions_supported)
			return -1;

		VkPhysicalDeviceFeatures features;
		vkGetPhysicalDeviceFeatures(device, &features);

		int score = 0;
		#ifndef FORCE_INTEGRATED_GPU
			if(props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
				score += 1000;
		#else
			if(props.deviceType != VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
				return -1;
		#endif

		if(!features.geometryShader)
			return -1;

		score += props.limits.maxImageDimension2D;
		score += props.limits.maxBoundDescriptorSets;
		return score;
	}

	bool Device::CheckDeviceExtensionSupport(VkPhysicalDevice device)
	{
		std::uint32_t extension_count;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, nullptr);

		std::vector<VkExtensionProperties> available_extensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, available_extensions.data());

		std::set<std::string> required_extensions(device_extensions.begin(), device_extensions.end());

		for(const auto& extension : available_extensions)
			required_extensions.erase(extension.extensionName);

		return required_extensions.empty();
	}

	void Device::Destroy() noexcept
	{
		vkDestroyDevice(m_device, nullptr);
		m_device = VK_NULL_HANDLE;
		DebugLog("Vulkan : destroyed a logical device");
	}
}
