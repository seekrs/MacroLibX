/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   singleton.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 19:18:46 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/24 14:42:56 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_SINGLETON__
#define __MLX_SINGLETON__

#include "non_copyable.h"

namespace mlx
{
	template <typename T>
	class Singleton : public NonCopyable
	{
		public:
			inline static T& get()
			{
				static T instance;
				return instance;
			}
	};
}

#endif // __MLX_SINGLETON__
