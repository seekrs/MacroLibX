/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_instance.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 19:04:21 by maldavid          #+#    #+#             */
/*   Updated: 2024/01/10 21:54:06 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vk_instance.h"
#include "render_core.h"
#include <platform/window.h>
#include <SDL2/SDL_vulkan.h>

namespace mlx
{
	void Instance::init()
	{
		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pEngineName = "MacroLibX";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 2, 1);
		appInfo.apiVersion = VK_API_VERSION_1_2;

		auto extensions = getRequiredExtensions();

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;
		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();
		createInfo.enabledLayerCount = 0; // will be replaced if validation layers are enabled
		createInfo.pNext = nullptr;

		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
		if constexpr(enableValidationLayers)
		{
			if(Render_Core::get().getLayers().checkValidationLayerSupport())
			{
				createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
				createInfo.ppEnabledLayerNames = validationLayers.data();
				Render_Core::get().getLayers().populateDebugMessengerCreateInfo(debugCreateInfo);
				createInfo.pNext = static_cast<VkDebugUtilsMessengerCreateInfoEXT*>(&debugCreateInfo);
			}
		}

		VkResult res;
		if((res = vkCreateInstance(&createInfo, nullptr, &_instance)) != VK_SUCCESS)
			core::error::report(e_kind::fatal_error, "Vulkan : failed to create Vulkan instance, %s", RCore::verbaliseResultVk(res));
		volkLoadInstance(_instance);
		#ifdef DEBUG
			core::error::report(e_kind::message, "Vulkan : created new instance");
		#endif
	}

	std::vector<const char*> Instance::getRequiredExtensions()
	{
		unsigned int count = 0;
		SDL_Window* window = SDL_CreateWindow("", 0, 0, 1, 1, SDL_WINDOW_VULKAN | SDL_WINDOW_HIDDEN);
		if(!window)
			core::error::report(e_kind::fatal_error, "Vulkan : cannot get instance extentions from window : %s",  SDL_GetError());

		if(!SDL_Vulkan_GetInstanceExtensions(window, &count, nullptr))
			core::error::report(e_kind::fatal_error, "Vulkan : cannot get instance extentions from window : %s",  SDL_GetError());

		std::vector<const char*> extensions = { VK_EXT_DEBUG_REPORT_EXTENSION_NAME };
		size_t additional_extension_count = extensions.size();
		extensions.resize(additional_extension_count + count);

		if(!SDL_Vulkan_GetInstanceExtensions(window, &count, extensions.data() + additional_extension_count))
			core::error::report(e_kind::error, "Vulkan : cannot get instance extentions from window : %s", SDL_GetError());

		if constexpr(enableValidationLayers)
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

		SDL_DestroyWindow(window);
		return extensions;
	}

	void Instance::destroy() noexcept
	{
		vkDestroyInstance(_instance, nullptr);
		_instance = VK_NULL_HANDLE;
		#ifdef DEBUG
			core::error::report(e_kind::message, "Vulkan : destroyed an instance");
		#endif
	}
}
