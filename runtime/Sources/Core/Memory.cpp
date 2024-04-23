/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Memory.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 16:32:01 by kbz_8             #+#    #+#             */
/*   Updated: 2024/04/23 14:05:52 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <PreCompiled.h>

#include <Core/Memory.h>

namespace mlx
{
	void* MemManager::Malloc(std::size_t size)
	{
		void* ptr = std::malloc(size);
		if(ptr != nullptr)
			s_blocks.push_back(ptr);
		return ptr;
	}

	void* MemManager::Calloc(std::size_t n, std::size_t size)
	{
		void* ptr = std::calloc(n, size);
		if(ptr != nullptr)
			s_blocks.push_back(ptr);
		return ptr;
	}

	void* MemManager::Realloc(void* ptr, std::size_t size)
	{
		void* ptr2 = std::realloc(ptr, size);
		if(ptr2 != nullptr)
			s_blocks.push_back(ptr2);
		auto it = std::find(s_blocks.begin(), s_blocks.end(), ptr);
		if(it != s_blocks.end())
			s_blocks.erase(it);
		return ptr2;
	}

	void MemManager::Free(void* ptr)
	{
		auto it = std::find(s_blocks.begin(), s_blocks.end(), ptr);
		if(it == s_blocks.end())
		{
			Error("Memory Manager : trying to free a pointer not allocated by the memory manager");
			return;
		}
		std::free(*it);
		s_blocks.erase(it);
	}

	MemManager::~MemManager()
	{
		std::for_each(s_blocks.begin(), s_blocks.end(), [](void* ptr)
		{
			std::free(ptr);
		});
	}
}
