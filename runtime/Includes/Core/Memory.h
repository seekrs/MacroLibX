/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Memory.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 16:31:51 by kbz_8             #+#    #+#             */
/*   Updated: 2024/03/27 21:16:44 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_MEMORY__
#define __MLX_MEMORY__

#include <Utils/Singleton.h>

namespace mlx
{
	class MemManager : public Singleton<MemManager>
	{
		friend class Singleton<MemManager>;

		public:
			static void* Malloc(std::size_t size);
			static void* Calloc(std::size_t n, std::size_t size);
			static void* Realloc(void* ptr, std::size_t size);
			static void Free(void* ptr);

		private:
			MemManager() = default;
			~MemManager();

		private:
			inline static std::list<void*> s_blocks;
	};
}

#endif