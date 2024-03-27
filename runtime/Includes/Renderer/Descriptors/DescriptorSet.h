/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DescriptorSet.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 18:39:36 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/27 23:02:38 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __VK_DESCRIPTOR_SET__
#define __VK_DESCRIPTOR_SET__

#include <Renderer/Core/RenderCore.h>

namespace mlx
{
	class DescriptorSet
	{
		public:
			DescriptorSet() = default;

			void Init(class Renderer* renderer, class DescriptorPool* pool, class DescriptorSetLayout* layout);

			void WriteDescriptor(int binding, NonOwningPtr<class UniformBuffer> ubo) const noexcept;
			void WriteDescriptor(int binding, const class Image& image) const noexcept;

			inline bool IsInit() const noexcept { return m_pool != nullptr && m_renderer != nullptr; }

			DescriptorSet Duplicate();

			VkDescriptorSet& operator()() noexcept;
			VkDescriptorSet& Get() noexcept;

			inline const std::array<VkDescriptorSet, MAX_FRAMES_IN_FLIGHT>& GetAllFramesDescriptorSets() const { return m_desc_set; }

			void Destroy() noexcept;

			~DescriptorSet() = default;

		private:
			std::array<VkDescriptorSet, MAX_FRAMES_IN_FLIGHT> m_desc_set;
			NonOwningPtr<class DescriptorPool> p_pool;
			NonOwningPtr<class DescriptorSetLayout> p_layout;
			NonOwningPtr<class Renderer> p_renderer;
	};
}

#endif
