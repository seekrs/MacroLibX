/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 17:36:44 by maldavid          #+#    #+#             */
/*   Updated: 2022/12/19 00:46:55 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "window.h"
#include <core/errors.h>

namespace mlx
{
	MLX_Window::MLX_Window(std::size_t w, std::size_t h, std::string title, int id) : _id(id), _renderer(new Renderer())
	{
		_win = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_VULKAN | SDL_WINDOW_SHOWN);
		if(!_win)
			core::error::report(e_kind::fatal_error, std::string("Unable to open a new window, ") + SDL_GetError());
		_renderer->setWindow(this);
		_renderer->init();
		_vbo.create(sizeof(Vertex));
	}

	bool MLX_Window::beginFrame()
	{
		return _renderer->beginFrame();
	}

	void MLX_Window::pixel_put(int x, int y, int color)
	{
		uint32_t ucolor = color;
		Vertex vert;
		vert.pos = glm::vec2(x, y);
		vert.color.r = (ucolor >> 16) & 0xFF;
		vert.color.g = (ucolor >> 8) & 0xFF;
		vert.color.b = ucolor & 0xFF;
		_vbo.setData(sizeof(Vertex), &vert);

		_vbo.bind(*_renderer);
		vkCmdDraw(_renderer->getActiveCmdBuffer().get(), 1, 1, 0, 0);
	}

	void MLX_Window::endFrame()
	{
		_renderer->endFrame();
	}

	MLX_Window::~MLX_Window()
	{
		_vbo.destroy();
		_renderer->destroy();
		if(_win)
			SDL_DestroyWindow(_win);
	}
}
