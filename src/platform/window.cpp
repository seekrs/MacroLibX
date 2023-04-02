/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 17:36:44 by maldavid          #+#    #+#             */
/*   Updated: 2023/04/02 15:39:59 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <platform/window.h>
#include <core/errors.h>

namespace mlx
{
	MLX_Window::MLX_Window(std::size_t w, std::size_t h, std::string title) : _width(w), _height(h)
	{
		_win = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_VULKAN | SDL_WINDOW_SHOWN);
		if(!_win)
			core::error::report(e_kind::fatal_error, std::string("unable to open a new window, ") + SDL_GetError());
	}

	MLX_Window::~MLX_Window()
	{
		if(_win)
			SDL_DestroyWindow(_win);
	}
}
