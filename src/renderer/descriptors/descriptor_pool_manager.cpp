/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   descriptor_pool_manager.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 06:51:47 by maldavid          #+#    #+#             */
/*   Updated: 2024/01/20 08:18:27 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <renderer/core/render_core.h>
#include <renderer/descriptors/descriptor_pool_manager.h>

namespace mlx
{
	DescriptorPool& DescriptorPoolManager::getAvailablePool()
	{
		for(auto& pool : _pools)
		{
			if(pool.getNumberOfSetsAllocated() < MAX_SETS_PER_POOL)
				return pool;
		}
		VkDescriptorPoolSize pool_sizes[] = {
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, (MAX_FRAMES_IN_FLIGHT * NUMBER_OF_UNIFORM_BUFFERS) },
			{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, MAX_SETS_PER_POOL - (MAX_FRAMES_IN_FLIGHT * NUMBER_OF_UNIFORM_BUFFERS) }
		};
		_pools.emplace_front().init((sizeof(pool_sizes) / sizeof(VkDescriptorPoolSize)), pool_sizes);
		return _pools.front();
	}

	void DescriptorPoolManager::destroyAllPools()
	{
		for(auto& pool : _pools)
			pool.destroy();
		_pools.clear();
	}
}
