/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_validation_layers.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/19 14:05:25 by maldavid          #+#    #+#             */
/*   Updated: 2024/01/03 13:11:27 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vk_validation_layers.h"
#include "render_core.h"

#include <core/errors.h>
#include <iostream>
#include <cstring>
#include <algorithm>

namespace mlx
{
	void ValidationLayers::init()
	{
		if constexpr(!enableValidationLayers)
			return;

		VkDebugUtilsMessengerCreateInfoEXT createInfo{};
		populateDebugMessengerCreateInfo(createInfo);
		VkResult res = createDebugUtilsMessengerEXT(&createInfo, nullptr);
		if(res != VK_SUCCESS)
			core::error::report(e_kind::warning, "Vulkan : failed to set up debug messenger, %s", RCore::verbaliseResultVk(res));
		#ifdef DEBUG
		else
			core::error::report(e_kind::message, "Vulkan : enabled validation layers");
		#endif
	}

	bool ValidationLayers::checkValidationLayerSupport()
	{
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		return std::all_of(validationLayers.begin(), validationLayers.end(), [&](const char* layerName)
		{
			if(!std::any_of(availableLayers.begin(), availableLayers.end(), [=](VkLayerProperties props) { return std::strcmp(layerName, props.layerName) == 0; }))
			{
				core::error::report(e_kind::error, "Vulkan : a validation layer was requested but was not found ('%s')", layerName);
				return false;
			}
			return true;
		});
	}

	void ValidationLayers::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
	{
		createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = ValidationLayers::debugCallback;
	}


	void ValidationLayers::destroy()
	{
		if constexpr(enableValidationLayers)
			destroyDebugUtilsMessengerEXT(nullptr);
	}

	VkResult ValidationLayers::createDebugUtilsMessengerEXT(const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator)
	{
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(Render_Core::get().getInstance().get(), "vkCreateDebugUtilsMessengerEXT");
		return func != nullptr ? func(Render_Core::get().getInstance().get(), pCreateInfo, pAllocator, &_debugMessenger) : VK_ERROR_EXTENSION_NOT_PRESENT;
	}

	VKAPI_ATTR VkBool32 VKAPI_CALL ValidationLayers::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, [[maybe_unused]] VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, [[maybe_unused]] void* pUserData)
	{
		if(messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
		{
			std::cout << '\n';
			core::error::report(e_kind::error, std::string("Vulkan layer error: ") + pCallbackData->pMessage);
		}
		else if(messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
		{
			std::cout << '\n';
			core::error::report(e_kind::warning, std::string("Vulkan layer warning: ") + pCallbackData->pMessage);
		}

		return VK_FALSE;
	}

	void ValidationLayers::destroyDebugUtilsMessengerEXT(const VkAllocationCallbacks* pAllocator)
	{
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(Render_Core::get().getInstance().get(), "vkDestroyDebugUtilsMessengerEXT");
		if(func != nullptr)
			func(Render_Core::get().getInstance().get(), _debugMessenger, pAllocator);
	}

}
