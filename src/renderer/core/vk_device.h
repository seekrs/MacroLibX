/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_device.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 19:13:42 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/14 16:59:54 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_VK_DEVICE__
#define __MLX_VK_DEVICE__

#include <mlx_profile.h>
#include <volk.h>

namespace mlx
{
	class Device
	{
		public:
			void init();
			void destroy() noexcept;

			inline VkDevice& operator()() noexcept { return _device; }
			inline VkDevice& get() noexcept { return _device; }

			inline VkPhysicalDevice& getPhysicalDevice() noexcept { return _physical_device; }

		private:
			void pickPhysicalDevice();
			bool checkDeviceExtensionSupport(VkPhysicalDevice device);
			int deviceScore(VkPhysicalDevice device, VkSurfaceKHR surface);

		private:
			VkPhysicalDevice _physical_device = VK_NULL_HANDLE;
			VkDevice _device = VK_NULL_HANDLE;
	};
}

#endif // __MLX_VK_DEVICE__
