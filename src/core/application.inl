/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   application.inl                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 21:49:46 by maldavid          #+#    #+#             */
/*   Updated: 2023/04/02 14:56:27 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <core/application.h>

namespace mlx::core
{
	void Application::getMousePos(int* x, int* y) noexcept
	{
		*x = _in.getX();
		*y = _in.getY();
	}

	void Application::mouseMove(void* win_ptr, int x, int y) noexcept
	{
		SDL_WarpMouseInWindow(_graphics[*static_cast<int*>(win_ptr)]->getWindow()->getNativeWindow(), x, y);
		SDL_PumpEvents();
		SDL_FlushEvent(SDL_MOUSEMOTION);
	}

	void Application::getScreenSize(int* w, int* h) noexcept
	{
		SDL_DisplayMode DM;
		SDL_GetDesktopDisplayMode(0, &DM);
		*w = DM.w;
		*h = DM.h;
	}

	void* Application::newGraphicsSuport(std::size_t w, std::size_t h, std::string title)
	{
		_graphics.emplace_back(std::make_unique<GraphicsSupport>(w, h, std::move(title), _graphics.size()));
		return static_cast<void*>(&_graphics.back()->getID());
	}

	void Application::clearGraphicsSupport(void* win_ptr)
	{
		_graphics[*static_cast<int*>(win_ptr)]->clearRenderData();
	}
	
	void Application::destroyGraphicsSupport(void* win_ptr)
	{
		_graphics[*static_cast<int*>(win_ptr)].reset();
	}

	void Application::pixelPut(void* win_ptr, int x, int y, int color) const noexcept
	{
		_graphics[*static_cast<int*>(win_ptr)]->pixelPut(x, y, color);
	}

	void Application::texturePut(void* win_ptr, void* img, int x, int y)
	{
		TextureID id = *static_cast<TextureID*>(img);
		std::shared_ptr<Texture> texture = _texture_lib.getTexture(id);
		_graphics[*static_cast<int*>(win_ptr)]->texturePut(texture, x, y);
	}

	void Application::loopHook(int (*f)(void*), void* param)
	{
		_loop_hook = f;
		_param = param;
	}
	
	void Application::loopEnd() noexcept
	{
		_in.finish();
	}
}
