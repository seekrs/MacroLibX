/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_vbo.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 18:28:08 by maldavid          #+#    #+#             */
/*   Updated: 2023/11/10 08:33:52 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vk_vbo.h"
#include <cstring>

namespace mlx
{
	void VBO::setData(uint32_t size, const void* data)
	{
		if(size > getSize())
			core::error::report(e_kind::error, "Vulkan : trying to store to much data in a vertex buffer (%d bytes in %d bytes)", size, getSize());

		if(data == nullptr)
			core::error::report(e_kind::warning, "Vulkan : mapping null data in a vertex buffer");

		void* temp = nullptr;
		mapMem(&temp);
			std::memcpy(temp, data, static_cast<size_t>(size));
		unmapMem();
	}
}
