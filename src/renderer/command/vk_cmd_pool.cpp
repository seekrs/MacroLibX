/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_cmd_pool.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 18:24:33 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/14 17:23:20 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vk_cmd_pool.h"
#include <renderer/core/render_core.h>

namespace mlx
{
	void CmdPool::init()
	{
		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		poolInfo.queueFamilyIndex = Render_Core::get().getQueue().getFamilies().graphics_family.value();

		VkResult res = vkCreateCommandPool(Render_Core::get().getDevice().get(), &poolInfo, nullptr, &_cmd_pool);
		if(res != VK_SUCCESS)
			core::error::report(e_kind::fatal_error, "Vulkan : failed to create command pool, %s", RCore::verbaliseResultVk(res));
	}

	void CmdPool::destroy() noexcept
	{
		vkDestroyCommandPool(Render_Core::get().getDevice().get(), _cmd_pool, nullptr);
		_cmd_pool = VK_NULL_HANDLE;
	}
}
