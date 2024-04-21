/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_validation_layers.h                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/19 14:04:25 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/14 17:02:55 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __VK_VALIDATION_LAYERS__
#define __VK_VALIDATION_LAYERS__

#include <mlx_profile.h>
#include <volk.h>

namespace mlx
{
	class ValidationLayers
	{
		public:
			ValidationLayers() = default;
			
			void init();
			void destroy();
			
			bool checkValidationLayerSupport();
			void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
			
			VkResult setDebugUtilsObjectNameEXT(VkObjectType object_type, std::uint64_t object_handle, const char* object_name);

			~ValidationLayers() = default;

		private:
			VkResult createDebugUtilsMessengerEXT(const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator);
			static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
			void destroyDebugUtilsMessengerEXT(const VkAllocationCallbacks* pAllocator);

		private:
			VkDebugUtilsMessengerEXT _debug_messenger;
			PFN_vkSetDebugUtilsObjectNameEXT _vkSetDebugUtilsObjectNameEXT = nullptr;
	};
}

#endif
