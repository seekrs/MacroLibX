/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 17:36:44 by maldavid          #+#    #+#             */
/*   Updated: 2022/12/18 01:12:05 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "window.h"
#include <core/errors.h>

namespace mlx
{
	MLX_Window::MLX_Window(std::size_t w, std::size_t h, std::string title, int id) : _id(id)
	{
		_win = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, /*SDL_WINDOW_VULKAN |*/ SDL_WINDOW_SHOWN);
		if(!_win)
			core::error::report(e_kind::fatal_error, std::string("Unable to open a new window, ") + SDL_GetError());
	}

	MLX_Window::~MLX_Window()
	{
		if(_win)
			SDL_DestroyWindow(_win);
	}
}
