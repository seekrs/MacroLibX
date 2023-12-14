/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphics.inl                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 15:13:55 by maldavid          #+#    #+#             */
/*   Updated: 2023/04/02 15:26:16 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "renderer/images/texture.h"
#include <core/graphics.h>
#include <type_traits>

namespace mlx
{
	int& GraphicsSupport::getID() noexcept { return _id; }
	std::shared_ptr<MLX_Window> GraphicsSupport::getWindow() { return _window; }

	void GraphicsSupport::beginRender() noexcept
	{
		if(!_renderer->beginFrame())
			return;
		_proj = glm::ortho<float>(0, _window->getWidth(), 0, _window->getHeight());
		_renderer->getUniformBuffer()->setData(sizeof(_proj), &_proj);
	}

	void GraphicsSupport::clearRenderData() noexcept
	{
		_textures_to_render.clear();
		_pixel_put_pipeline.clear();
		_text_put_pipeline->clear();
	}

	void GraphicsSupport::pixelPut(int x, int y, uint32_t color) noexcept
	{
		_pixel_put_pipeline.setPixel(x, y, color);
	}

	void GraphicsSupport::stringPut(int x, int y, int color, std::string str)
	{
		_text_put_pipeline->put(x, y, color, str);
	}

	void GraphicsSupport::texturePut(Texture* texture, int x, int y)
	{
		_textures_to_render.emplace_back(texture, x, y);
		auto it = std::find(_textures_to_render.begin(), _textures_to_render.end() - 1, _textures_to_render.back());
		if(it != _textures_to_render.end() - 1)
			_textures_to_render.erase(it);
	}

	void GraphicsSupport::loadFont(const std::filesystem::path& filepath, float scale)
	{
		_text_put_pipeline->loadFont(filepath, scale);
	}
}
