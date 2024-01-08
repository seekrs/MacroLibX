/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_queues.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 19:01:49 by maldavid          #+#    #+#             */
/*   Updated: 2024/01/08 23:46:23 by maldavid         ###   ########.fr       */
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
				std::optional<uint32_t> graphicsFamily;
				std::optional<uint32_t> presentFamily;

				inline bool isComplete() { return graphicsFamily.has_value() && presentFamily.has_value(); }
			};

			QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);

			void init();

			inline VkQueue& getGraphic() noexcept { return _graphicsQueue; }
			inline VkQueue& getPresent() noexcept { return _presentQueue; }
			inline QueueFamilyIndices getFamilies() noexcept
			{
				if(_families.has_value())
					return *_families;
				core::error::report(e_kind::fatal_error, "Vulkan : cannot get queue families, not init");
				return {}; // just to avoid warnings
			}

		private:
			VkQueue _graphicsQueue;
			VkQueue _presentQueue;
			std::optional<QueueFamilyIndices> _families;
	};
}

#endif // __MLX_VK_QUEUES__
