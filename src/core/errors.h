/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 17:42:32 by maldavid          #+#    #+#             */
/*   Updated: 2023/12/27 17:21:07 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_ERRORS__
#define __MLX_ERRORS__

#include <string>
#include <mlx_profile.h>

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
