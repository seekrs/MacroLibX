/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_cmd_pool.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 18:24:33 by maldavid          #+#    #+#             */
/*   Updated: 2022/12/18 22:56:26 by maldavid         ###   ########.fr       */
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
		poolInfo.queueFamilyIndex = Render_Core::get().getQueue().getFamilies().graphicsFamily.value();

		if(vkCreateCommandPool(Render_Core::get().getDevice().get(), &poolInfo, nullptr, &_cmd_pool) != VK_SUCCESS)
			core::error::report(e_kind::fatal_error, "Vulkan : failed to create command pool");
	}

	void CmdPool::destroy() noexcept
	{
		vkDestroyCommandPool(Render_Core::get().getDevice().get(), _cmd_pool, nullptr);
	}
}
