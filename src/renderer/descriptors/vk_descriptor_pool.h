/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_descriptor_pool.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 18:32:43 by maldavid          #+#    #+#             */
/*   Updated: 2024/01/20 07:38:32 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __VK_DESCRIPTOR_POOL__
#define __VK_DESCRIPTOR_POOL__

#include <mlx_profile.h>
#include <volk.h>
#include <cstddef>

namespace mlx
{
	class DescriptorPool
	{
		public:
			DescriptorPool() = default;

			void init(std::size_t n, VkDescriptorPoolSize* size);
			void freeDescriptor(const class DescriptorSet& set);
			void destroy() noexcept;

			inline VkDescriptorPool& operator()() noexcept { return _pool; }
			inline VkDescriptorPool& get() noexcept { return _pool; }
			inline std::size_t getNumberOfSetsAllocated() const noexcept { return _allocated_sets; }

			inline bool isInit() const noexcept { return _pool != VK_NULL_HANDLE; }

			~DescriptorPool() = default;

		private:
			VkDescriptorPool _pool = VK_NULL_HANDLE;
			std::size_t _allocated_sets = 0;
	};
}

#endif
