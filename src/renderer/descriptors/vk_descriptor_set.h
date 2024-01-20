/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_descriptor_set.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 18:39:36 by maldavid          #+#    #+#             */
/*   Updated: 2024/01/20 07:17:39 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __VK_DESCRIPTOR_SET__
#define __VK_DESCRIPTOR_SET__

#include <mlx_profile.h>
#include <volk.h>
#include <array>
#include <renderer/core/render_core.h>

namespace mlx
{
	class DescriptorSet
	{
		public:
			DescriptorSet() = default;

			void init(class Renderer* renderer, class DescriptorPool* pool, class DescriptorSetLayout* layout);

			void writeDescriptor(int binding, class UBO* ubo) const noexcept;
			void writeDescriptor(int binding, const class Image& image) const noexcept;

			inline bool isInit() const noexcept { return _pool != nullptr && _renderer != nullptr; }

			DescriptorSet duplicate();

			VkDescriptorSet& operator()() noexcept;
			VkDescriptorSet& get() noexcept;

			inline const std::array<VkDescriptorSet, MAX_FRAMES_IN_FLIGHT>& getAllFramesDescriptorSets() const { return _desc_set; }

			void destroy() noexcept;

			~DescriptorSet() = default;

		private:
			std::array<VkDescriptorSet, MAX_FRAMES_IN_FLIGHT> _desc_set;
			class DescriptorPool* _pool = nullptr;
			class DescriptorSetLayout* _layout = nullptr;
			class Renderer* _renderer = nullptr;
	};
}

#endif
