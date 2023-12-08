/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 16:31:51 by kbz_8             #+#    #+#             */
/*   Updated: 2023/12/08 19:05:15 by kbz_8            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_MEMORY__
#define __MLX_MEMORY__

#include <utils/singleton.h>
#include <core/profile.h>
#include <list>

namespace mlx
{
	class MemManager : public Singleton<MemManager>
	{
		friend class Singleton<MemManager>;

		public:
			void* alloc(std::size_t size);
			void free(void* ptr);

		private:
			MemManager() = default;
			~MemManager();

		private:
			std::list<void*> _blocks;
	};
}

#endif
