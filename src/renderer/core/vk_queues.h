/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_queues.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 19:01:49 by maldavid          #+#    #+#             */
/*   Updated: 2023/12/08 19:08:25 by kbz_8            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_VK_QUEUES__
#define __MLX_VK_QUEUES__

#include <volk.h>
#include <optional>
#include <cstdint>
#include <mlx_profile.h>

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
			inline QueueFamilyIndices getFamilies() noexcept { return *_families; }

		private:
			VkQueue _graphicsQueue;
			VkQueue _presentQueue;
			std::optional<QueueFamilyIndices> _families;
	};
}

#endif // __MLX_VK_QUEUES__
