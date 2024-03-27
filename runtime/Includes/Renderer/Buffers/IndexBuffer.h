/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IndexBuffer.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 15:05:05 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/27 22:11:57 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __VK_IBO__
#define __VK_IBO__

#include <Renderer/Buffer/Buffer.h>
#include <Renderer/Renderer.h>

namespace mlx
{
	class C_IBO : public Buffer
	{
		public:
			inline void Create(std::uint32_t size, const std::uint16_t* data, const char* name) { Buffer::Create(BufferType::Constant, size, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, name, data); }
			inline void Bind(Renderer& renderer) noexcept { renderer.GetActiveCmdBuffer().BindIndexBuffer(*this); }
	};
}

#endif
