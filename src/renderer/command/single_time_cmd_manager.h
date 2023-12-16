/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_time_cmd_manager.h                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 18:25:57 by maldavid          #+#    #+#             */
/*   Updated: 2023/12/16 18:09:56 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_SINGLE_TIME_CMD_MANAGER__
#define __MLX_SINGLE_TIME_CMD_MANAGER__

#include <vector>

#include <renderer/command/vk_cmd_pool.h>
#include <renderer/command/vk_cmd_buffer.h>

namespace mlx
{
	class SingleTimeCmdManager
	{
		public:
			SingleTimeCmdManager();

			void init() noexcept;
			void destroy() noexcept;

			inline CmdPool& getCmdPool() noexcept { return _pool; }
			CmdBuffer& getCmdBuffer() noexcept;

			~SingleTimeCmdManager() = default;

			inline static constexpr const uint8_t MIN_POOL_SIZE = 8;

		private:

		private:
			std::vector<CmdBuffer> _buffers;
			CmdPool _pool;
	};
}

#endif
