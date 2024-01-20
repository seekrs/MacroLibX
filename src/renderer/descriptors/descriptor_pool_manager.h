/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   descriptor_pool_manager.h                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 06:26:26 by maldavid          #+#    #+#             */
/*   Updated: 2024/01/20 08:23:04 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_DESCRIPTOR_POOL_MANAGER__
#define __MLX_DESCRIPTOR_POOL_MANAGER__

#include <renderer/descriptors/vk_descriptor_pool.h>
#include <list>

namespace mlx
{
	class DescriptorPoolManager
	{
		public:
			DescriptorPoolManager() = default;

			DescriptorPool& getAvailablePool(); // assumes the pool is for only one set allocation, may cause some issues if this is for more than one
			void destroyAllPools();

			~DescriptorPoolManager() = default;

		private:
			std::list<DescriptorPool> _pools;
	};
}

#endif
