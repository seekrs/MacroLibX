/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inputs.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/05 16:27:35 by maldavid          #+#    #+#             */
/*   Updated: 2024/04/22 12:27:12 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <array>
#include <memory>
#include <cstdint>
#include <function.h>
#include <SDL2/SDL.h>
#include <unordered_map>

#include <mlx_profile.h>

#include "window.h"

namespace mlx
{
	struct Hook
	{
		func::function<int(int, void*)> hook;
		void* param = nullptr;
	};

	class Input
	{
		public:
			Input() = default;

			void update();

			inline bool isMouseMoving() const noexcept { return _xRel || _yRel; }

			inline int getX() const noexcept { return _x; }
			inline int getY() const noexcept { return _y; }

			inline int getXRel() const noexcept { return _xRel; }
			inline int getYRel() const noexcept { return _yRel; }

			inline bool isRunning() const noexcept { return !_end; }
			inline constexpr void setRunning(bool running) noexcept { _end = !running; }
			inline constexpr void finish() noexcept { setRunning(false); }

			inline void addWindow(std::shared_ptr<MLX_Window> window)
			{
				_windows[window->getID()] = window;
				_events_hooks[window->getID()] = {};
			}

			inline void onEvent(std::uint32_t id, int event, int (*funct_ptr)(int, void*), void* param) noexcept
			{
				_events_hooks[id][event].hook = funct_ptr;
				_events_hooks[id][event].param = param;
			}

			~Input() = default;

		private:
			std::unordered_map<std::uint32_t, std::shared_ptr<MLX_Window>> _windows;
			std::unordered_map<std::uint32_t, std::array<Hook, 6>> _events_hooks;
			SDL_Event _event;

			int _x = 0;
			int _y = 0;
			int _xRel = 0;
			int _yRel = 0;

			bool _end = false;
	};
}
