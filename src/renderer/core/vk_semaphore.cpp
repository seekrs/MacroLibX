/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_semaphore.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 19:01:08 by maldavid          #+#    #+#             */
/*   Updated: 2022/12/18 20:12:40 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vk_semaphore.h"
#include "render_core.h"
#include <renderer/renderer.h>

namespace mlx
{
	void Semaphore::init(Renderer& renderer)
	{
		_imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		_renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		_inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
		_imagesInFlight.resize(renderer.getSwapChain().getImagesNumber(), VK_NULL_HANDLE);

		VkSemaphoreCreateInfo semaphoreInfo{};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo{};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			if(vkCreateSemaphore(Render_Core::get().getDevice().get(), &semaphoreInfo, nullptr, &_imageAvailableSemaphores[i]) != VK_SUCCESS ||
			vkCreateSemaphore(Render_Core::get().getDevice().get(), &semaphoreInfo, nullptr, &_renderFinishedSemaphores[i]) != VK_SUCCESS ||
			vkCreateFence(Render_Core::get().getDevice().get(), &fenceInfo, nullptr, &_inFlightFences[i]) != VK_SUCCESS)
				core::error::report(e_kind::fatal_error, "Vulkan : failed to create synchronization objects for a frame");
		}
	}

	void Semaphore::destroy() noexcept
	{
		for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			vkDestroySemaphore(Render_Core::get().getDevice().get(), _renderFinishedSemaphores[i], nullptr);
			vkDestroySemaphore(Render_Core::get().getDevice().get(), _imageAvailableSemaphores[i], nullptr);
			vkDestroyFence(Render_Core::get().getDevice().get(), _inFlightFences[i], nullptr);
		}
	}
}
