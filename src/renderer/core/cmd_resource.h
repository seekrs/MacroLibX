/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_resource.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/16 20:44:29 by maldavid          #+#    #+#             */
/*   Updated: 2024/01/11 05:12:42 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_COMMAND_RESOURCE__
#define __MLX_COMMAND_RESOURCE__

#include <function.h>
#include <core/UUID.h>

namespace mlx
{
	class CmdResource
	{
		friend class SingleTimeCmdManager;
		public:
			enum class state
			{
				in_cmd_buffer = 0,
				out_cmd_buffer,
			};

		public:
			CmdResource() : _uuid() {}
			inline void recordedInCmdBuffer() noexcept { _state = state::in_cmd_buffer; }
			inline void removedFromCmdBuffer() noexcept
			{
				_state = state::out_cmd_buffer;
				if(_destroy_required && _destroyer)
				{
					_destroyer();
					_destroy_required = false;
				}
			}
			inline void setDestroyer(func::function<void(void)> functor) { _destroyer = functor; }
			inline void requireDestroy() noexcept
			{
				if(_state == state::out_cmd_buffer && _destroyer)
					_destroyer();
				else
					_destroy_required = true;
			}
			inline UUID getUUID() const noexcept { return _uuid; }
			virtual ~CmdResource() = default;

		private:
			UUID _uuid;
			state _state = state::out_cmd_buffer;
			func::function<void(void)> _destroyer;
			bool _destroy_required = false;
	};
}

#endif
