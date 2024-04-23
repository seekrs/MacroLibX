/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ValidationLayers.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/19 14:05:25 by maldavid          #+#    #+#             */
/*   Updated: 2024/04/23 19:20:21 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <PreCompiled.h>
#include <Renderer/Core/RenderCore.h>

namespace mlx
{
	void ValidationLayers::Init()
	{
		if constexpr(!enable_validation_layers)
			return;

		std::uint32_t extension_count;
		vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);
		std::vector<VkExtensionProperties> extensions(extension_count);
		vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extensions.data());
		if(!std::any_of(extensions.begin(), extensions.end(), [=](VkExtensionProperties ext) { return std::strcmp(ext.extensionName, VK_EXT_DEBUG_UTILS_EXTENSION_NAME) == 0; }))
		{
			Warning("Vulkan : %s not present, debug utils are disabled", VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
			return;
		}

		VkDebugUtilsMessengerCreateInfoEXT create_info{};
		populateDebugMessengerCreateInfo(create_info);
		VkResult res = createDebugUtilsMessengerEXT(&create_info, nullptr);
		if(res != VK_SUCCESS)
			Warning("Vulkan : failed to set up debug messenger, %", VerbaliseVkResult(res));
		else
			DebugLog("Vulkan : enabled validation layers");

		f_vkSetDebugUtilsObjectNameEXT = (PFN_vkSetDebugUtilsObjectNameEXT)vkGetInstanceProcAddr(RenderCore::Get().GetInstance().Get(), "vkSetDebugUtilsObjectNameEXT");
		if(!f_vkSetDebugUtilsObjectNameEXT)
			Warning("Vulkan : failed to set up debug object names, %", VerbaliseVkResult(VK_ERROR_EXTENSION_NOT_PRESENT));
		else
			DebugLog("Vulkan : enabled debug object names");
	}

	bool ValidationLayers::CheckValidationLayerSupport()
	{
		std::uint32_t layer_count;
		vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

		std::vector<VkLayerProperties> available_layers(layer_count);
		vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

		return std::all_of(validation_layers.begin(), validation_layers.end(), [&](const char* layer_name)
		{
			if(!std::any_of(available_layers.begin(), available_layers.end(), [=](VkLayerProperties props) { return std::strcmp(layer_name, props.layer_name) == 0; }))
			{
				Error("Vulkan : a validation layer was requested but was not found ('%')", layer_name);
				return false;
			}
			return true;
		});
	}

	VkResult ValidationLayers::SetDebugUtilsObjectNameEXT(VkObjectType object_type, std::uint64_t object_handle, const char* object_name)
	{
		if(!f_vkSetDebugUtilsObjectNameEXT)
			return VK_ERROR_EXTENSION_NOT_PRESENT;

		VkDebugUtilsObjectNameInfoEXT name_info{};
		name_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
		name_info.objectType = object_type;
		name_info.objectHandle = object_handle;
		name_info.pObjectName = object_name;
		return f_vkSetDebugUtilsObjectNameEXT(RenderCore::Get().GetDevice().Get(), &name_info);
	}

	void ValidationLayers::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& create_info)
	{
		create_info = {};
		create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		create_info.pfnUserCallback = ValidationLayers::DebugCallback;
	}

	void ValidationLayers::Destroy()
	{
		if constexpr(enable_validation_layers)
		{
			DestroyDebugUtilsMessengerEXT(nullptr);
			#ifdef DEBUG
				DebugLog("Vulkan : destroyed validation layers");
			#endif
		}
	}

	VkResult ValidationLayers::CreateDebugUtilsMessengerEXT(const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator)
	{
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(RenderCore::Get().GetInstance().Get(), "vkCreateDebugUtilsMessengerEXT");
		return func != nullptr ? func(RenderCore::Get().GetInstance().Get(), pCreateInfo, pAllocator, &m_debug_messenger) : VK_ERROR_EXTENSION_NOT_PRESENT;
	}

	VKAPI_ATTR VkBool32 VKAPI_CALL ValidationLayers::DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, [[maybe_unused]] VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, [[maybe_unused]] void* pUserData)
	{
		if(messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
			Error(pCallbackData->pMessage);
		else if(messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
			Warning(pCallbackData->pMessage);
		return VK_FALSE;
	}

	void ValidationLayers::destroyDebugUtilsMessengerEXT(const VkAllocationCallbacks* pAllocator)
	{
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(RenderCore::Get().GetInstance().Get(), "vkDestroyDebugUtilsMessengerEXT");
		if(func != nullptr)
			func(RenderCore::Get().GetInstance().Get(), m_debug_messenger, pAllocator);
	}

}
