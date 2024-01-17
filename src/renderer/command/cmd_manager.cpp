/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_manager.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 17:50:52 by maldavid          #+#    #+#             */
/*   Updated: 2023/12/17 20:10:45 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <renderer/command/cmd_manager.h>

namespace mlx
{
	void CmdManager::init() noexcept
	{
		_cmd_pool.init();
		for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
			_cmd_buffers[i].init(CmdBuffer::kind::long_time, this);
	}

	void CmdManager::beginRecord(int active_image_index)
	{
		_cmd_buffers[active_image_index].beginRecord();
	}

	void CmdManager::endRecord(int active_image_index)
	{
		_cmd_buffers[active_image_index].endRecord();
	}

	void CmdManager::destroy() noexcept
	{
		for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
			_cmd_buffers[i].destroy();
		_cmd_pool.destroy();
	}
}
