/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_ibo.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 15:05:05 by maldavid          #+#    #+#             */
/*   Updated: 2024/01/10 23:05:15 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __VK_IBO__
#define __VK_IBO__

#include <mlx_profile.h>
#include <volk.h>
#include "vk_buffer.h"
#include <renderer/renderer.h>

namespace mlx
{
	class C_IBO : public Buffer
	{
		public:
			inline void create(std::uint32_t size, const std::uint16_t* data, const char* name) { Buffer::create(Buffer::kind::constant, size, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, name, data); }
			inline void bind(Renderer& renderer) noexcept { renderer.getActiveCmdBuffer().bindIndexBuffer(*this); }
	};
}

#endif
