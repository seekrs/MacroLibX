/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inputs.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/05 16:27:35 by maldavid          #+#    #+#             */
/*   Updated: 2022/12/18 01:13:19 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <array>
#include <memory>
#include <vector>
#include <cstdint>
#include <SDL2/SDL.h>

#include "window.h"

namespace mlx
{
	enum class action : uint8_t { up = (1 << 1), down = (1 << 2) };

	class Input
	{
		public:
			Input(const std::vector<std::shared_ptr<MLX_Window>>& wins);

			void update();

			inline bool getInKey(const SDL_Scancode key, action type = action::down) const noexcept { return _keys[key] & static_cast<uint8_t>(type); }

			inline bool getInMouse(const uint8_t button, action type = action::down) const noexcept { return _mouse[button] & static_cast<uint8_t>(type); }
			inline bool isMouseMoving() const noexcept { return _xRel || _yRel; }

			inline int getX() const noexcept { return _x; }
			inline int getY() const noexcept { return _y; }

			inline int getXRel() const noexcept { return _xRel; }
			inline int getYRel() const noexcept { return _yRel; }

			inline bool is_running() const noexcept { return !_end; }
			inline constexpr void finish() noexcept { _end = true; }

			~Input() = default;

		private:
			SDL_Event _event;
			std::array<uint8_t, SDL_NUM_SCANCODES> _keys;
			std::array<uint8_t, 8> _mouse;
			std::vector<std::shared_ptr<MLX_Window>> _wins;

			int _x = 0;
			int _y = 0;
			int _xRel = 0;
			int _yRel = 0;

			bool _end = false;
	};
}
