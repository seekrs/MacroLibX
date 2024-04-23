/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Fence.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 17:53:06 by maldavid          #+#    #+#             */
/*   Updated: 2024/04/23 18:13:09 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Precompiled.h>

#include <Renderer/Core/Fence.h>
#include <Renderer/Core/RenderCore.h>

namespace mlx
{
	void Fence::Init()
	{
		VkFenceCreateInfo fence_info{};
		fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		VkResult res;
		if((res = vkCreateFence(RenderCore::Get().GetDevice().Get(), &fence_info, nullptr, &m_fence)) != VK_SUCCESS)
			FatalError("Vulkan : failed to create a synchronization object (fence), %", VerbaliseVkResult(res));
		DebugLog("Vulkan : created new fence");
	}

	void Fence::Wait() noexcept
	{
		vkWaitForFences(RenderCore::Get().GetDevice().Get(), 1, &m_fence, VK_TRUE, UINT64_MAX);
	}

	void Fence::Reset() noexcept
	{
		vkResetFences(RenderCore::Get().GetDevice().Get(), 1, &m_fence);
	}

	bool Fence::IsReady() const noexcept
	{
		return vkGetFenceStatus(RenderCore::Get().GetDevice().Get(), m_fence) == VK_SUCCESS;
	}

	void Fence::destroy() noexcept
	{
		if(m_fence != VK_NULL_HANDLE)
			vkDestroyFence(RenderCore::Get().GetDevice().Get(), m_fence, nullptr);
		m_fence = VK_NULL_HANDLE;
		DebugLog("Vulkan : destroyed fence");
	}
}
