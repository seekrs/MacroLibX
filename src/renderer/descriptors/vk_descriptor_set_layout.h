/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_descriptor_set_layout.h                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 18:36:22 by maldavid          #+#    #+#             */
/*   Updated: 2024/01/20 06:25:54 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __VK_DESCRIPTOR_SET_LAYOUT__
#define __VK_DESCRIPTOR_SET_LAYOUT__

#include <mlx_profile.h>
#include <volk.h>
#include <vector>

namespace mlx
{
	class DescriptorSetLayout
	{
		public:
			DescriptorSetLayout() = default;

			void init(std::vector<std::pair<int, VkDescriptorType>> binds, VkShaderStageFlagBits stage);
			void destroy() noexcept;

			inline VkDescriptorSetLayout& operator()() noexcept { return _layout; }
			inline VkDescriptorSetLayout& get() noexcept { return _layout; }
			inline const std::vector<std::pair<int, VkDescriptorType>>& getBindings() const noexcept { return _bindings; }

			~DescriptorSetLayout() = default;

		private:
			VkDescriptorSetLayout _layout = VK_NULL_HANDLE;
			std::vector<std::pair<int, VkDescriptorType>> _bindings;
	};
}

#endif
