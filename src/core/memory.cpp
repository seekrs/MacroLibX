/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 16:32:01 by kbz_8             #+#    #+#             */
/*   Updated: 2023/12/08 12:56:14 by kbz_8            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <core/memory.h>
#include <core/errors.h>
#include <algorithm>
#include <stdlib.h>

namespace mlx
{
	void* MemManager::alloc(std::size_t size)
	{
		void* ptr = std::malloc(size);
		if(ptr != nullptr)
			_blocks.push_back(ptr);
		return ptr;
	}

	void MemManager::free(void* ptr)
	{
		auto it = std::find(_blocks.begin(), _blocks.end(), ptr);
		if(it == _blocks.end())
		{
			core::error::report(e_kind::error, "Memory Manager : trying to free a pointer not allocated by the memory manager");
			return;
		}
		std::free(*it);
		_blocks.erase(it);
	}

	MemManager::~MemManager()
	{
		std::for_each(_blocks.begin(), _blocks.end(), [](void* ptr)
		{
			std::free(ptr);
		});
	}
}
