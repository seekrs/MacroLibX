/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_vbo.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 18:27:38 by maldavid          #+#    #+#             */
/*   Updated: 2024/01/10 23:04:40 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_VK_VBO__
#define __MLX_VK_VBO__

#include "vk_buffer.h"
#include <renderer/renderer.h>
#include <mlx_profile.h>

namespace mlx
{
	class VBO : public Buffer
	{
		public:
			inline void create(std::uint32_t size, const void* data, const char* name) { Buffer::create(Buffer::kind::dynamic, size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, name, data); }
			void setData(std::uint32_t size, const void* data);
			inline void bind(Renderer& renderer) noexcept { renderer.getActiveCmdBuffer().bindVertexBuffer(*this); }
	};

	class D_VBO : public Buffer
	{
		public:
			inline void create(std::uint32_t size, const void* data, const char* name) { Buffer::create(Buffer::kind::dynamic_device_local, size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, name, data); }
			void setData(std::uint32_t size, const void* data);
			inline void bind(Renderer& renderer) noexcept { renderer.getActiveCmdBuffer().bindVertexBuffer(*this); }
	};

	class C_VBO : public Buffer
	{
		public:
			inline void create(std::uint32_t size, const void* data, const char* name) { Buffer::create(Buffer::kind::constant, size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, name, data); }
			inline void bind(Renderer& renderer) noexcept { renderer.getActiveCmdBuffer().bindVertexBuffer(*this); }
	};
}

#endif // __MLX_VK_VBO__
