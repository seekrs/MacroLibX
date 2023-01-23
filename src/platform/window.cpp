/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 17:36:44 by maldavid          #+#    #+#             */
/*   Updated: 2023/01/22 18:53:41 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "window.h"
#include <core/errors.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

namespace mlx
{
	static glm::mat4 proj = glm::mat4(1.0);

	MLX_Window::MLX_Window(std::size_t w, std::size_t h, std::string title, int id) : _id(id), _renderer(new Renderer()), _width(w), _height(h)
	{
		_win = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_VULKAN | SDL_WINDOW_SHOWN);
		if(!_win)
			core::error::report(e_kind::fatal_error, std::string("Unable to open a new window, ") + SDL_GetError());
		_renderer->setWindow(this);
		_renderer->init();
		_vbo.create(sizeof(Vertex));

		proj = glm::ortho<float>(0, _width, 0, _height);
	}

	bool MLX_Window::beginFrame()
	{
		bool success = _renderer->beginFrame();
		if(success)
			_vbo.bind(*_renderer);
		return success;
	}

	void MLX_Window::pixel_put(int x, int y, int color)
	{
		Vertex vert;
		float xf = glm::vec4(glm::vec4((float)x, (float)y, 0.0, 0.0) * proj).x;
		float yf = glm::vec4(glm::vec4((float)x, (float)y, 0.0, 0.0) * proj).y;

		std::cout << xf << " " << yf << "                                        " << std::endl;
		vert.pos = glm::vec2(xf, yf);
		vert.color.r = (color >> 16) & 0xFF;
		vert.color.g = (color >> 8) & 0xFF;
		vert.color.b = color & 0xFF;
		_vbo.setData(sizeof(Vertex), &vert);

		vkCmdDraw(_renderer->getActiveCmdBuffer().get(), 1, 1, 0, 0);
	}

	void MLX_Window::endFrame()
	{
		_renderer->endFrame();
	}

	MLX_Window::~MLX_Window()
	{
		_renderer->destroy();
		_vbo.destroy();
		if(_win)
			SDL_DestroyWindow(_win);
	}
}
