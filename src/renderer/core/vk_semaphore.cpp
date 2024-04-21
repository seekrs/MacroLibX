/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_semaphore.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 19:01:08 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/14 17:02:36 by maldavid         ###   ########.fr       */
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

		VkResult res;
		if(	(res = vkCreateSemaphore(Render_Core::get().getDevice().get(), &semaphoreInfo, nullptr, &_image_available_semaphore)) != VK_SUCCESS ||
			(res = vkCreateSemaphore(Render_Core::get().getDevice().get(), &semaphoreInfo, nullptr, &_render_finished_semaphore)) != VK_SUCCESS)
			core::error::report(e_kind::fatal_error, "Vulkan : failed to create a synchronization object (semaphore), %s", RCore::verbaliseResultVk(res));
		#ifdef DEBUG
			core::error::report(e_kind::message, "Vulkan : created new semaphores");
		#endif
	}

	void Semaphore::destroy() noexcept
	{
		vkDestroySemaphore(Render_Core::get().getDevice().get(), _render_finished_semaphore, nullptr);
		_render_finished_semaphore = VK_NULL_HANDLE;
		vkDestroySemaphore(Render_Core::get().getDevice().get(), _image_available_semaphore, nullptr);
		_image_available_semaphore = VK_NULL_HANDLE;
		#ifdef DEBUG
			core::error::report(e_kind::message, "Vulkan : destroyed semaphores");
		#endif
	}
}
