/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Queues.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 19:01:49 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/28 22:05:15 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_VK_QUEUES__
#define __MLX_VK_QUEUES__

namespace mlx
{
	class Queues
	{
		public:
			struct QueueFamilyIndices
			{
				std::optional<std::uint32_t> graphics_family;
				std::optional<std::uint32_t> present_family;

				inline bool IsComplete() { return graphics_family.has_value() && present_family.has_value(); }
			};

		public:
			QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

			void Init();

			inline VkQueue& GetGraphic() noexcept { return _graphics_queue; }
			inline VkQueue& GetPresent() noexcept { return _present_queue; }
			inline QueueFamilyIndices GetFamilies() noexcept
			{
				if(m_families.has_value())
					return *m_families;
				FatalError("Vulkan : cannot get queue families, not init");
				return {}; // just to avoid warnings
			}

		private:
			VkQueue m_graphics_queue;
			VkQueue m_present_queue;
			std::optional<QueueFamilyIndices> m_families;
	};
}

#endif // __MLX_VK_QUEUES__
