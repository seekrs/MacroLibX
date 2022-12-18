/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_vbo.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 18:28:08 by maldavid          #+#    #+#             */
/*   Updated: 2022/12/18 00:28:52 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vk_vbo.h"
#include <cstring>

namespace mlx
{
	void VBO::setData(uint32_t size, const void* data)
	{
		if(size > _size)
			core::error::report(e_kind::error, "Vulkan : trying to store to much data in a vertex buffer (%d on %d)", size, _size);

		if(data == nullptr)
			core::error::report(e_kind::warning, "Vulkan : mapping null data in a vertex buffer");

		void* temp = nullptr;
		mapMem(&temp);
			std::memcpy(temp, data, static_cast<size_t>(size));
		unmapMem();

		_used_size += size;
	}

	void VBO::setSubData(uint32_t offset, uint32_t size, const void* data)
	{
		if(size + _used_size > _size)
			core::error::report(e_kind::error, "Vulkan : trying to store to much data in a vertex buffer (%d on %d)", size + _used_size, _size);

		if(data == nullptr)
			core::error::report(e_kind::warning, "Vulkan : mapping null data in a vertex buffer");

		void* temp = nullptr;
		mapMem(&temp, size, offset);
			std::memcpy(temp, data, static_cast<size_t>(size));
		unmapMem();
		
		_used_size += size;
	}
}
