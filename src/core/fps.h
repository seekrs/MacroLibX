/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fps.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 14:53:30 by maldavid          #+#    #+#             */
/*   Updated: 2024/01/18 15:16:06 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_FPS__
#define __MLX_FPS__

#include <cstdint>

namespace mlx
{
	class FpsManager
	{
		public:
			FpsManager() = default;

			void init();
			bool update();
			inline void setMaxFPS(std::uint32_t fps) noexcept { _max_fps = fps; _ns = 1000000000.0 / fps; }

			~FpsManager() = default;

		private:
			double _ns = 1000000000.0 / 1'337'000.0;
			std::uint64_t _timer = 0;
			std::uint64_t _fps_before = 0;
			std::uint64_t _fps_now = 0;
			std::uint32_t _max_fps = 1'337'000;
			std::uint32_t _fps_elapsed_time = 0;
	};
}

#endif
