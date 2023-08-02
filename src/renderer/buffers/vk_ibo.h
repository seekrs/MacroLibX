/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_ibo.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 15:05:05 by maldavid          #+#    #+#             */
/*   Updated: 2023/04/22 19:51:54 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __VK_IBO__
#define __VK_IBO__

#include <volk.h>
#include "vk_buffer.h"
#include <renderer/renderer.h>

namespace mlx
{
	class C_IBO : public Buffer
	{
		public:
			inline void create(uint32_t size, const uint16_t* data) { Buffer::create(Buffer::kind::constant, size, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, data); }
			inline void bind(Renderer& renderer) noexcept { vkCmdBindIndexBuffer(renderer.getActiveCmdBuffer().get(), _buffer, _offset, VK_INDEX_TYPE_UINT16); }
	};
}

#endif
