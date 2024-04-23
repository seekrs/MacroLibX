/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Semaphore.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 19:01:08 by maldavid          #+#    #+#             */
/*   Updated: 2024/04/23 18:55:42 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <PreCompiled.h>
#include <Renderer/Core/RenderCore.h>
#include <Renderer/Core/Semaphore.h>

namespace mlx
{
	void Semaphore::Init()
	{
		VkSemaphoreCreateInfo semaphore_info{};
		semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkResult res;
		if((res = vkCreateSemaphore(RenderCore::Get().GetDevice().Get(), &semaphore_info, nullptr, &m_semaphore)) != VK_SUCCESS)
			FatalError("Vulkan : failed to create a synchronization object (semaphore), %", VerbaliseVkResult(res));
		DebugLog("Vulkan : created new semaphores");
	}

	void Semaphore::Destroy() noexcept
	{
		vkDestroySemaphore(RenderCore::Get().GetDevice().Get(), m_semaphore, nullptr);
		m_semaphore = VK_NULL_HANDLE;
		DebugLog("Vulkan : destroyed semaphore");
	}
}
