/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Enums.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 17:15:24 by maldavid          #+#    #+#             */
/*   Updated: 2024/07/05 13:23:10 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_CORE_ENUMS__
#define __MLX_CORE_ENUMS__

#include <cstddef>

namespace mlx
{
	enum class LogType
	{
		Message = 0,
		Warning,
		Error,
		FatalError,
		Debug,

		EndEnum
	};

	constexpr std::size_t LogTypeCount = static_cast<std::size_t>(LogType::EndEnum);
}

#endif
