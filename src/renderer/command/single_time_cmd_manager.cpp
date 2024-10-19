/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_time_cmd_manager.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 19:57:49 by maldavid          #+#    #+#             */
/*   Updated: 2024/10/19 10:47:04 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <algorithm>
#include <renderer/command/single_time_cmd_manager.h>
#include <renderer/core/render_core.h>

namespace mlx
{
	void SingleTimeCmdManager::init() noexcept
	{
		_pool.init();
		for(int i = 0; i < BASE_POOL_SIZE; i++)
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

	void SingleTimeCmdManager::updateSingleTimesCmdBuffersSubmitState() noexcept
	{
		for(CmdBuffer& cmd : _buffers)
			cmd.updateSubmitState();
	}

	void SingleTimeCmdManager::waitForAllExecutions() noexcept
	{
		for(CmdBuffer& cmd : _buffers)
			cmd.waitForExecution();
	}

	void SingleTimeCmdManager::destroy() noexcept
	{
		std::for_each(_buffers.begin(), _buffers.end(), [](CmdBuffer& buf)
		{
			buf.destroy();
		});
		_pool.destroy();
		_buffers.clear();
	}
}
