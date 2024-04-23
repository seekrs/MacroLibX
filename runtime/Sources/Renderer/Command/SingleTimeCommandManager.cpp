/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SingleTimeCommandManager.cpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 19:57:49 by maldavid          #+#    #+#             */
/*   Updated: 2024/04/23 15:05:19 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <PreCompiled.h>

#include <Renderer/Command/SingleTimeCommandManager.h>
#include <Renderer/Core/RenderCore.h>

namespace mlx
{
	void SingleTimeCmdManager::Init() noexcept
	{
		m_pool.init();
		for(int i = 0; i < BASE_POOL_SIZE; i++)
		{
			m_buffers.emplace_back();
			m_buffers.back().Init(CommandBufferType::SingleTime, &m_pool);
		}
	}

	CommandBuffer& SingleTimeCmdManager::GetCmdBuffer() noexcept
	{
		for(CmdBuffer& buf : m_buffers)
		{
			if(buf.IsReadyToBeUsed())
			{
				buf.reset();
				return buf;
			}
		}
		m_buffers.emplace_back().Init(CommandBufferType::SingleTime, &m_pool);
		return m_buffers.back();
	}

	void SingleTimeCmdManager::UpdateSingleTimesCmdBuffersSubmitState() noexcept
	{
		for(CmdBuffer& cmd : m_buffers)
			cmd.UpdateSubmitState();
	}

	void SingleTimeCmdManager::WaitForAllExecutions() noexcept
	{
		for(CmdBuffer& cmd : m_buffers)
			cmd.WaitForExecution();
	}

	void SingleTimeCmdManager::Destroy() noexcept
	{
		std::for_each(m_buffers.begin(), m_buffers.end(), [](CommandBuffer& buf)
		{
			buf.Destroy();
		});
		m_pool.Destroy();
	}
}
