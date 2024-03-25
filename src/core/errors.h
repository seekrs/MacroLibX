/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 17:42:32 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/25 17:45:21 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_ERRORS__
#define __MLX_ERRORS__

#include <pre_compiled.h>

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
