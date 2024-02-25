/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_descriptor_set.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 18:40:44 by maldavid          #+#    #+#             */
/*   Updated: 2024/01/20 08:18:07 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vk_descriptor_set.h"
#include "renderer/core/render_core.h"
#include "vk_descriptor_pool.h"
#include "vk_descriptor_set_layout.h"
#include <renderer/buffers/vk_ubo.h>
#include <renderer/renderer.h>
#include <renderer/images/vk_image.h>
#include <core/profiler.h>

namespace mlx
{
	void DescriptorSet::init(Renderer* renderer, DescriptorPool* pool, DescriptorSetLayout* layout)
	{
		MLX_PROFILE_FUNCTION();
		_renderer = renderer;
		_layout = layout;
		_pool = pool;

		auto device = Render_Core::get().getDevice().get();

		std::array<VkDescriptorSetLayout, MAX_FRAMES_IN_FLIGHT> layouts;
		layouts.fill(layout->get());

		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = _pool->get();
		allocInfo.descriptorSetCount = static_cast<std::uint32_t>(MAX_FRAMES_IN_FLIGHT);
		allocInfo.pSetLayouts = layouts.data();

		VkResult res = vkAllocateDescriptorSets(device, &allocInfo, _desc_set.data());
		if(res != VK_SUCCESS)
			core::error::report(e_kind::fatal_error, "Vulkan : failed to allocate descriptor set, %s", RCore::verbaliseResultVk(res));
		#ifdef DEBUG
			core::error::report(e_kind::message, "Vulkan : created new descriptor set");
		#endif
	}

	void DescriptorSet::writeDescriptor(int binding, UBO* ubo) const noexcept
	{
		MLX_PROFILE_FUNCTION();
		auto device = Render_Core::get().getDevice().get();

		for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			VkDescriptorBufferInfo bufferInfo{};
			bufferInfo.buffer = ubo->get(i);
			bufferInfo.offset = ubo->getOffset(i);
			bufferInfo.range = ubo->getSize(i);

			VkWriteDescriptorSet descriptorWrite{};
			descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrite.dstSet = _desc_set[i];
			descriptorWrite.dstBinding = binding;
			descriptorWrite.dstArrayElement = 0;
			descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descriptorWrite.descriptorCount = 1;
			descriptorWrite.pBufferInfo = &bufferInfo;

			vkUpdateDescriptorSets(device, 1, &descriptorWrite, 0, nullptr);
		}
	}

	void DescriptorSet::writeDescriptor(int binding, const Image& image) const noexcept
	{
		MLX_PROFILE_FUNCTION();
		auto device = Render_Core::get().getDevice().get();

		VkDescriptorImageInfo imageInfo{};
		imageInfo.imageLayout = image.getLayout();
		imageInfo.imageView = image.getImageView();
		imageInfo.sampler = image.getSampler();

		VkWriteDescriptorSet descriptorWrite{};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = _desc_set[_renderer->getActiveImageIndex()];
		descriptorWrite.dstBinding = binding;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pImageInfo = &imageInfo;

		vkUpdateDescriptorSets(device, 1, &descriptorWrite, 0, nullptr);
	}

	DescriptorSet DescriptorSet::duplicate()
	{
		MLX_PROFILE_FUNCTION();
		DescriptorSet set;
		set.init(_renderer, &Render_Core::get().getDescriptorPool(), _layout);
		return set;
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
		MLX_PROFILE_FUNCTION();
		if(_pool != nullptr && Render_Core::get().isInit()) // checks if the render core is still init (it should always be init but just in case)
			_pool->freeDescriptor(*this);
		for(auto& set : _desc_set)
		{
			if(set != VK_NULL_HANDLE)
				set = VK_NULL_HANDLE;
		}
		#ifdef DEBUG
			core::error::report(e_kind::message, "Vulkan : destroyed descriptor set");
		#endif
	}
}
