/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DescriptorSet.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 18:40:44 by maldavid          #+#    #+#             */
/*   Updated: 2024/04/23 19:50:06 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <PreCompiled.h>

#include <Renderer/Descriptors/DescriptorSet.h>
#include <Renderer/Core/RenderCore.h>
#include <Renderer/Descriptors/DescriptorPool.h>
#include <Renderer/Buffers/UniformBuffer.h>
#include <Renderer/Renderer.h>
#include <Renderer/Images/Image.h>

namespace mlx
{
	void DescriptorSet::Init(NonOwningPtr<Renderer> renderer, NonOwningPtr<DescriptorPool> pool, DescriptorSetLayout layout)
	{
		MLX_PROFILE_FUNCTION();
		m_renderer = renderer;
		m_layout = layout;
		m_pool = pool;

		for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
			m_desc_set[i] = pool->AllocateDescriptorSet(layout);
	}

	void DescriptorSet::WriteDescriptor(int binding, NonOwningPtr<UniformBuffer> ubo) const noexcept
	{
		MLX_PROFILE_FUNCTION();
		auto device = RenderCore::Get().GetDevice().Get();

		for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			VkDescriptorBufferInfo buffer_info{};
			buffer_info.buffer = ubo->Get(i);
			buffer_info.offset = ubo->GetOffset(i);
			buffer_info.range = ubo->GetSize(i);

			VkWriteDescriptorSet descriptor_write{};
			descriptor_write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptor_write.dstSet = m_desc_set[i];
			descriptor_write.dstBinding = binding;
			descriptor_write.dstArrayElement = 0;
			descriptor_write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descriptor_write.descriptorCount = 1;
			descriptor_write.pBufferInfo = &buffer_info;

			vkUpdateDescriptorSets(device, 1, &descriptor_write, 0, nullptr);
		}
	}

	void DescriptorSet::WriteDescriptor(int binding, const Image& image) const noexcept
	{
		MLX_PROFILE_FUNCTION();
		auto device = RenderCore::Get().GetDevice().Get();

		VkDescriptorImageInfo image_info{};
		image_info.imageLayout = image.GetLayout();
		image_info.imageView = image.GetImageView();
		image_info.sampler = image.GetSampler();

		VkWriteDescriptorSet descriptor_write{};
		descriptor_write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptor_write.dstSet = m_desc_set[m_renderer->GetActiveImageIndex()];
		descriptor_write.dstBinding = binding;
		descriptor_write.dstArrayElement = 0;
		descriptor_write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptor_write.descriptorCount = 1;
		descriptor_write.pImageInfo = &image_info;

		vkUpdateDescriptorSets(device, 1, &descriptor_write, 0, nullptr);
	}

	DescriptorSet DescriptorSet::Duplicate()
	{
		MLX_PROFILE_FUNCTION();
		DescriptorSet set;
		set.Init(m_renderer, &RenderCore::Get().GetDescriptorPool(), m_layout);
		return set;
	}

	VkDescriptorSet& DescriptorSet::operator()() noexcept
	{
		return m_desc_set[m_renderer->GetActiveImageIndex()];
	}

	VkDescriptorSet& DescriptorSet::Get() noexcept
	{
		return m_desc_set[m_renderer->GetActiveImageIndex()];
	}

	void DescriptorSet::Destroy() noexcept
	{
		MLX_PROFILE_FUNCTION();
		if(m_pool != nullptr && RenderCore::Get().IsInit()) // checks if the render core is still init (it should always be init but just in case)
			m_pool->FreeDescriptor(*this);
		for(auto& set : m_desc_set)
		{
			if(set != VK_NULL_HANDLE)
				set = VK_NULL_HANDLE;
		}
	}
}
