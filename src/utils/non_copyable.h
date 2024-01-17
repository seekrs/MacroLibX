/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   non_copyable.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 19:20:13 by maldavid          #+#    #+#             */
/*   Updated: 2022/10/08 19:21:22 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_NON_COPYABLE__
#define __MLX_NON_COPYABLE__

namespace mlx
{
	class non_copyable
	{
		protected:
			non_copyable() = default;
			virtual ~non_copyable() = default;

		public:
			non_copyable(const non_copyable&) = delete;
			non_copyable(non_copyable&&) noexcept = default;
			non_copyable &operator=(const non_copyable&) = delete;
			non_copyable &operator=(non_copyable&&) noexcept = default;
	};

}

#endif // __MLX_NON_COPYABLE__
