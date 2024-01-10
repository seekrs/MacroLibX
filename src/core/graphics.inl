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

#include <core/graphics.h>
#include <type_traits>

namespace mlx
{
	int& GraphicsSupport::getID() noexcept { return _id; }
	std::shared_ptr<MLX_Window> GraphicsSupport::getWindow() { return _window; }

	void GraphicsSupport::clearRenderData() noexcept
	{
		MLX_PROFILE_FUNCTION();
		_textures_to_render.clear();
		_pixel_put_pipeline.clear();
		_text_put_pipeline->clear();
	}

	void GraphicsSupport::pixelPut(int x, int y, uint32_t color) noexcept
	{
		MLX_PROFILE_FUNCTION();
		_pixel_put_pipeline.setPixel(x, y, color);
	}

	void GraphicsSupport::stringPut(int x, int y, int color, std::string str)
	{
		MLX_PROFILE_FUNCTION();
		_text_put_pipeline->put(x, y, color, str);
	}

	void GraphicsSupport::texturePut(Texture* texture, int x, int y)
	{
		MLX_PROFILE_FUNCTION();
		_textures_to_render.emplace_back(texture, x, y);
		auto it = std::find(_textures_to_render.begin(), _textures_to_render.end() - 1, _textures_to_render.back());
		if(it != _textures_to_render.end() - 1)
			_textures_to_render.erase(it);
	}

	void GraphicsSupport::loadFont(const std::filesystem::path& filepath, float scale)
	{
		MLX_PROFILE_FUNCTION();
		_text_put_pipeline->loadFont(filepath, scale);
	}
}
