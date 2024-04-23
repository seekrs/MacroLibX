/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DescriptorPoolManager.h                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 06:26:26 by maldavid          #+#    #+#             */
/*   Updated: 2024/04/23 19:40:22 by maldavid         ###   ########.fr       */
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

			DescriptorPool& GetAvailablePool();
			void DestroyAllPools();

			~DescriptorPoolManager() = default;

		private:
			std::list<DescriptorPool> m_pools;
	};
}

#endif
