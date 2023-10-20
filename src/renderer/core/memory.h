/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 02:13:03 by maldavid          #+#    #+#             */
/*   Updated: 2023/10/20 03:33:28 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_VK_MEMORY__
#define __MLX_VK_MEMORY__

#include <volk.h>
#include <vma.h>

namespace mlx
{
	enum class gpu_allocation_type
	{
		buffer,
		image,
	};

	class GPUallocator
	{
		public:
			GPUallocator() = default;
			void init() noexcept;
			VkDeviceMemory alloc(gpu_allocation_type type, VkDeviceSize size);
			void destroy() noexcept;
			~GPUallocator() = default;

		private:
			VmaAllocator _allocator;
	};
}

#endif
