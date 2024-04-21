/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_queues.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 19:01:49 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/14 17:00:48 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_VK_QUEUES__
#define __MLX_VK_QUEUES__

#include <mlx_profile.h>
#include <volk.h>
#include <optional>
#include <cstdint>
#include <core/errors.h>

namespace mlx
{
	class Queues
	{
		public:
			struct QueueFamilyIndices
			{
				std::optional<std::uint32_t> graphics_family;
				std::optional<std::uint32_t> present_family;

				inline bool isComplete() { return graphics_family.has_value() && present_family.has_value(); }
			};

			QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);

			void init();

			inline VkQueue& getGraphic() noexcept { return _graphics_queue; }
			inline VkQueue& getPresent() noexcept { return _present_queue; }
			inline QueueFamilyIndices getFamilies() noexcept
			{
				if(_families.has_value())
					return *_families;
				core::error::report(e_kind::fatal_error, "Vulkan : cannot get queue families, not init");
				return {}; // just to avoid warnings
			}

		private:
			VkQueue _graphics_queue;
			VkQueue _present_queue;
			std::optional<QueueFamilyIndices> _families;
	};
}

#endif // __MLX_VK_QUEUES__
