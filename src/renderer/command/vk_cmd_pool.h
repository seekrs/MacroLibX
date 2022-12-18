/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_cmd_pool.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 18:24:12 by maldavid          #+#    #+#             */
/*   Updated: 2022/12/18 01:08:31 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_VK_CMD_POOL__
#define __MLX_VK_CMD_POOL__

#include <volk.h>

namespace mlx
{
	class CmdPool
	{
		public:
			void init();
			void destroy() noexcept;

			inline VkCommandPool& operator()() noexcept { return _cmd_pool; }
			inline VkCommandPool& get() noexcept { return _cmd_pool; }

		private:
			VkCommandPool _cmd_pool = VK_NULL_HANDLE;
	};
}

#endif // __MLX_VK_CMD_POOL__
