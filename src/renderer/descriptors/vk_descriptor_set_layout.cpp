/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_descriptor_set_layout.cpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 18:37:28 by maldavid          #+#    #+#             */
/*   Updated: 2023/01/23 18:45:32 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vk_descriptor_set_layout.h"
#include <renderer/core/render_core.h>

namespace mlx
{
	void DescriptorSetLayout::init(VkDescriptorType t, std::size_t n, int binding, VkShaderStageFlagBits stage)
	{
		VkDescriptorSetLayoutBinding bindings{};
		bindings.binding = binding;
		bindings.descriptorCount = 1;
		bindings.descriptorType = t;
		bindings.pImmutableSamplers = nullptr;
		bindings.stageFlags = stage;

		VkDescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = n;
		layoutInfo.pBindings = &bindings;

		if(vkCreateDescriptorSetLayout(Render_Core::get().getDevice().get(), &layoutInfo, nullptr, &_layout) != VK_SUCCESS)
			core::error::report(e_kind::fatal_error, "Vulkan : failed to create descriptor set layout");
	}

	void DescriptorSetLayout::destroy() noexcept
	{
		vkDestroyDescriptorSetLayout(Render_Core::get().getDevice().get(), _layout, nullptr);
	}
}
