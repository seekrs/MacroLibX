/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VertexBuffer.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 18:27:38 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/28 22:23:32 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_VK_VBO__
#define __MLX_VK_VBO__

#include <Renderer/Enums.h>
#include <Renderer/Buffer/Buffer.h>
#include <Renderer/Renderer.h>

namespace mlx
{
	class RAMVertexBuffer : public Buffer
	{
		public:
			inline void Create(std::uint32_t size, const void* data, const char* name) { Buffer::Create(BufferType::HighDynamic, size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, name, data); }
			void SetData(std::uint32_t size, const void* data);
			inline void Bind(Renderer& renderer) noexcept { renderer.GetActiveCmdBuffer().BindVertexBuffer(*this); }
	};

	class DeviceVertexBuffer : public Buffer
	{
		public:
			inline void create(std::uint32_t size, const void* data, const char* name) { Buffer::Create(BufferType::LowDynamic, size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, name, data); }
			void SetData(std::uint32_t size, const void* data);
			inline void Bind(Renderer& renderer) noexcept { renderer.GetActiveCmdBuffer().BindVertexBuffer(*this); }
	};

	class ConstantVertexBuffer : public Buffer
	{
		public:
			inline void Create(std::uint32_t size, const void* data, const char* name) { Buffer::Create(BufferType::Constant, size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, name, data); }
			inline void Bind(Renderer& renderer) noexcept { renderer.GetActiveCmdBuffer().BindVertexBuffer(*this); }
	};
}

#endif // __MLX_VK_VBO__
