/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_descriptor_set.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 18:40:44 by maldavid          #+#    #+#             */
/*   Updated: 2023/01/23 18:54:28 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vk_descriptor_set.h"
#include "vk_descriptor_pool.h"
#include "vk_descriptor_set_layout.h"
#include <renderer/buffers/vk_ubo.h>
#include <renderer/renderer.h>

namespace mlx
{
	void DescriptorSet::init(Renderer* renderer, UBO* ubo, DescriptorSetLayout& layout, DescriptorPool& pool)
	{
		_renderer = renderer;

		auto device = Render_Core::get().getDevice().get();

		_pool = pool.get();

		std::array<VkDescriptorSetLayout, MAX_FRAMES_IN_FLIGHT> layouts;
		layouts.fill(layout.get());

		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = _pool;
		allocInfo.descriptorSetCount = MAX_FRAMES_IN_FLIGHT;
		allocInfo.pSetLayouts = layouts.data();

		if(vkAllocateDescriptorSets(device, &allocInfo, _desc_set.data()) != VK_SUCCESS)
			core::error::report(e_kind::fatal_error, "Vulkan : failed to allocate descriptor set");

		for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			VkDescriptorBufferInfo bufferInfo{};
			bufferInfo.buffer = ubo->get(i);
			bufferInfo.offset = ubo->getOffset(i);
			bufferInfo.range = ubo->getSize(i);

			VkWriteDescriptorSet descriptorWrite{};
			descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrite.dstSet = _desc_set[i];
			descriptorWrite.dstBinding = 0;
			descriptorWrite.dstArrayElement = 0;
			descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descriptorWrite.descriptorCount = 1;
			descriptorWrite.pBufferInfo = &bufferInfo;

			vkUpdateDescriptorSets(device, 1, &descriptorWrite, 0, nullptr);
		}
	}

	VkDescriptorSet& DescriptorSet::operator()() noexcept
	{
		return _desc_set[_renderer->getActiveImageIndex()];
	}
	VkDescriptorSet& DescriptorSet::get() noexcept
	{
		return _desc_set[_renderer->getActiveImageIndex()];
	}

	void DescriptorSet::destroy() noexcept
	{
		vkFreeDescriptorSets(Render_Core::get().getDevice().get(), _pool, MAX_FRAMES_IN_FLIGHT, _desc_set.data());
	}
}
