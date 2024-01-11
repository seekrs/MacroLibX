/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_device.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 19:14:29 by maldavid          #+#    #+#             */
/*   Updated: 2024/01/10 21:54:17 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render_core.h"
#include <iterator>
#include <vector>
#include <set>
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include <iostream>
#include <algorithm>

namespace mlx
{
	const std::vector<const char*> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

	void Device::init()
	{
		pickPhysicalDevice();

		Queues::QueueFamilyIndices indices = Render_Core::get().getQueue().getFamilies();

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

		float queuePriority = 1.0f;
		for(uint32_t queueFamily : uniqueQueueFamilies)
		{
			VkDeviceQueueCreateInfo queueCreateInfo{};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfos.push_back(queueCreateInfo);
		}

		VkPhysicalDeviceFeatures deviceFeatures{};

		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		createInfo.pQueueCreateInfos = queueCreateInfos.data();

		createInfo.pEnabledFeatures = &deviceFeatures;

		createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
		createInfo.ppEnabledExtensionNames = deviceExtensions.data();
		createInfo.enabledLayerCount = 0;

		VkResult res;
		if((res = vkCreateDevice(_physicalDevice, &createInfo, nullptr, &_device)) != VK_SUCCESS)
			core::error::report(e_kind::fatal_error, "Vulkan : failed to create logcal device, %s", RCore::verbaliseResultVk(res));
		#ifdef DEBUG
			core::error::report(e_kind::message, "Vulkan : created new logical device");
		#endif
	}

	void Device::pickPhysicalDevice()
	{
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(Render_Core::get().getInstance().get(), &deviceCount, nullptr);

		if(deviceCount == 0)
			core::error::report(e_kind::fatal_error, "Vulkan : failed to find GPUs with Vulkan support");

		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(Render_Core::get().getInstance().get(), &deviceCount, devices.data());

		SDL_Window* window = SDL_CreateWindow("", 0, 0, 1, 1, SDL_WINDOW_VULKAN | SDL_WINDOW_HIDDEN);
		if(!window)
			core::error::report(e_kind::fatal_error, "Vulkan : failed to create a window to pick physical device");

		VkSurfaceKHR surface = VK_NULL_HANDLE;
		if(SDL_Vulkan_CreateSurface(window, Render_Core::get().getInstance().get(), &surface) != SDL_TRUE)
			core::error::report(e_kind::fatal_error, "Vulkan : failed to create a surface to pick physical device");

		std::vector<std::pair<int, VkPhysicalDevice>> devices_score;

		std::transform(devices.cbegin(), devices.cend(), std::back_inserter(devices_score), [&](VkPhysicalDevice device)
		{
			return std::make_pair(deviceScore(device, surface), device);
		});

		using device_pair = std::pair<int, VkPhysicalDevice>;
		std::sort(devices_score.begin(), devices_score.end(), [](const device_pair& a, const device_pair& b)
		{
			return a.first > b.first;
		});

		if(devices_score.front().first > 0)
			_physicalDevice = devices_score.front().second;

		if(_physicalDevice == VK_NULL_HANDLE)
			core::error::report(e_kind::fatal_error, "Vulkan : failed to find a suitable GPU");
		#ifdef DEBUG
			VkPhysicalDeviceProperties props;
			vkGetPhysicalDeviceProperties(_physicalDevice, &props);
			core::error::report(e_kind::message, "Vulkan : picked a physical device, %s", props.deviceName);
		#endif
		Render_Core::get().getQueue().findQueueFamilies(_physicalDevice, surface); // update queue indicies to current physical device
		vkDestroySurfaceKHR(Render_Core::get().getInstance().get(), surface, nullptr);
		SDL_DestroyWindow(window);
	}

	int Device::deviceScore(VkPhysicalDevice device, VkSurfaceKHR surface)
	{
		Queues::QueueFamilyIndices indices = Render_Core::get().getQueue().findQueueFamilies(device, surface);
		bool extensionsSupported = checkDeviceExtensionSupport(device);

		uint32_t formatCount = 0;
		if(extensionsSupported)
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

		VkPhysicalDeviceProperties props;
		vkGetPhysicalDeviceProperties(device, &props);
		if(!indices.isComplete() || !extensionsSupported || formatCount == 0)
			return -1;

		int score = 0;
		#ifndef FORCE_INTEGRATED_GPU
			if(props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
				score += 1000;
		#else
			if(props.deviceType != VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
				return -1;
		#endif
		score += props.limits.maxImageDimension2D;
		score += props.limits.maxBoundDescriptorSets;
		return score;
	}

	bool Device::checkDeviceExtensionSupport(VkPhysicalDevice device)
	{
		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

		std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

		for(const auto& extension : availableExtensions)
			requiredExtensions.erase(extension.extensionName);

		return requiredExtensions.empty();
	}

	void Device::destroy() noexcept
	{
		vkDestroyDevice(_device, nullptr);
		_device = VK_NULL_HANDLE;
		#ifdef DEBUG
			core::error::report(e_kind::message, "Vulkan : destroyed a logical device");
		#endif
	}
}
