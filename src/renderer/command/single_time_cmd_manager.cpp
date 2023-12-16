/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_time_cmd_manager.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 19:57:49 by maldavid          #+#    #+#             */
/*   Updated: 2023/12/16 18:46:26 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <algorithm>
#include <renderer/command/single_time_cmd_manager.h>
#include <renderer/core/render_core.h>

namespace mlx
{
	SingleTimeCmdManager::SingleTimeCmdManager() : _buffers(MIN_POOL_SIZE) {}

	void SingleTimeCmdManager::init() noexcept
	{
		_pool.init();
		for(int i = 0; i < MIN_POOL_SIZE; i++)
		{
			_buffers.emplace_back();
			_buffers.back().init(&_pool);
		}
	}

	CmdBuffer& SingleTimeCmdManager::getCmdBuffer() noexcept
	{
		for(CmdBuffer& buf : _buffers)
		{
			if(buf.isReadyToBeUsed())
			{
				buf.reset();
				return buf;
			}
		}
		_buffers.emplace_back().init(&_pool);
		return _buffers.back();
	}

	void SingleTimeCmdManager::destroy() noexcept
	{
		std::for_each(_buffers.begin(), _buffers.end(), [](CmdBuffer& buf)
		{
			buf.destroy();
		});
		_pool.destroy();
	}
}
