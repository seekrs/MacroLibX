/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventBase.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 17:27:22 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/27 17:31:16 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_BASE_EVENT__
#define __MLX_BASE_EVENT__

namespace mlx
{
	struct EventBase
	{
		virtual std::uint32_t What() const = 0;
	};
}

#endif
