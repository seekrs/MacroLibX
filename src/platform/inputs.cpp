/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inputs.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/05 16:30:19 by maldavid          #+#    #+#             */
/*   Updated: 2023/08/28 10:49:03 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inputs.h"
#include <mlx.h>
#include <cstring>

namespace mlx
{
	Input::Input()
	{
		std::memset(_keys.data(), 0, SDL_NUM_SCANCODES);
		std::memset(_mouse.data(), 0, 8);
	}

	void Input::update()
	{
		_xRel = 0;
		_yRel = 0;

		while(SDL_PollEvent(&_event))
		{
			if(_event.type == SDL_MOUSEMOTION) 
			{
				_x = _event.motion.x;
				_y = _event.motion.y;

				_xRel = _event.motion.xrel;
				_yRel = _event.motion.yrel;
			}

			uint32_t id = _event.window.windowID;
			if(!_events_hooks.count(id))
				continue;
			auto& hooks = _events_hooks[id];

			switch(_event.type) 
			{
				case SDL_KEYDOWN:
				{
					_keys[_event.key.keysym.scancode] = static_cast<uint8_t>(action::down);
					if(hooks[MLX_KEYDOWN].hook)
						hooks[MLX_KEYDOWN].hook(_event.key.keysym.scancode, hooks[MLX_KEYDOWN].param);
					break;
				}

				case SDL_KEYUP:
				{
					_keys[_event.key.keysym.scancode] = static_cast<uint8_t>(action::up);
					if(hooks[MLX_KEYUP].hook)
						hooks[MLX_KEYUP].hook(_event.key.keysym.scancode, hooks[MLX_KEYUP].param);
					break;
				}

				case SDL_MOUSEBUTTONDOWN:
				{
					_mouse[_event.button.button] = static_cast<uint8_t>(action::down);
					if(hooks[MLX_MOUSEDOWN].hook)
						hooks[MLX_MOUSEDOWN].hook(_event.button.button, hooks[MLX_MOUSEDOWN].param);
					break;
				}

				case SDL_MOUSEBUTTONUP:
				{
					_mouse[_event.button.button] = static_cast<uint8_t>(action::up);
					if(hooks[MLX_MOUSEUP].hook)
						hooks[MLX_MOUSEUP].hook(_event.button.button, hooks[MLX_MOUSEUP].param);
					break;
				}

				case SDL_WINDOWEVENT:
				{
					auto& win_hook = hooks[MLX_WINDOW_EVENT];
					switch(_event.window.event)
					{
						case SDL_WINDOWEVENT_CLOSE:
						{
							if(win_hook.hook)
								win_hook.hook(0, win_hook.param);
							break;
						}
						case SDL_WINDOWEVENT_MOVED:
						{
							if(win_hook.hook)
								win_hook.hook(1, win_hook.param);
							break;
						}
						case SDL_WINDOWEVENT_MINIMIZED:
						{
							if(win_hook.hook)
								win_hook.hook(2, win_hook.param);
							break;
						}
						case SDL_WINDOWEVENT_MAXIMIZED:
						{
							if(win_hook.hook)
								win_hook.hook(3, win_hook.param);
							break;
						}
						case SDL_WINDOWEVENT_ENTER:
						{
							if(win_hook.hook)
								win_hook.hook(4, win_hook.param);
							break;
						}
						case SDL_WINDOWEVENT_FOCUS_GAINED:
						{
							if(win_hook.hook)
								win_hook.hook(4, win_hook.param);
							break;
						}
						case SDL_WINDOWEVENT_LEAVE:
						{
							if(win_hook.hook)
								win_hook.hook(5, win_hook.param);
							break;
						}
						case SDL_WINDOWEVENT_FOCUS_LOST:
						{
							if(win_hook.hook)
								win_hook.hook(4, win_hook.param);
							break;
						}

						default : break;
					}
					break;
				}

				default: break;
			}
		}
	}
}
