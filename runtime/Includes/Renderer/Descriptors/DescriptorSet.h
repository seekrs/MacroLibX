/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DescriptorSet.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 18:39:36 by maldavid          #+#    #+#             */
/*   Updated: 2024/04/23 19:49:02 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __VK_DESCRIPTOR_SET__
#define __VK_DESCRIPTOR_SET__

#include <Renderer/Core/RenderCore.h>
#include <Renderer/Descriptors/DescriptorSetLayout.h>

namespace mlx
{
	class DescriptorSet
	{
		public:
			DescriptorSet() = default;

			void Init(NonOwningPtr<class Renderer> renderer, NonOwningPtr<class DescriptorPool> pool, DescriptorSetLayout layout);

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
			DescriptorSetLayout p_layout;
			std::array<VkDescriptorSet, MAX_FRAMES_IN_FLIGHT> m_desc_set;
			NonOwningPtr<class DescriptorPool> p_pool;
			NonOwningPtr<class Renderer> p_renderer;
	};
}

#endif
