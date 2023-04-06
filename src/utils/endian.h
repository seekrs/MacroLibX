/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   endian.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 15:33:15 by maldavid          #+#    #+#             */
/*   Updated: 2023/04/06 15:35:25 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_ENDIAN__
#define __MLX_ENDIAN__

namespace mlx
{
	inline bool isSystemBigEndian()
	{
		const int value = 0x01;
		const void* address = static_cast<const void*>(&value);
		const unsigned char* least_significant_address = static_cast<const unsigned char*>(address);
		return (*least_significant_address != 0x01);
	}
}

#endif
