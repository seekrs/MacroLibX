/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   singleton.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 19:18:46 by maldavid          #+#    #+#             */
/*   Updated: 2022/10/08 19:22:07 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_SINGLETON__
#define __MLX_SINGLETON__

#include "non_copyable.h"

namespace mlx
{
	template <typename T>
	class Singleton : public non_copyable
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
