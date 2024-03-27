/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Device.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 19:13:42 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/27 22:47:21 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_VK_DEVICE__
#define __MLX_VK_DEVICE__

namespace mlx
{
	class Device
	{
		public:
			void Init();
			void Destroy() noexcept;

			inline VkDevice& operator()() noexcept { return m_device; }
			inline VkDevice& Get() noexcept { return m_device; }

			inline VkPhysicalDevice& GetPhysicalDevice() noexcept { return m_physical_device; }

		private:
			void PickPhysicalDevice();
			bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
			int DeviceScore(VkPhysicalDevice device);

		private:
			VkPhysicalDevice m_physical_device = VK_NULL_HANDLE;
			VkDevice m_device = VK_NULL_HANDLE;
	};
}

#endif // __MLX_VK_DEVICE__
