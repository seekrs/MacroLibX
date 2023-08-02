/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inputs.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/05 16:30:19 by maldavid          #+#    #+#             */
/*   Updated: 2023/04/19 12:14:38 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inputs.h"
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

			switch(_event.type) 
			{
				case SDL_KEYDOWN:
				{
					_keys[_event.key.keysym.scancode] = static_cast<uint8_t>(action::down);
					if(_events_hooks[id][0].hook)
						_events_hooks[id][0].hook(_event.key.keysym.scancode, _events_hooks[id][0].param);
					break;
				}

				case SDL_KEYUP:
				{
					_keys[_event.key.keysym.scancode] = static_cast<uint8_t>(action::up);
					if(_events_hooks[id][1].hook)
						_events_hooks[id][1].hook(_event.key.keysym.scancode, _events_hooks[id][1].param);
					break;
				}

				case SDL_MOUSEBUTTONDOWN:
				{
					_mouse[_event.button.button] = static_cast<uint8_t>(action::down);
					if(_events_hooks[id][2].hook)
						_events_hooks[id][2].hook(_event.button.button, _events_hooks[id][2].param);
					break;
				}

				case SDL_MOUSEBUTTONUP:
				{
					_mouse[_event.button.button] = static_cast<uint8_t>(action::up);
					if(_events_hooks[id][3].hook)
						_events_hooks[id][3].hook(_event.button.button, _events_hooks[id][3].param);
					break;
				}

				case SDL_WINDOWEVENT:
				{
					switch(_event.window.event)
					{
						case SDL_WINDOWEVENT_CLOSE:
						{
							if(_events_hooks[id][4].hook)
								_events_hooks[id][4].hook(0, _events_hooks[id][4].param);
							break;
						}
						case SDL_WINDOWEVENT_MOVED:
						{
							if(_events_hooks[id][4].hook)
								_events_hooks[id][4].hook(1, _events_hooks[id][4].param);
							break;
						}
						case SDL_WINDOWEVENT_MINIMIZED:
						{
							if(_events_hooks[id][4].hook)
								_events_hooks[id][4].hook(2, _events_hooks[id][4].param);
							break;
						}
						case SDL_WINDOWEVENT_MAXIMIZED:
						{
							if(_events_hooks[id][4].hook)
								_events_hooks[id][4].hook(3, _events_hooks[id][4].param);
							break;
						}
						case SDL_WINDOWEVENT_ENTER:
						{
							if(_events_hooks[id][4].hook)
								_events_hooks[id][4].hook(4, _events_hooks[id][4].param);
							break;
						}
						case SDL_WINDOWEVENT_FOCUS_GAINED:
						{
							if(_events_hooks[id][4].hook)
								_events_hooks[id][4].hook(4, _events_hooks[id][4].param);
							break;
						}
						case SDL_WINDOWEVENT_LEAVE:
						{
							if(_events_hooks[id][4].hook)
								_events_hooks[id][4].hook(5, _events_hooks[id][4].param);
							break;
						}
						case SDL_WINDOWEVENT_FOCUS_LOST:
						{
							if(_events_hooks[id][4].hook)
								_events_hooks[id][4].hook(4, _events_hooks[id][4].param);
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
