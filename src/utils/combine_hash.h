/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   combine_hash.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 16:16:06 by maldavid          #+#    #+#             */
/*   Updated: 2023/12/14 16:47:39 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_HASH__
#define __MLX_HASH__

#include <cstddef>
#include <functional>

namespace mlx
{
	inline void hashCombine([[maybe_unused]] std::size_t& seed) noexcept {}

	template <typename T, typename... Rest>
	inline void hashCombine(std::size_t& seed, const T& v, Rest... rest)
	{
		std::hash<T> hasher;
		seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		hashCombine(seed, rest...);
	}
}

#endif
