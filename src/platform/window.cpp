/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 17:36:44 by maldavid          #+#    #+#             */
/*   Updated: 2023/03/31 20:30:14 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "window.h"
#include <core/errors.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <cstring>

namespace mlx
{
	MLX_Window::MLX_Window(std::size_t w, std::size_t h, std::string title, int id) : _id(id), _renderer(new Renderer), _width(w), _height(h)
	{
		_win = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_VULKAN | SDL_WINDOW_SHOWN);
		if(!_win)
			core::error::report(e_kind::fatal_error, std::string("unable to open a new window, ") + SDL_GetError());
		_renderer->setWindow(this);
		_renderer->init();
		_renderer->getPixelPutPipeline().init(w, h, *_renderer);

		std::vector<Vertex> vertexData = {
			{{0, 0},			{1.f, 0.f, 0.f},	{0.0f, 0.0f}},
			{{_width, 0},		{1.f, 0.f, 0.f},	{1.0f, 0.0f}},
			{{_width, _height},	{1.f, 0.f, 0.f},	{1.0f, 1.0f}},
			{{0, _height},		{1.f, 0.f, 0.},		{0.0f, 1.0f}}
		};

		std::vector<uint16_t> indexData = { 0, 1, 2, 2, 3, 0 };

		_vbo.create(sizeof(Vertex) * vertexData.size(), vertexData.data());
		_ibo.create(sizeof(uint16_t) * indexData.size(), indexData.data());
	}

	bool MLX_Window::beginFrame()
	{
		if(!_renderer->beginFrame())
			return false;
		_proj = glm::ortho<float>(0, _width, 0, _height);
		_renderer->getUniformBuffer()->setData(sizeof(_proj), &_proj);
		return true;
	}

	void MLX_Window::pixel_put(int x, int y, int color)
	{
		_renderer->getPixelPutPipeline().setPixel(x, y, color);
	}

	void MLX_Window::endFrame()
	{
		auto cmd_buff = _renderer->getActiveCmdBuffer().get();

		_renderer->getPixelPutPipeline().present();

		std::vector<VkDescriptorSet> sets;
		sets.push_back(_renderer->getVertDescriptorSet().get());
		sets.push_back(_renderer->getPixelPutPipeline().getDescriptorSet());

		vkCmdBindDescriptorSets(cmd_buff, VK_PIPELINE_BIND_POINT_GRAPHICS, _renderer->getPipeline().getPipelineLayout(), 0, sets.size(), sets.data(), 0, nullptr);
		
		_vbo.bind(*_renderer);
		_ibo.bind(*_renderer);

		vkCmdDrawIndexed(cmd_buff, static_cast<uint32_t>(_ibo.getSize() / sizeof(uint16_t)), 1, 0, 0, 0);

		_renderer->endFrame();
	}

	MLX_Window::~MLX_Window()
	{
		_renderer->destroy();
		_staging_buffer.destroy();
		_vbo.destroy();
		_ibo.destroy();
		if(_win)
			SDL_DestroyWindow(_win);
	}
}
