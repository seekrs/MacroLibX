/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_ubo.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 18:45:29 by maldavid          #+#    #+#             */
/*   Updated: 2022/10/06 18:45:49 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_VK_UBO__
#define __MLX_VK_UBO__

#include "vk_buffer.h"

namespace mlx
{
	class UBO : public Buffer
	{
		public:
			inline void create(uint32_t size) { Buffer::create(Buffer::kind::dynamic, size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT); }

			void setData(uint32_t size, const void* data);
			void setDynamicData(uint32_t size, uint32_t typeSize, const void* data);
	};
}

#endif // __MLX_VK_UBO__
