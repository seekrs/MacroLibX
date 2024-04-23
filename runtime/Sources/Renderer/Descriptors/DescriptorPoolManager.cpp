/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DescriptorPoolManager.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 06:51:47 by maldavid          #+#    #+#             */
/*   Updated: 2024/04/23 19:41:38 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <PreCompiled.h>

#include <Renderer/Core/RenderCore.h>
#include <Renderer/Descriptors/DescriptorPoolManager.h>

namespace mlx
{
	DescriptorPool& DescriptorPoolManager::GetAvailablePool()
	{
		for(auto& pool : m_pools)
		{
			if(pool.GetNumberOfSetsAllocated() < MAX_SETS_PER_POOL)
				return pool;
		}
		std::vector<VkDescriptorPoolSize> pool_sizes = {
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, (MAX_FRAMES_IN_FLIGHT * NUMBER_OF_UNIFORM_BUFFERS) },
			{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, MAX_SETS_PER_POOL - (MAX_FRAMES_IN_FLIGHT * NUMBER_OF_UNIFORM_BUFFERS) }
		};
		m_pools.emplace_front().Init(std::move(pool_sizes));
		return m_pools.front();
	}

	void DescriptorPoolManager::DestroyAllPools()
	{
		for(auto& pool : m_pools)
			pool.Destroy();
		m_pools.clear();
	}
}
