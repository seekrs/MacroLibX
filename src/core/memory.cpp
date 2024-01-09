/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 16:32:01 by kbz_8             #+#    #+#             */
/*   Updated: 2023/12/11 15:25:02 by kbz_8            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <core/memory.h>
#include <core/errors.h>
#include <algorithm>

namespace mlx
{
	void* MemManager::malloc(std::size_t size)
	{
		void* ptr = std::malloc(size);
		if(ptr != nullptr)
			_blocks.push_back(ptr);
		return ptr;
	}

	void* MemManager::calloc(std::size_t n, std::size_t size)
	{
		void* ptr = std::calloc(n, size);
		if(ptr != nullptr)
			_blocks.push_back(ptr);
		return ptr;
	}

	void* MemManager::realloc(void* ptr, std::size_t size)
	{
		void* ptr2 = std::realloc(ptr, size);
		if(ptr2 != nullptr)
			_blocks.push_back(ptr2);
		auto it = std::find(_blocks.begin(), _blocks.end(), ptr);
		if(it != _blocks.end())
			_blocks.erase(it);
		return ptr2;
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
