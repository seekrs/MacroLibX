/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UUID.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 11:13:23 by maldavid          #+#    #+#             */
/*   Updated: 2024/01/07 01:39:35 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_UUID__
#define __MLX_UUID__

#include <cstdint>
#include <cstddef>

namespace mlx
{
	class UUID
	{
		public:
			UUID();
			UUID(uint64_t uuid);

			inline operator uint64_t() const { return _uuid; }

		private:
			uint64_t _uuid;
	};
}

namespace std
{
	template <typename T> struct hash;

	template<>
	struct hash<mlx::UUID>
	{
		std::size_t operator()(const mlx::UUID& uuid) const
		{
			return static_cast<uint64_t>(uuid);
		}
	};

}

#endif
