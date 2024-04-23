/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DescriptorPool.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 18:34:23 by maldavid          #+#    #+#             */
/*   Updated: 2024/04/23 19:39:39 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <PreCompiled.h>

#include <Renderer/Descriptors/DescriptorPool.h>
#include <Renderer/Descriptors/DescriptorSet.h>
#include <Renderer/Core/RenderCore.h>

namespace mlx
{
	void DescriptorPool::Init(std::vector<VkDescriptorPoolSize> sizes)
	{
		VkDescriptorPoolCreateInfo pool_info{};
		pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		pool_info.poolSizeCount = sizes.size();
		pool_info.pPoolSizes = sizes.data();
		pool_info.maxSets = MAX_SETS_PER_POOL;
		pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;

		VkResult res = vkCreateDescriptorPool(RenderCore::Get().GetDevice().Get(), &pool_info, nullptr, &m_pool);
		if(res != VK_SUCCESS)
			FatalError("Vulkan : failed to create descriptor pool, %", VerbaliseVkResult(res));
		DebugLog("Vulkan : created new descriptor pool");
	}

	VkDescriptorSet DescriptorPool::AllocateDescriptorSet(class DescriptorSetLayout& layout)
	{
		VkDescriptorSet set;

		VkDescriptorSetAllocateInfo alloc_info{};
		alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		alloc_info.descriptorPool = m_pool;
		alloc_info.descriptorSetCount = 1;
		alloc_info.pSetLayouts = layouts.Get();

		VkResult res = vkAllocateDescriptorSets(RenderCore::Get().GetDevice().Get(), &alloc_info, &set);
		if(res != VK_SUCCESS)
			FatalError("Vulkan : failed to allocate descriptor set, %", VerbaliseVkResult(res));
		m_allocated_sets++;
		DebugLog("Vulkan : created new descriptor set");
		return set;
	}

	void DescriptorPool::FreeDescriptor(VkDescriptorSet set)
	{
		if(!IsInit())
			return;
		vkFreeDescriptorSets(RenderCore::Get().GetDevice().Get(), m_pool, 1, set);
		m_allocated_sets--; // if this goes underflow I quit
	}

	void DescriptorPool::Destroy() noexcept
	{
		if(m_pool != VK_NULL_HANDLE)
			vkDestroyDescriptorPool(RenderCore::Get().GetDevice().Get(), m_pool, nullptr);
		m_pool = VK_NULL_HANDLE;
		DebugLog("Vulkan : destroyed a descriptor pool");
	}
}
