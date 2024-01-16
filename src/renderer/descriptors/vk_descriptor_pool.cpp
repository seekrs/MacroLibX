/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_descriptor_pool.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 18:34:23 by maldavid          #+#    #+#             */
/*   Updated: 2024/01/03 13:13:54 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vk_descriptor_pool.h"
#include <renderer/core/render_core.h>

namespace mlx
{
	void DescriptorPool::init(std::size_t n, VkDescriptorPoolSize* size)
	{
		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = n;
		poolInfo.pPoolSizes = size;
		poolInfo.maxSets = 8192;

		VkResult res = vkCreateDescriptorPool(Render_Core::get().getDevice().get(), &poolInfo, nullptr, &_pool);
		if(res != VK_SUCCESS)
			core::error::report(e_kind::fatal_error, "Vulkan : failed to create descriptor pool, %s", RCore::verbaliseResultVk(res));
	}

	void DescriptorPool::destroy() noexcept    
	{
		vkDestroyDescriptorPool(Render_Core::get().getDevice().get(), _pool, nullptr);
		_pool = VK_NULL_HANDLE;
	}
}
