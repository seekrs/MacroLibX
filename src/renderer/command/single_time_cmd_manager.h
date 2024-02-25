/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_time_cmd_manager.h                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 18:25:57 by maldavid          #+#    #+#             */
/*   Updated: 2024/01/07 01:30:19 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_SINGLE_TIME_CMD_MANAGER__
#define __MLX_SINGLE_TIME_CMD_MANAGER__

#include <vector>
#include <renderer/command/vk_cmd_buffer.h>
#include <renderer/command/vk_cmd_pool.h>

namespace mlx
{
	class CmdBuffer;

	class SingleTimeCmdManager
	{
		friend class Render_Core;

		public:
			SingleTimeCmdManager() = default;

			void init() noexcept;
			void destroy() noexcept;

			void updateSingleTimesCmdBuffersSubmitState() noexcept;
			void waitForAllExecutions() noexcept;

			inline CmdPool& getCmdPool() noexcept { return _pool; }
			CmdBuffer& getCmdBuffer() noexcept;

			~SingleTimeCmdManager() = default;

			inline static constexpr const std::uint8_t BASE_POOL_SIZE = 16;

		private:
			std::vector<CmdBuffer> _buffers;
			CmdPool _pool;
	};
}

#endif
