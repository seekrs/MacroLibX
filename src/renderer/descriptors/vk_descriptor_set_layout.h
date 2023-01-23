/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_descriptor_set_layout.h                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 18:36:22 by maldavid          #+#    #+#             */
/*   Updated: 2023/01/23 18:45:44 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __VK_DESCRIPTOR_SET_LAYOUT__
#define __VK_DESCRIPTOR_SET_LAYOUT__

#include <volk.h>
#include <cstddef>

namespace mlx
{
	class DescriptorSetLayout
	{
		public:
			void init(VkDescriptorType t = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, std::size_t n = 1, int binding = 0, VkShaderStageFlagBits stage = VK_SHADER_STAGE_VERTEX_BIT);
			void destroy() noexcept;

			inline VkDescriptorSetLayout& operator()() noexcept { return _layout; }
			inline VkDescriptorSetLayout& get() noexcept { return _layout; }

		private:
			VkDescriptorSetLayout _layout = VK_NULL_HANDLE;
	};
}

#endif
