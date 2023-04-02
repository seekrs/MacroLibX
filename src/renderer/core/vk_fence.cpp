/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_fence.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 17:53:06 by maldavid          #+#    #+#             */
/*   Updated: 2023/04/02 17:54:14 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <renderer/core/vk_fence.h>

namespace mlx
{
	void Fence::init()
	{
		VkSemaphoreCreateInfo semaphoreInfo{};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo{};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		if(vkCreateFence(Render_Core::get().getDevice().get(), &fenceInfo, nullptr, &_fence) != VK_SUCCESS)
			core::error::report(e_kind::fatal_error, "Vulkan : failed to create CPU synchronization object");
	}

	void Fence::wait() noexcept
	{
		vkWaitForFences(Render_Core::get().getDevice().get(), 1, &_fence, VK_TRUE, UINT64_MAX);
	}

	void Fence::reset() noexcept
	{
		vkResetFences(Render_Core::get().getDevice().get(), 1, &_fence);
	}

	void Fence::destroy() noexcept
	{
		vkDestroyFence(Render_Core::get().getDevice().get(), _fence, nullptr);
	}
}
