/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_descriptor_set_layout.cpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 18:37:28 by maldavid          #+#    #+#             */
/*   Updated: 2023/03/31 16:37:09 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vk_descriptor_set_layout.h"
#include <renderer/core/render_core.h>

namespace mlx
{
	void DescriptorSetLayout::init(std::vector<std::pair<int, VkDescriptorType>> binds, VkShaderStageFlagBits stage)
	{
		std::vector<VkDescriptorSetLayoutBinding> bindings(binds.size());
		for(int i = 0; i < binds.size(); i++)
		{
			bindings[i].binding = binds[i].first;
			bindings[i].descriptorCount = 1;
			bindings[i].descriptorType = binds[i].second;
			bindings[i].pImmutableSamplers = nullptr;
			bindings[i].stageFlags = stage;
		}

		_bindings = std::move(binds);

		VkDescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = _bindings.size();
		layoutInfo.pBindings = bindings.data();

		if(vkCreateDescriptorSetLayout(Render_Core::get().getDevice().get(), &layoutInfo, nullptr, &_layout) != VK_SUCCESS)
			core::error::report(e_kind::fatal_error, "Vulkan : failed to create descriptor set layout");
	}

	void DescriptorSetLayout::destroy() noexcept
	{
		vkDestroyDescriptorSetLayout(Render_Core::get().getDevice().get(), _layout, nullptr);
	}
}
