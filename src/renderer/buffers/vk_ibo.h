/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_ibo.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 15:05:05 by maldavid          #+#    #+#             */
/*   Updated: 2023/12/11 19:47:47 by kbz_8            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __VK_IBO__
#define __VK_IBO__

#include <volk.h>
#include "vk_buffer.h"
#include <renderer/renderer.h>
#include <mlx_profile.h>

namespace mlx
{
	class C_IBO : public Buffer
	{
		public:
			inline void create(uint32_t size, const uint16_t* data, const char* name) { Buffer::create(Buffer::kind::constant, size, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, name, data); }
			inline void bind(Renderer& renderer) noexcept { vkCmdBindIndexBuffer(renderer.getActiveCmdBuffer().get(), _buffer, _offset, VK_INDEX_TYPE_UINT16); }
	};
}

#endif
