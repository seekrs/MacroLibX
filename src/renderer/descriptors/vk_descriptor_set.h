/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_descriptor_set.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 18:39:36 by maldavid          #+#    #+#             */
/*   Updated: 2023/01/23 18:54:22 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __VK_DESCRIPTOR_SET__
#define __VK_DESCRIPTOR_SET__

#include <volk.h>
#include <array>
#include <renderer/core/render_core.h>

namespace mlx
{
	class DescriptorSet
	{
		public:
			void init(class Renderer* renderer, class UBO* ubo, class DescriptorSetLayout& layout, class DescriptorPool& pool);
			void destroy() noexcept;

			VkDescriptorSet& operator()() noexcept;
			VkDescriptorSet& get() noexcept;

		private:
			std::array<VkDescriptorSet, MAX_FRAMES_IN_FLIGHT> _desc_set;
			VkDescriptorPool _pool = VK_NULL_HANDLE;
			class Renderer* _renderer = nullptr;
	};
}

#endif
