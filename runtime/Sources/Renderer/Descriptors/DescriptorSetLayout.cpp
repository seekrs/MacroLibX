/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DescriptorSetLayout.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 18:37:28 by maldavid          #+#    #+#             */
/*   Updated: 2024/04/23 19:52:41 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <PreCompiled.h>

#include <Renderer/Descriptors/DescriptorSetLayout.h>
#include <Renderer/Core/Rendercore.h>

namespace mlx
{
	void DescriptorSetLayout::Init(std::vector<std::pair<int, VkDescriptorType>> binds, VkShaderStageFlagBits stage)
	{
		std::vector<VkDescriptorSetLayoutBinding> bindings(binds.size());
		for(std::size_t i = 0; i < binds.size(); i++)
		{
			bindings[i].binding = binds[i].first;
			bindings[i].descriptorCount = 1;
			bindings[i].descriptorType = binds[i].second;
			bindings[i].pImmutableSamplers = nullptr;
			bindings[i].stageFlags = stage;
		}

		m_bindings = std::move(binds);

		VkDescriptorSetLayoutCreateInfo layout_info{};
		layout_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layout_info.bindingCount = m_bindings.size();
		layout_info.pBindings = m_bindings.data();

		VkResult res = vkCreateDescriptorSetLayout(RenderCore::Get().GetDevice().Get(), &layout_info, nullptr, &m_layout);
		if(res != VK_SUCCESS)
			FatalError("Vulkan : failed to create descriptor set layout, %", VerbaliseVkResult(res));
	}

	void DescriptorSetLayout::Destroy() noexcept
	{
		vkDestroyDescriptorSetLayout(RenderCore::Get().GetDevice().Get(), m_layout, nullptr);
		m_layout = VK_NULL_HANDLE;
	}
}
