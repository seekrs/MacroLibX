/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandManager.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 17:50:52 by maldavid          #+#    #+#             */
/*   Updated: 2024/04/23 14:55:04 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <PreCompiled.h>

#include <Renderer/Command/CommandManager.h>

namespace mlx
{
	void CommandManager::Init() noexcept
	{
		m_cmd_pool.Init();
		for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
			m_cmd_buffers[i].Init(CommandBufferType::LongTime, this);
	}

	void CommandManager::BeginRecord(int active_image_index)
	{
		m_cmd_buffers[active_image_index].BeginRecord();
	}

	void CommandManager::EndRecord(int active_image_index)
	{
		m_cmd_buffers[active_image_index].EndRecord();
	}

	void CommandManager::Destroy() noexcept
	{
		for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
			m_cmd_buffers[i].Destroy();
		m_cmd_pool.Destroy();
	}
}
