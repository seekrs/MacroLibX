/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_time_cmd_manager.h                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 18:25:57 by maldavid          #+#    #+#             */
/*   Updated: 2024/01/05 20:28:41 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_SINGLE_TIME_CMD_MANAGER__
#define __MLX_SINGLE_TIME_CMD_MANAGER__

#include <vector>

#include <renderer/command/vk_cmd_pool.h>

namespace mlx
{
	class CmdBuffer;

	class SingleTimeCmdManager
	{
		public:
			SingleTimeCmdManager();

			void init() noexcept;
			void destroy() noexcept;

			inline CmdPool& getCmdPool() noexcept { return _pool; }
			CmdBuffer& getCmdBuffer() noexcept;

			~SingleTimeCmdManager();

			inline static constexpr const uint8_t MIN_POOL_SIZE = 8;

		private:

		private:
			std::vector<CmdBuffer> _buffers;
			CmdPool _pool;
	};
}

#endif
