/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inputs.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/05 16:30:19 by maldavid          #+#    #+#             */
/*   Updated: 2023/04/12 11:31:28 by maldavid         ###   ########.fr       */
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

		if(!_auto_repeat)
			std::memset(_keys.data(), 0, SDL_NUM_SCANCODES);
		
		while(SDL_PollEvent(&_event))
		{
			if(_event.window.event == SDL_WINDOWEVENT_CLOSE) 
				_end = true;

			switch(_event.type) 
			{
				case SDL_KEYDOWN:
				{
					_keys[_event.key.keysym.scancode] = static_cast<uint8_t>(action::down);
					if(_events_hooks[0].hook)
						_events_hooks[0].hook(_event.key.keysym.scancode, _events_hooks[0].param);
					break;
				}

				case SDL_KEYUP:
				{
					_keys[_event.key.keysym.scancode] = static_cast<uint8_t>(action::up);
					if(_events_hooks[1].hook)
						_events_hooks[1].hook(_event.key.keysym.scancode, _events_hooks[1].param);
					break;
				}

				case SDL_MOUSEBUTTONDOWN:
				{
					_mouse[_event.button.button] = static_cast<uint8_t>(action::down);
					if(_events_hooks[2].hook)
						_events_hooks[2].hook(_event.button.button, _events_hooks[2].param);
					break;
				}

				case SDL_MOUSEBUTTONUP:
				{
					_mouse[_event.button.button] = static_cast<uint8_t>(action::up);
					if(_events_hooks[3].hook)
						_events_hooks[3].hook(_event.button.button, _events_hooks[3].param);
					break;
				}

				case SDL_WINDOWEVENT:
				{
					switch(_event.window.event)
					{
						//case

						default : break;
					}
					break;
				}

				default: break;
			}

			if(_event.type == SDL_MOUSEMOTION) 
			{
				_x = _event.motion.x;
				_y = _event.motion.y;

				_xRel = _event.motion.xrel;
				_yRel = _event.motion.yrel;
			}
		}
	}
}
