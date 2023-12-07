/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbz_8 </var/spool/mail/kbz_8>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 16:31:51 by kbz_8             #+#    #+#             */
/*   Updated: 2023/12/07 16:37:15 by kbz_8            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_MEMORY__
#define __MLX_MEMORY__

#include <utils/singleton.h>
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
