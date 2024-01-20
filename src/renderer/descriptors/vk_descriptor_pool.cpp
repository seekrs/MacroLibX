/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_descriptor_pool.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 18:34:23 by maldavid          #+#    #+#             */
/*   Updated: 2024/01/20 07:40:40 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vk_descriptor_pool.h"
#include <renderer/descriptors/vk_descriptor_set.h>
#include <renderer/core/render_core.h>

namespace mlx
{
	void DescriptorPool::init(std::size_t n, VkDescriptorPoolSize* size)
	{
		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = n;
		poolInfo.pPoolSizes = size;
		poolInfo.maxSets = MAX_SETS_PER_POOL;
		poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;

		VkResult res = vkCreateDescriptorPool(Render_Core::get().getDevice().get(), &poolInfo, nullptr, &_pool);
		if(res != VK_SUCCESS)
			core::error::report(e_kind::fatal_error, "Vulkan : failed to create descriptor pool, %s", RCore::verbaliseResultVk(res));
		_allocated_sets++;
		#ifdef DEBUG
			core::error::report(e_kind::message, "Vulkan : created new descriptor pool");
		#endif
	}

	void DescriptorPool::freeDescriptor(const DescriptorSet& set)
	{
		if(!isInit())
			return;
		const auto& sets = set.getAllFramesDescriptorSets();
		vkFreeDescriptorSets(Render_Core::get().getDevice().get(), _pool, sets.size(), sets.data());
		_allocated_sets--; // if this goes in underflow I quit
	}

	void DescriptorPool::destroy() noexcept
	{
		if(_pool != VK_NULL_HANDLE)
			vkDestroyDescriptorPool(Render_Core::get().getDevice().get(), _pool, nullptr);
		_pool = VK_NULL_HANDLE;
		#ifdef DEBUG
			core::error::report(e_kind::message, "Vulkan : destroyed a descriptor pool");
		#endif
	}
}
