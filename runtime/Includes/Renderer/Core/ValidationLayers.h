/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ValidationLayers.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/19 14:04:25 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/27 22:59:00 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __VK_VALIDATION_LAYERS__
#define __VK_VALIDATION_LAYERS__

namespace mlx
{
	class ValidationLayers
	{
		public:
			ValidationLayers() = default;
			
			void Init();
			void Destroy();
			
			bool CheckValidationLayerSupport();
			void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& create_info);
			
			VkResult SetDebugUtilsObjectNameEXT(VkObjectType object_type, std::uint64_t object_handle, const char* object_name);

			~ValidationLayers() = default;

		private:
			VkResult CreateDebugUtilsMessengerEXT(const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator);
			static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity, VkDebugUtilsMessageTypeFlagsEXT message_type, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
			void DestroyDebugUtilsMessengerEXT(const VkAllocationCallbacks* pAllocator);

		private:
			VkDebugUtilsMessengerEXT m_debug_messenger;
			PFN_vkSetDebugUtilsObjectNameEXT f_vkSetDebugUtilsObjectNameEXT = nullptr;
	};
}

#endif
