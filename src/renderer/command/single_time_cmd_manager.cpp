/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_time_cmd_manager.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 19:57:49 by maldavid          #+#    #+#             */
/*   Updated: 2024/01/05 20:29:01 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <algorithm>
#include <renderer/command/single_time_cmd_manager.h>
#include <renderer/command/vk_cmd_buffer.h>
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
			_buffers.back().init(CmdBuffer::kind::single_time, &_pool);
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
		_buffers.emplace_back().init(CmdBuffer::kind::single_time, &_pool);
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

	SingleTimeCmdManager::~SingleTimeCmdManager() {}
}
