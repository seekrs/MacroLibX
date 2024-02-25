/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_vbo.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 18:28:08 by maldavid          #+#    #+#             */
/*   Updated: 2023/12/12 22:17:14 by kbz_8            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vk_vbo.h"
#include <cstring>

namespace mlx
{
	void VBO::setData(std::uint32_t size, const void* data)
	{
		if(size > getSize())
		{
			core::error::report(e_kind::error, "Vulkan : trying to store to much data in a vertex buffer (%d bytes in %d bytes)", size, getSize());
			return;
		}

		if(data == nullptr)
			core::error::report(e_kind::warning, "Vulkan : mapping null data in a vertex buffer");

		void* temp = nullptr;
		mapMem(&temp);
			std::memcpy(temp, data, static_cast<std::size_t>(size));
		unmapMem();
	}

	void D_VBO::setData(std::uint32_t size, const void* data)
	{
		if(size > getSize())
		{
			core::error::report(e_kind::error, "Vulkan : trying to store to much data in a vertex buffer (%d bytes in %d bytes)", size, getSize());
			return;
		}

		if(data == nullptr)
			core::error::report(e_kind::warning, "Vulkan : mapping null data in a vertex buffer");

		Buffer tmp_buf;
		#ifdef DEBUG
			tmp_buf.create(Buffer::kind::dynamic, size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT, "tmp_buffer", data);
		#else
			tmp_buf.create(Buffer::kind::dynamic, size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT, nullptr, data);
		#endif
		copyFromBuffer(tmp_buf);
		tmp_buf.destroy();
	}
}
