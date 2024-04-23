/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Instance.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 19:04:21 by maldavid          #+#    #+#             */
/*   Updated: 2024/04/23 18:43:47 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <PreCompiled.h>

#include <Renderer/Core/Instance.h>
#include <Renderer/Core/RenderCore.h>

namespace mlx
{
	void Instance::Init()
	{
		std::uint32_t api_version = std::min(volkGetInstanceVersion(), MLX_TARGET_VULKAN_API_VERSION);

		if(api_version == 0)
			FatalError("Vulkan API is not supported by this driver");

		m_instance_version = api_version;

		VkApplicationInfo app_info{};
		app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		app_info.pEngineName = "MacroLibX";
		app_info.engineVersion = MLX_VERSION;
		app_info.apiVersion = api_version;

		auto extensions = GetRequiredExtensions();

		VkInstanceCreateInfo create_info{};
		create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		create_info.pApplicationInfo = &app_info;
		create_info.enabledExtensionCount = static_cast<std::uint32_t>(extensions.size());
		create_info.ppEnabledExtensionNames = extensions.data();
		create_info.enabledLayerCount = 0; // will be replaced if validation layers are enabled
		create_info.pNext = nullptr;

		VkDebugUtilsMessengerCreateInfoEXT debug_create_info;
		if constexpr(enable_validation_layers)
		{
			if(RenderCore::Get().GetLayers().CheckValidationLayerSupport())
			{
				create_info.enabledLayerCount = static_cast<std::uint32_t>(validation_layers.size());
				create_info.ppEnabledLayerNames = validation_layers.data();
				RenderCore::Get().GetLayers().PopulateDebugMessengerCreateInfo(debug_create_info);
				create_info.pNext = static_cast<VkDebugUtilsMessengerCreateInfoEXT*>(&debug_create_info);
			}
		}

		VkResult res;
		if((res = vkCreateInstance(&create_info, nullptr, &m_instance)) != VK_SUCCESS)
			FatalError("Vulkan : failed to create Vulkan instance, %", VerbaliseVkResult(res));
		volkLoadInstance(m_instance);
		DebugLog("Vulkan : created new instance");
	}

	std::vector<const char*> Instance::GetRequiredExtensions()
	{
		std::uint32_t glfw_extension_count = 0;
		const char** glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

		std::vector<const char*> extensions(glfw_extensions, glfw_extensions + glfw_extension_count);

		extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);

		if constexpr(enableValidationLayers)
		{
			extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		return extensions;
	}

	void Instance::Destroy() noexcept
	{
		vkDestroyInstance(m_instance, nullptr);
		m_instance = VK_NULL_HANDLE;
		DebugLog("Vulkan : destroyed an instance");
	}
}
