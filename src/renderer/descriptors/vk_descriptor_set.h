/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_descriptor_set.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 18:39:36 by maldavid          #+#    #+#             */
/*   Updated: 2023/03/31 17:28:36 by maldavid         ###   ########.fr       */
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
			void init(class Renderer* renderer, class DescriptorPool* pool, class DescriptorSetLayout* layout);

			void writeDescriptor(int binding, class UBO* ubo) noexcept;
			void writeDescriptor(int binding, VkImageView view, VkSampler sampler) noexcept;

			inline bool isInit() noexcept { return _pool != nullptr && _renderer != nullptr; }

			DescriptorSet duplicate();

			VkDescriptorSet& operator()() noexcept;
			VkDescriptorSet& get() noexcept;

		private:
			std::array<VkDescriptorSet, MAX_FRAMES_IN_FLIGHT> _desc_set;
			class DescriptorPool* _pool = nullptr;
			class DescriptorSetLayout* _layout = nullptr;
			class Renderer* _renderer = nullptr;
	};
}

#endif
