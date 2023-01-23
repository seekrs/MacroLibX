/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_descriptor_pool.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 18:34:23 by maldavid          #+#    #+#             */
/*   Updated: 2023/01/23 18:44:51 by maldavid         ###   ########.fr       */
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

		if(vkCreateDescriptorPool(Render_Core::get().getDevice().get(), &poolInfo, nullptr, &_pool) != VK_SUCCESS)
			core::error::report(e_kind::fatal_error, "Vulkan : failed to create descriptor pool");
	}

	void DescriptorPool::destroy() noexcept    
	{
		vkDestroyDescriptorPool(Render_Core::get().getDevice().get(), _pool, nullptr);
	}
}
