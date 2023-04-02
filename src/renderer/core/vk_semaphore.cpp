/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_semaphore.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 19:01:08 by maldavid          #+#    #+#             */
/*   Updated: 2023/04/02 17:55:58 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vk_semaphore.h"
#include "render_core.h"
#include <renderer/renderer.h>

namespace mlx
{
	void Semaphore::init()
	{
		VkSemaphoreCreateInfo semaphoreInfo{};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		if(	vkCreateSemaphore(Render_Core::get().getDevice().get(), &semaphoreInfo, nullptr, &_imageAvailableSemaphores) != VK_SUCCESS ||
			vkCreateSemaphore(Render_Core::get().getDevice().get(), &semaphoreInfo, nullptr, &_renderFinishedSemaphores) != VK_SUCCESS)
			core::error::report(e_kind::fatal_error, "Vulkan : failed to create GPU synchronization object");
	}

	void Semaphore::destroy() noexcept
	{
		vkDestroySemaphore(Render_Core::get().getDevice().get(), _renderFinishedSemaphores, nullptr);
		vkDestroySemaphore(Render_Core::get().getDevice().get(), _imageAvailableSemaphores, nullptr);
	}
}
