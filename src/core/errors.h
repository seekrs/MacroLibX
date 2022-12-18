/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 17:42:32 by maldavid          #+#    #+#             */
/*   Updated: 2022/10/08 19:06:41 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_ERRORS__
#define __MLX_ERRORS__

#include <string>

enum class e_kind
{
	message,
	warning,
	error,
	fatal_error
};

namespace mlx::core::error
{
	void report(e_kind kind, std::string msg, ...);
}

#endif // __MLX_ERRORS__
