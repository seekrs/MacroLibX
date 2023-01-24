/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 17:36:44 by maldavid          #+#    #+#             */
/*   Updated: 2023/01/24 17:57:19 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "window.h"
#include <core/errors.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

namespace mlx
{
	MLX_Window::MLX_Window(std::size_t w, std::size_t h, std::string title, int id) : _id(id), _renderer(new Renderer()), _width(w), _height(h)
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
		bool success = _renderer->beginFrame();
		if(success)
			_vbo.bind(*_renderer);
		_proj = glm::ortho<float>(0, _width, 0, _height);
		_renderer->getUniformBuffer()->setData(sizeof(_proj), &_proj);
		vkCmdBindDescriptorSets(_renderer->getActiveCmdBuffer().get(), VK_PIPELINE_BIND_POINT_GRAPHICS, _renderer->getPipeline().getPipelineLayout(), 0, 1, &_renderer->getDescriptorSet().get(), 0, nullptr);
		return success;
	}

	void MLX_Window::pixel_put(int x, int y, int color)
	{
		static Vertex vert;
		vert.pos = glm::vec2(x, y);
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
