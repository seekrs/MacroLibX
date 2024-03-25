/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_validation_layers.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/19 14:05:25 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/14 17:03:24 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render_core.h"
#include "vulkan/vulkan_core.h"

#include <core/errors.h>
#include <cstring>
#include <algorithm>

namespace mlx
{
	void ValidationLayers::init()
	{
		if constexpr(!enableValidationLayers)
			return;

		std::uint32_t extensionCount;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
		std::vector<VkExtensionProperties> extensions(extensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
		if(!std::any_of(extensions.begin(), extensions.end(), [=](VkExtensionProperties ext) { return std::strcmp(ext.extensionName, VK_EXT_DEBUG_UTILS_EXTENSION_NAME) == 0; }))
		{
			core::error::report(e_kind::warning , "Vulkan : %s not present, debug utils are disabled", VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
			return;
		}

		VkDebugUtilsMessengerCreateInfoEXT createInfo{};
		populateDebugMessengerCreateInfo(createInfo);
		VkResult res = createDebugUtilsMessengerEXT(&createInfo, nullptr);
		if(res != VK_SUCCESS)
			core::error::report(e_kind::warning, "Vulkan : failed to set up debug messenger, %s", RCore::verbaliseResultVk(res));
		#ifdef DEBUG
		else
			core::error::report(e_kind::message, "Vulkan : enabled validation layers");
		#endif

		_vkSetDebugUtilsObjectNameEXT = (PFN_vkSetDebugUtilsObjectNameEXT)vkGetInstanceProcAddr(Render_Core::get().getInstance().get(), "vkSetDebugUtilsObjectNameEXT");
		if(!_vkSetDebugUtilsObjectNameEXT)
			core::error::report(e_kind::warning, "Vulkan : failed to set up debug object names, %s", RCore::verbaliseResultVk(VK_ERROR_EXTENSION_NOT_PRESENT));
		#ifdef DEBUG
		else
			core::error::report(e_kind::message, "Vulkan : enabled debug object names");
		#endif
	}

	bool ValidationLayers::checkValidationLayerSupport()
	{
		std::uint32_t layerCount;
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

	VkResult ValidationLayers::setDebugUtilsObjectNameEXT(VkObjectType object_type, std::uint64_t object_handle, const char* object_name)
	{
		if(!_vkSetDebugUtilsObjectNameEXT)
			return VK_ERROR_EXTENSION_NOT_PRESENT;

		VkDebugUtilsObjectNameInfoEXT name_info{};
		name_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
		name_info.objectType = object_type;
		name_info.objectHandle = object_handle;
		name_info.pObjectName = object_name;
		return _vkSetDebugUtilsObjectNameEXT(Render_Core::get().getDevice().get(), &name_info);
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
		{
			destroyDebugUtilsMessengerEXT(nullptr);
			#ifdef DEBUG
				core::error::report(e_kind::message, "Vulkan : destroyed validation layers");
			#endif
		}
	}

	VkResult ValidationLayers::createDebugUtilsMessengerEXT(const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator)
	{
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(Render_Core::get().getInstance().get(), "vkCreateDebugUtilsMessengerEXT");
		return func != nullptr ? func(Render_Core::get().getInstance().get(), pCreateInfo, pAllocator, &_debug_messenger) : VK_ERROR_EXTENSION_NOT_PRESENT;
	}

	VKAPI_ATTR VkBool32 VKAPI_CALL ValidationLayers::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, [[maybe_unused]] VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, [[maybe_unused]] void* pUserData)
	{
		if(messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
			core::error::report(e_kind::error, pCallbackData->pMessage);
		else if(messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
			core::error::report(e_kind::warning, pCallbackData->pMessage);
		return VK_FALSE;
	}

	void ValidationLayers::destroyDebugUtilsMessengerEXT(const VkAllocationCallbacks* pAllocator)
	{
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(Render_Core::get().getInstance().get(), "vkDestroyDebugUtilsMessengerEXT");
		if(func != nullptr)
			func(Render_Core::get().getInstance().get(), _debug_messenger, pAllocator);
	}

}
