/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_vbo.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 18:27:38 by maldavid          #+#    #+#             */
/*   Updated: 2022/12/18 02:51:53 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_VK_VBO__
#define __MLX_VK_VBO__

#include "vk_buffer.h"

namespace mlx
{
	class VBO : public Buffer
	{
		public:
			inline void create(uint32_t size) { Buffer::create(Buffer::kind::dynamic, size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT); }

			void setData(uint32_t size, const void* data);
			void setSubData(uint32_t offset, uint32_t size, const void* data);

			inline void bind() noexcept { vkCmdBindVertexBuffers(Render_Core::get().getActiveCmdBuffer().get(), 0, 1, &_buffer, &_offset); }

		private:
			uint32_t _used_size = 0;
	};
}

#endif // __MLX_VK_VBO__
