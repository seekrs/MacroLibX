/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DescriptorPool.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 18:32:43 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/27 23:00:29 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __VK_DESCRIPTOR_POOL__
#define __VK_DESCRIPTOR_POOL__

namespace mlx
{
	class DescriptorPool
	{
		public:
			DescriptorPool() = default;

			void Init(std::size_t n, NonOwningPtr<VkDescriptorPoolSize> size);
			void FreeDescriptor(const class DescriptorSet& set);
			void Destroy() noexcept;

			inline VkDescriptorPool& operator()() noexcept { return m_pool; }
			inline VkDescriptorPool& Get() noexcept { return m_pool; }
			inline std::size_t GetNumberOfSetsAllocated() const noexcept { return m_allocated_sets; }

			inline bool IsInit() const noexcept { return m_pool != VK_NULL_HANDLE; }

			~DescriptorPool() = default;

		private:
			VkDescriptorPool m_pool = VK_NULL_HANDLE;
			std::size_t m_allocated_sets = 0;
	};
}

#endif
