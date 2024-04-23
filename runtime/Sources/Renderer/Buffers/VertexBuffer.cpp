/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VertexBuffer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 18:28:08 by maldavid          #+#    #+#             */
/*   Updated: 2024/04/23 14:48:15 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <PreCompiled.h>

#include <Renderer/Buffers/VertexBuffer.h>

namespace mlx
{
	void RAMVertexBuffer::SetData(std::uint32_t size, const void* data)
	{
		if(size > GetSize())
		{
			Error("Vulkan : trying to store to much data in a vertex buffer (% bytes in % bytes)", size, GetSize());
			return;
		}

		if(data == nullptr)
			Warning("Vulkan : mapping null data in a vertex buffer");

		void* temp = nullptr;
		MapMem(&temp);
			std::memcpy(temp, data, static_cast<std::size_t>(size));
		UnmapMem();
	}

	void DeviceVertexBuffer::SetData(std::uint32_t size, const void* data)
	{
		if(size > GetSize())
		{
			Error("Vulkan : trying to store to much data in a vertex buffer (% bytes in % bytes)", size, GetSize());
			return;
		}

		if(data == nullptr)
			Warning("Vulkan : mapping null data in a vertex buffer");

		Buffer tmp_buf;
		#ifdef DEBUG
			tmp_buf.Create(BufferType::HighDynamic, size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT, "tmp_buffer", data);
		#else
			tmp_buf.Create(BufferType::HighDynamic, size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT, nullptr, data);
		#endif
		CopyFromBuffer(tmp_buf);
		tmp_buf.Destroy();
	}
}
