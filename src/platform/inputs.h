/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inputs.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/05 16:27:35 by maldavid          #+#    #+#             */
/*   Updated: 2023/12/11 19:47:20 by vavaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <array>
#include <memory>
#include <vector>
#include <cstdint>
#include <functional>
#include <SDL2/SDL.h>
#include <unordered_map>

#include <mlx_profile.h>

#include "window.h"

namespace mlx
{
	struct Hook
	{
		std::function<int(int, void*)> hook;
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

			inline bool is_running() const noexcept { return !_end; }
			inline constexpr void finish() noexcept { _end = true; }

			inline void addWindow(std::shared_ptr<MLX_Window> window)
			{
				_windows[window->getID()] = window;
				_events_hooks[window->getID()] = {};
			}

			inline void onEvent(uint32_t id, int event, int (*funct_ptr)(int, void*), void* param) noexcept
			{
				_events_hooks[id][event].hook = funct_ptr;
				_events_hooks[id][event].param = param;
			}

			~Input() = default;

		private:
			std::unordered_map<uint32_t, std::shared_ptr<MLX_Window>> _windows;
			std::unordered_map<uint32_t, std::array<Hook, 6>> _events_hooks;
			SDL_Event _event;

			int _x = 0;
			int _y = 0;
			int _xRel = 0;
			int _yRel = 0;

			bool _end = false;
	};
}
