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
		*x = _in->getX();
		*y = _in->getY();
	}

	void Application::mouseMove(void* win, int x, int y) noexcept
	{
		SDL_WarpMouseInWindow(_graphics[*static_cast<int*>(win)]->getWindow()->getNativeWindow(), x, y);
		SDL_PumpEvents();
		SDL_FlushEvent(SDL_MOUSEMOTION);
	}

	void Application::onEvent(void* win, int event, int (*funct_ptr)(int, void*), void* param) noexcept
	{
		_in->onEvent(_graphics[*static_cast<int*>(win)]->getWindow()->getID(), event, funct_ptr, param);
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
		_graphics.emplace_back(std::make_unique<GraphicsSupport>(w, h, title, _graphics.size()));
		_in->addWindow(_graphics.back()->getWindow());
		return static_cast<void*>(&_graphics.back()->getID());
	}

	void Application::clearGraphicsSupport(void* win)
	{
		_graphics[*static_cast<int*>(win)]->clearRenderData();
	}
	
	void Application::destroyGraphicsSupport(void* win)
	{
		_graphics[*static_cast<int*>(win)].reset();
	}

	void Application::pixelPut(void* win, int x, int y, uint32_t color) const noexcept
	{
		_graphics[*static_cast<int*>(win)]->pixelPut(x, y, color);
	}

	void Application::stringPut(void* win, int x, int y, int color, char* str)
	{
		_graphics[*static_cast<int*>(win)]->stringPut(x, y, color, str);
	}

	void Application::loadFont(void* win, const std::filesystem::path& filepath, float scale)
	{
		_graphics[*static_cast<int*>(win)]->loadFont(filepath, scale);
	}

	void Application::texturePut(void* win, void* img, int x, int y)
	{
		if(img == nullptr)
		{
			core::error::report(e_kind::error, "wrong texture (NULL)");
			return;
		}
		Texture* texture = static_cast<Texture*>(img);
		if(!texture->isInit())
			core::error::report(e_kind::error, "trying to put a texture that has been destroyed");
		else
			_graphics[*static_cast<int*>(win)]->texturePut(texture, x, y);
	}

	int Application::getTexturePixel(void* img, int x, int y)
	{
		if(img == nullptr)
		{
			core::error::report(e_kind::error, "wrong texture (NULL)");
			return 0;
		}
		Texture* texture = static_cast<Texture*>(img);
		if(!texture->isInit())
		{
			core::error::report(e_kind::error, "trying to get a pixel from texture that has been destroyed");
			return 0;
		}
		return texture->getPixel(x, y);
	}

	void Application::setTexturePixel(void* img, int x, int y, uint32_t color)
	{
		if(img == nullptr)
		{
			core::error::report(e_kind::error, "wrong texture (NULL)");
			return;
		}
		Texture* texture = static_cast<Texture*>(img);
		if(!texture->isInit())
			core::error::report(e_kind::error, "trying to set a pixel on texture that has been destroyed");
		else
			texture->setPixel(x, y, color);
	}

	void Application::loopHook(int (*f)(void*), void* param)
	{
		_loop_hook = f;
		_param = param;
	}
	
	void Application::loopEnd() noexcept
	{
		_in->finish();
	}
}
