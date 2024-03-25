/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UUID.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 11:13:23 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/25 17:43:58 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_UUID__
#define __MLX_UUID__

#include <pre_compiled.h>

namespace mlx
{
	class UUID
	{
		public:
			UUID();
			UUID(std::uint64_t uuid);

			inline operator std::uint64_t() const { return _uuid; }

		private:
			std::uint64_t _uuid;
	};
}

#endif
