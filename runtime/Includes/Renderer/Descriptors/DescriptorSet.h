/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DescriptorSet.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 18:39:36 by maldavid          #+#    #+#             */
/*   Updated: 2024/07/05 13:42:22 by maldavid         ###   ########.fr       */
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

			inline bool IsInit() const noexcept { return p_pool && p_renderer; }

			void Bind() noexcept;

			DescriptorSet Duplicate();

			VkDescriptorSet& operator()() noexcept;
			VkDescriptorSet& Get() noexcept;

			inline const DescriptorSetLayout& GetLayout() const noexcept { return m_layout; }

			inline const std::array<VkDescriptorSet, MAX_FRAMES_IN_FLIGHT>& GetAllFramesDescriptorSets() const { return m_desc_set; }

			void Destroy() noexcept;

			~DescriptorSet() = default;

		private:
			DescriptorSetLayout m_layout;
			std::array<VkDescriptorSet, MAX_FRAMES_IN_FLIGHT> m_desc_set;
			NonOwningPtr<class DescriptorPool> p_pool;
			NonOwningPtr<class Renderer> p_renderer;
	};
}

#endif
