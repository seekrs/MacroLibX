/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_descriptor_set_layout.h                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 18:36:22 by maldavid          #+#    #+#             */
/*   Updated: 2023/12/08 19:09:44 by kbz_8            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __VK_DESCRIPTOR_SET_LAYOUT__
#define __VK_DESCRIPTOR_SET_LAYOUT__

#include <volk.h>
#include <cstddef>
#include <vector>
#include <map>
#include <mlx_profile.h>

namespace mlx
{
	class DescriptorSetLayout
	{
		public:
			void init(std::vector<std::pair<int, VkDescriptorType>> binds, VkShaderStageFlagBits stage);
			void destroy() noexcept;

			inline VkDescriptorSetLayout& operator()() noexcept { return _layout; }
			inline VkDescriptorSetLayout& get() noexcept { return _layout; }
			inline const std::vector<std::pair<int, VkDescriptorType>>& getBindings() const noexcept { return _bindings; }

		private:
			VkDescriptorSetLayout _layout = VK_NULL_HANDLE;
			std::vector<std::pair<int, VkDescriptorType>> _bindings;
	};
}

#endif
