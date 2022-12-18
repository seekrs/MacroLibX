/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_device.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 19:14:29 by maldavid          #+#    #+#             */
/*   Updated: 2022/12/18 01:10:42 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render_core.h"
#include <vector>
#include <set>

namespace mlx
{
	const std::vector<const char*> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

	void Device::init()
	{
		pickPhysicalDevice();

		Queues::QueueFamilyIndices indices = Render_Core::get().getQueue().findQueueFamilies(_physicalDevice);

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

		if(vkCreateDevice(_physicalDevice, &createInfo, nullptr, &_device) != VK_SUCCESS)
			core::error::report(e_kind::fatal_error, "Vulkan : failed to create logcal device");
	}

	void Device::pickPhysicalDevice()
	{
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(Render_Core::get().getInstance().get(), &deviceCount, nullptr);

		if(deviceCount == 0)
			core::error::report(e_kind::fatal_error, "Vulkan : failed to find GPUs with Vulkan support");

		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(Render_Core::get().getInstance().get(), &deviceCount, devices.data());

		for(const auto& device : devices)
		{
			if(isDeviceSuitable(device))
			{
				_physicalDevice = device;
				break;
			}
		}

		if(_physicalDevice == VK_NULL_HANDLE)
			core::error::report(e_kind::fatal_error, "Vulkan : failed to find a suitable GPU");
	}
	bool Device::isDeviceSuitable(VkPhysicalDevice device)
	{
		Queues::QueueFamilyIndices indices = Render_Core::get().getQueue().findQueueFamilies(device);

		bool extensionsSupported = checkDeviceExtensionSupport(device);

		bool swapChainAdequate = false;
		if(extensionsSupported)
		{
			SwapChain::SwapChainSupportDetails swapChainSupport = Render_Core::get().getSwapChain().querySwapChainSupport(device);
			swapChainAdequate = !swapChainSupport.formats.empty();
		}

		return indices.isComplete() && extensionsSupported && swapChainAdequate;
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
	}
}
