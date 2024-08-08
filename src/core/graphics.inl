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

#include <algorithm>
#include <core/graphics.h>

namespace mlx
{
	int& GraphicsSupport::getID() noexcept { return _id; }
	std::shared_ptr<MLX_Window> GraphicsSupport::getWindow() { return _window; }

	void GraphicsSupport::clearRenderData() noexcept
	{
		MLX_PROFILE_FUNCTION();
		_drawlist.clear();
		_pixel_put_pipeline.clear();
		_text_manager.clear();
		_texture_manager.clear();
	}

	void GraphicsSupport::pixelPut(int x, int y, std::uint32_t color) noexcept
	{
		MLX_PROFILE_FUNCTION();
		_pixel_put_pipeline.setPixel(x, y, color);
	}

	void GraphicsSupport::stringPut(int x, int y, std::uint32_t color, std::string str)
	{
		MLX_PROFILE_FUNCTION();
		std::pair<DrawableResource*, bool> res = _text_manager.registerText(x, y, color, str);
		if(!res.second) // if this is not a completly new text draw
		{
			auto it = std::find(_drawlist.begin(), _drawlist.end(), res.first);
			if(it != _drawlist.end())
				_drawlist.erase(it);
		}
		_drawlist.push_back(res.first);
	}

	void GraphicsSupport::texturePut(Texture* texture, int x, int y)
	{
		MLX_PROFILE_FUNCTION();
		auto res = _texture_manager.registerTexture(texture, x, y);
		if(!res.second) // if this is not a completly new texture draw
		{
			auto it = std::find(_drawlist.begin(), _drawlist.end(), res.first);
			if(it != _drawlist.end())
				_drawlist.erase(it);
		}
		_drawlist.push_back(res.first);
	}

	void GraphicsSupport::loadFont(const std::filesystem::path& filepath, float scale)
	{
		MLX_PROFILE_FUNCTION();
		_text_manager.loadFont(*_renderer, filepath, scale);
	}

	void GraphicsSupport::tryEraseTextureFromManager(Texture* texture) noexcept
	{
		MLX_PROFILE_FUNCTION();
		for(auto it = _drawlist.begin(); it != _drawlist.end();)
		{
			if(_texture_manager.isTextureKnown(texture))
				it = _drawlist.erase(it);
			else
				++it;
		}
		_texture_manager.eraseTextures(texture);
	}
}
