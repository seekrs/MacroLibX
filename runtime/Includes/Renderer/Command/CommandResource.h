/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandResource.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/16 20:44:29 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/27 22:37:06 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_COMMAND_RESOURCE__
#define __MLX_COMMAND_RESOURCE__

#include <Renderer/Enums.h>
#include <Core/UUID.h>

namespace mlx
{
	class CommandResource
	{
		friend class SingleTimeCmdManager;

		public:
			CommandResource() : m_uuid() {}
			inline void RecordedInCmdBuffer() noexcept { m_state = CommandResourceState::Held; }
			inline void RemovedFromCmdBuffer() noexcept { m_state = CommandResourceState::Free; }
			inline UUID GetUUID() const noexcept { return m_uuid; }
			virtual ~CommandResource() = default;

		private:
			UUID m_uuid;
			CommandResourceState m_state = CommandResourceState::Free;
	};
}

#endif
