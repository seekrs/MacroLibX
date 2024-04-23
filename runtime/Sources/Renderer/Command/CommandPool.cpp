/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandPool.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 18:24:33 by maldavid          #+#    #+#             */
/*   Updated: 2024/04/23 14:57:15 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <PreCompiled.h>

#include <Renderer/Command/CommandPool.h>
#include <Renderer/Core/RenderCore.h>

namespace mlx
{
	void CommandPool::Init()
	{
		VkCommandPoolCreateInfo pool_info{};
		pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		pool_info.queueFamilyIndex = RenderCore::Get().GetQueue().GetFamilies().graphics_family.value();

		VkResult res = vkCreateCommandPool(RenderCore::Get().GetDevice().Get(), &pool_info, nullptr, &m_cmd_pool);
		if(res != VK_SUCCESS)
			FatalError("Vulkan : failed to create command pool, %", VerbaliseVkResult(res));
	}

	void CommandPool::Destroy() noexcept
	{
		vkDestroyCommandPool(RenderCore::Get().GetDevice().Get(), m_cmd_pool, nullptr);
		m_cmd_pool = VK_NULL_HANDLE;
	}
}
