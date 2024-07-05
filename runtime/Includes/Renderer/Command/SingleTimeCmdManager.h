/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SingleTimeCmdManager.h                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 18:25:57 by maldavid          #+#    #+#             */
/*   Updated: 2024/07/05 13:38:11 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_SINGLE_TIME_CMD_MANAGER__
#define __MLX_SINGLE_TIME_CMD_MANAGER__

#include <Renderer/Command/CommandBuffer.h>
#include <Renderer/Command/CommandPool.h>

namespace mlx
{
	class CommandBuffer;

	class SingleTimeCmdManager
	{
		friend class RenderCore;

		public:
			SingleTimeCmdManager() = default;

			void Init() noexcept;
			void Destroy() noexcept;

			void UpdateSingleTimesCmdBuffersSubmitState() noexcept;
			void WaitForAllExecutions() noexcept;

			inline CommandPool& GetCmdPool() noexcept { return m_pool; }
			CommandBuffer& GetCmdBuffer() noexcept;

			~SingleTimeCmdManager() = default;

			inline static constexpr const std::uint8_t BASE_POOL_SIZE = 16;

		private:
			std::vector<CommandBuffer> m_buffers;
			CommandPool m_pool;
	};
}

#endif
