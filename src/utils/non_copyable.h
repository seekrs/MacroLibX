/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   non_copyable.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 19:20:13 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/24 14:42:48 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_NON_COPYABLE__
#define __MLX_NON_COPYABLE__

namespace mlx
{
	class NonCopyable
	{
		protected:
			NonCopyable() = default;
			virtual ~NonCopyable() = default;

		public:
			NonCopyable(const NonCopyable&) = delete;
			NonCopyable(NonCopyable&&) noexcept = default;
			NonCopyable &operator=(const NonCopyable&) = delete;
			NonCopyable &operator=(NonCopyable&&) noexcept = default;
	};

}

#endif // __MLX_NON_COPYABLE__
