/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DescriptorSetLayout.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 18:36:22 by maldavid          #+#    #+#             */
/*   Updated: 2024/04/23 19:50:50 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __VK_DESCRIPTOR_SET_LAYOUT__
#define __VK_DESCRIPTOR_SET_LAYOUT__

namespace mlx
{
	class DescriptorSetLayout
	{
		public:
			DescriptorSetLayout() = default;

			void Init(std::vector<std::pair<int, VkDescriptorType>> binds, VkShaderStageFlagBits stage);
			void Destroy() noexcept;

			inline VkDescriptorSetLayout& operator()() noexcept { return m_layout; }
			inline VkDescriptorSetLayout& Get() noexcept { return m_layout; }
			inline const std::vector<std::pair<int, VkDescriptorType>>& GetBindings() const noexcept { return m_bindings; }

			~DescriptorSetLayout() = default;

		private:
			std::vector<std::pair<int, VkDescriptorType>> m_bindings;
			VkDescriptorSetLayout m_layout = VK_NULL_HANDLE;
	};
}

#endif
