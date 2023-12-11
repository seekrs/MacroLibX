/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_instance.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 19:03:04 by maldavid          #+#    #+#             */
/*   Updated: 2023/12/08 19:08:14 by kbz_8            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_VK_INSTANCE__
#define __MLX_VK_INSTANCE__

#include <volk.h>
#include <vector>
#include <mlx_profile.h>

namespace mlx
{
	class Instance
	{
		public:
			void init();
			void destroy() noexcept;

			inline VkInstance& operator()() noexcept { return _instance; }
			inline VkInstance& get() noexcept { return _instance; }

		private:
			std::vector<const char*> getRequiredExtensions();
			VkInstance _instance = VK_NULL_HANDLE;
	};
}

#endif // __MLX_VK_INSTANCE__
