/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 17:36:44 by maldavid          #+#    #+#             */
/*   Updated: 2023/04/01 15:40:30 by maldavid         ###   ########.fr       */
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

	void MLX_Window::texture_put(std::shared_ptr<Texture> texture, int x, int y)
	{
		if(texture->getSet() == VK_NULL_HANDLE)
			texture->setDescriptor(_renderer->getFragDescriptorSet().duplicate());
		texture->updateSet(0);
		std::vector<VkDescriptorSet> sets;
		sets.push_back(_renderer->getVertDescriptorSet().get());
		sets.push_back(texture->getSet());
		vkCmdBindDescriptorSets(_renderer->getActiveCmdBuffer().get(), VK_PIPELINE_BIND_POINT_GRAPHICS, _renderer->getPipeline().getPipelineLayout(), 0, sets.size(), sets.data(), 0, nullptr);
		texture->render(*_renderer, x, y);
	}

	void MLX_Window::endFrame()
	{
		auto cmd_buff = _renderer->getActiveCmdBuffer().get();

		_renderer->getPixelPutPipeline().present();

		std::vector<VkDescriptorSet> sets;
		sets.push_back(_renderer->getVertDescriptorSet().get());
		sets.push_back(_renderer->getPixelPutPipeline().getDescriptorSet());
		vkCmdBindDescriptorSets(cmd_buff, VK_PIPELINE_BIND_POINT_GRAPHICS, _renderer->getPipeline().getPipelineLayout(), 0, sets.size(), sets.data(), 0, nullptr);
		_renderer->getPixelPutPipeline().render(*_renderer);
		
		_renderer->endFrame();
	}

	MLX_Window::~MLX_Window()
	{
		_renderer->destroy();
		if(_win)
			SDL_DestroyWindow(_win);
	}
}
