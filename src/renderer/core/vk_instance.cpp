/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_instance.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 19:04:21 by maldavid          #+#    #+#             */
/*   Updated: 2022/12/18 00:34:58 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vk_instance.h"
#include "render_core.h"
#include <platform/window.h>
#include <SDL2/SDL_vulkan.h>

namespace mlx
{
	namespace RCore { const char* verbaliseResultVk(VkResult result); }

	void Instance::init()
	{
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "MicroLibX";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "MicroLibX";
        appInfo.engineVersion = VK_MAKE_VERSION(0, 0, 1);
        appInfo.apiVersion = VK_API_VERSION_1_2;

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        auto extensions = getRequiredExtensions();
        createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();

        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
		createInfo.enabledLayerCount = 0;
		createInfo.pNext = nullptr;

		VkResult res;
        if((res = vkCreateInstance(&createInfo, nullptr, &_instance)) != VK_SUCCESS)
			core::error::report(e_kind::fatal_error, "Vulkan : failed to create Vulkan instance : %s", RCore::verbaliseResultVk(res));
	}

	std::vector<const char*> Instance::getRequiredExtensions()
    {
        unsigned int count = 0;
        if(!SDL_Vulkan_GetInstanceExtensions(Render_Core::get().getWindow()->getNativeWindow(), &count, nullptr))
			core::error::report(e_kind::fatal_error, "Vulkan : cannot get instance extentions from window : %s",  SDL_GetError());

        std::vector<const char*> extensions = { VK_EXT_DEBUG_REPORT_EXTENSION_NAME };
        size_t additional_extension_count = extensions.size();
        extensions.resize(additional_extension_count + count);

        if(!SDL_Vulkan_GetInstanceExtensions(Render_Core::get().getWindow()->getNativeWindow(), &count, extensions.data() + additional_extension_count))
			core::error::report(e_kind::error, "Vulkan : cannot get instance extentions from window : %s", SDL_GetError());
        return extensions;
    }

    void Instance::destroy() noexcept
    {
        vkDestroyInstance(_instance, nullptr);
    }
}
