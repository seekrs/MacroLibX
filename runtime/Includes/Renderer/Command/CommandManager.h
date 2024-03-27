/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandManager.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 17:48:52 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/27 22:20:53 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_COMMAND_MANAGER__
#define __MLX_COMMAND_MANAGER__

#include <Renderer/Core/RenderCore.h>
#include <Renderer/Command/CommandPool.h>
#include <Renderer/Command/CommandBuffer.h>

namespace mlx
{
	class CommandManager
	{
		public:
			CommandManager() = default;

			void Init() noexcept;
			void BeginRecord(int active_image_index);
			void EndRecord(int active_image_index);
			void Destroy() noexcept;

			inline CommandPool& GetCmdPool() noexcept { return m_cmd_pool; }
			inline CommandBuffer& GetCmdBuffer(int i) noexcept { return m_cmd_buffers[i]; }

			~CommandManager() = default;

		private:
			std::array<CommandBuffer, MAX_FRAMES_IN_FLIGHT> m_cmd_buffers;
			CommandPool m_cmd_pool;
	};
}

#endif
