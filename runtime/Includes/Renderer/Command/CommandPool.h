/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandPool.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 18:24:12 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/27 22:33:15 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_VK_CMD_POOL__
#define __MLX_VK_CMD_POOL__

namespace mlx
{
	class CommandPool
	{
		public:
			CommandPool() = default;

			void Init();
			void Destroy() noexcept;

			inline VkCommandPool& operator()() noexcept { return m_cmd_pool; }
			inline VkCommandPool& Get() noexcept { return m_cmd_pool; }

			~CommandPool() = default;

		private:
			VkCommandPool m_cmd_pool = VK_NULL_HANDLE;
	};
}

#endif // __MLX_VK_CMD_POOL__
