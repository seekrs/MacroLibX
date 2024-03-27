/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DescriptorPoolManager.h                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 06:26:26 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/27 23:00:56 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_DESCRIPTOR_POOL_MANAGER__
#define __MLX_DESCRIPTOR_POOL_MANAGER__

#include <Renderer/Descriptors/DescriptorPool.h>

namespace mlx
{
	class DescriptorPoolManager
	{
		public:
			DescriptorPoolManager() = default;

			DescriptorPool& GetAvailablePool(); // assumes the pool is for only one set allocation, may cause some issues if this is for more than one
			void DestroyAllPools();

			~DescriptorPoolManager() = default;

		private:
			std::list<DescriptorPool> m_pools;
	};
}

#endif
