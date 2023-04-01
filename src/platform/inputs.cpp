/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inputs.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/05 16:30:19 by maldavid          #+#    #+#             */
/*   Updated: 2023/04/01 16:46:13 by maldavid         ###   ########.fr       */
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
			if(_event.window.event == SDL_WINDOWEVENT_CLOSE) 
				_end = true;

			switch(_event.type) 
			{
				case SDL_KEYDOWN:			_keys[_event.key.keysym.scancode] = static_cast<uint8_t>(action::down); break;
				case SDL_KEYUP:				_keys[_event.key.keysym.scancode] = static_cast<uint8_t>(action::up); break;
				case SDL_MOUSEBUTTONDOWN:	_mouse[_event.button.button] = static_cast<uint8_t>(action::down); break;
				case SDL_MOUSEBUTTONUP:		_mouse[_event.button.button] = static_cast<uint8_t>(action::up); break;

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
