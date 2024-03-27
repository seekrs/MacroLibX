/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Format.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 17:11:09 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/27 17:12:03 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_FORMAT__
#define __MLX_FORMAT__

#include <type_traits>
#include <string_view>

namespace mlx
{
	template<typename T, typename = void>
	struct IsOstreamable : std::false_type {};

	template<typename T>
	struct IsOstreamable<T, std::void_t<decltype(std::declval<std::ostream>() << std::declval<T>())>> : std::true_type {};

	template<typename... Args, std::enable_if_t<std::conjunction_v<IsOstreamable<Args>...>, int> = 0>
	auto Format(std::string_view format, const Args&... args);
}

#include <Core/Format.inl>

#endif
