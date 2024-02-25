/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inputs.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/05 16:30:19 by maldavid          #+#    #+#             */
/*   Updated: 2024/02/23 22:27:30 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inputs.h"
#include <mlx.h>
#include <core/profiler.h>

namespace mlx
{
	void Input::update()
	{
		MLX_PROFILE_FUNCTION();
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

			std::uint32_t id = _event.window.windowID;
			if(_events_hooks.find(id) == _events_hooks.end())
				continue;
			auto& hooks = _events_hooks[id];

			switch(_event.type) 
			{
				case SDL_KEYDOWN:
				{
					if(hooks[MLX_KEYDOWN].hook)
						hooks[MLX_KEYDOWN].hook(_event.key.keysym.scancode, hooks[MLX_KEYDOWN].param);
					break;
				}

				case SDL_KEYUP:
				{
					if(hooks[MLX_KEYUP].hook)
						hooks[MLX_KEYUP].hook(_event.key.keysym.scancode, hooks[MLX_KEYUP].param);
					break;
				}

				case SDL_MOUSEBUTTONDOWN:
				{
					if(hooks[MLX_MOUSEDOWN].hook)
						hooks[MLX_MOUSEDOWN].hook(_event.button.button, hooks[MLX_MOUSEDOWN].param);
					break;
				}

				case SDL_MOUSEBUTTONUP:
				{
					if(hooks[MLX_MOUSEUP].hook)
						hooks[MLX_MOUSEUP].hook(_event.button.button, hooks[MLX_MOUSEUP].param);
					break;
				}

				case SDL_MOUSEWHEEL:
				{
					if(hooks[MLX_MOUSEWHEEL].hook)
					{
						if(_event.wheel.y > 0) // scroll up
							hooks[MLX_MOUSEWHEEL].hook(1, hooks[MLX_MOUSEWHEEL].param);
						else if(_event.wheel.y < 0) // scroll down
							hooks[MLX_MOUSEWHEEL].hook(2, hooks[MLX_MOUSEWHEEL].param);

						if(_event.wheel.x > 0) // scroll right
							hooks[MLX_MOUSEWHEEL].hook(3, hooks[MLX_MOUSEWHEEL].param);
						else if(_event.wheel.x < 0) // scroll left
							hooks[MLX_MOUSEWHEEL].hook(4, hooks[MLX_MOUSEWHEEL].param);
					}
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
								win_hook.hook(5, win_hook.param);
							break;
						}
						case SDL_WINDOWEVENT_LEAVE:
						{
							if(win_hook.hook)
								win_hook.hook(6, win_hook.param);
							break;
						}
						case SDL_WINDOWEVENT_FOCUS_LOST:
						{
							if(win_hook.hook)
								win_hook.hook(7, win_hook.param);
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
