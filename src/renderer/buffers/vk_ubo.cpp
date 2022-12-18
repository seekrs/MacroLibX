/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_ubo.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 18:45:52 by maldavid          #+#    #+#             */
/*   Updated: 2022/12/18 00:25:55 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vk_ubo.h"
#include <cstring>

namespace mlx
{
	void UBO::setData(uint32_t size, const void* data)
	{
		void* temp = nullptr;
		mapMem(&temp);
			std::memcpy(temp, data, static_cast<size_t>(size));
		unmapMem();
	}

	void UBO::setDynamicData(uint32_t size, uint32_t typeSize, const void* data)
	{
		void* temp = nullptr;
		mapMem(&temp);
			std::memcpy(temp, data, static_cast<size_t>(size));
			Buffer::flush();
		unmapMem();
	}
}
