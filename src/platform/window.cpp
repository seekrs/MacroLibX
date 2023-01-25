/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 17:36:44 by maldavid          #+#    #+#             */
/*   Updated: 2023/01/25 16:27:23 by maldavid         ###   ########.fr       */
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

		std::vector<Vertex> vertexData = {
			{{0, 0}, {1.f, 0.f, 0.f}},
			{{_width, 0}, {1.f, 0.f, 0.f}},
			{{_width, _height}, {1.f, 0.f, 0.f}},
			{{0, _height}, {1.f, 0.f, 0.}}
		};

		std::vector<uint16_t> indexData = { 0, 1, 2, 2, 3, 0 };

		_vbo.create(sizeof(Vertex) * vertexData.size(), vertexData.data());
		_ibo.create(sizeof(uint16_t) * indexData.size(), indexData.data());

		//_vk_image.create(_width, _height);
		_staging_buffer.create(Buffer::kind::dynamic, sizeof(uint32_t) * 4, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
	}

	bool MLX_Window::beginFrame()
	{
		if(!_renderer->beginFrame())
			return false;
		_proj = glm::ortho<float>(0, _width, 0, _height);
		_renderer->getUniformBuffer()->setData(sizeof(_proj), &_proj);
		vkCmdBindDescriptorSets(_renderer->getActiveCmdBuffer().get(), VK_PIPELINE_BIND_POINT_GRAPHICS, _renderer->getPipeline().getPipelineLayout(), 0, 1, &_renderer->getDescriptorSet().get(), 0, nullptr);
		return true;
	}

	void MLX_Window::pixel_put(int x, int y, int color)
	{
		VkBufferImageCopy region{};
		region.bufferOffset = 0;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;
		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;
		region.imageOffset = {x, y, 0};
		region.imageExtent = {1, 1, 1};

		VkClearColorValue vk_color;
		vk_color.uint32[0] = (color >> 16) & 0xFF;
		vk_color.uint32[1] = (color >> 8) & 0xFF;
		vk_color.uint32[2] = color & 0xFF;
		vk_color.uint32[3] = 0xFF;

		void* map = nullptr;
		_staging_buffer.mapMem(&map, sizeof(vk_color));
			std::memcpy(map, &vk_color, sizeof(vk_color));
		_staging_buffer.unmapMem();

		vkCmdCopyBufferToImage(_renderer->getActiveCmdBuffer().get(), _staging_buffer.get(), _vk_image.get(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
	}

	void MLX_Window::endFrame()
	{
		_vbo.bind(*_renderer);
		_ibo.bind(*_renderer);
		vkCmdDrawIndexed(_renderer->getActiveCmdBuffer().get(), static_cast<uint32_t>(_ibo.getSize() / sizeof(uint16_t)), 1, 0, 0, 0);

		_renderer->endFrame();
	}

	MLX_Window::~MLX_Window()
	{
		_renderer->destroy();
		//_vk_image.destroy();
		_staging_buffer.destroy();
		_vbo.destroy();
		_ibo.destroy();
		if(_win)
			SDL_DestroyWindow(_win);
	}
}
