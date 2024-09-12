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

#include <SDL2/SDL_video.h>
#include <algorithm>
#include <core/application.h>

#define CHECK_WINDOW_PTR(win) \
	if(win == nullptr) \
	{ \
		core::error::report(e_kind::error, "invalid window ptr (NULL)"); \
		return; \
	} \
	else if(*static_cast<int*>(win) < 0 || *static_cast<int*>(win) > static_cast<int>(_graphics.size()))\
	{ \
		core::error::report(e_kind::error, "invalid window ptr"); \
		return; \
	} else {}

#define CHECK_IMAGE_PTR(img, retval) \
	if(img == nullptr) \
	{ \
		core::error::report(e_kind::error, "invalid image ptr (NULL)"); \
		retval; \
	} \
	else if(std::find_if(_textures.begin(), _textures.end(), [=](const Texture& texture) \
			{ \
				return &texture == img; \
			}) == _textures.end()) \
	{ \
		core::error::report(e_kind::error, "invalid image ptr"); \
		retval; \
	} else {}

namespace mlx::core
{
	void Application::getMousePos(int* x, int* y) noexcept
	{
		*x = _in->getX();
		*y = _in->getY();
	}

	void Application::mouseMove(void* win, int x, int y) noexcept
	{
		CHECK_WINDOW_PTR(win);
		if(!_graphics[*static_cast<int*>(win)]->hasWindow())
		{
			error::report(e_kind::warning, "trying to move the mouse relative to a window that is targeting an image and not a real window, this is not allowed (move ignored)");
			return;
		}
		SDL_WarpMouseInWindow(_graphics[*static_cast<int*>(win)]->getWindow()->getNativeWindow(), x, y);
		SDL_PumpEvents();
	}

	void Application::onEvent(void* win, int event, int (*funct_ptr)(int, void*), void* param) noexcept
	{
		CHECK_WINDOW_PTR(win);
		if(!_graphics[*static_cast<int*>(win)]->hasWindow())
		{
			error::report(e_kind::warning, "trying to add event hook for a window that is targeting an image and not a real window, this is not allowed (hook ignored)");
			return;
		}
		_in->onEvent(_graphics[*static_cast<int*>(win)]->getWindow()->getID(), event, funct_ptr, param);
	}

	void Application::setWindowPosition(void* win, int x, int y)
	{
		CHECK_WINDOW_PTR(win);
		if(!_graphics[*static_cast<int*>(win)]->hasWindow())
		{
			error::report(e_kind::warning, "trying to move a window that is targeting an image and not a real window, this is not allowed");
			return;
		}
		SDL_SetWindowPosition(_graphics[*static_cast<int*>(win)]->getWindow()->getNativeWindow(), x, y);
        }

	void Application::getScreenSize(void* win, int* w, int* h) noexcept
	{
		CHECK_WINDOW_PTR(win);
		SDL_DisplayMode DM;
		SDL_GetDesktopDisplayMode(SDL_GetWindowDisplayIndex(_graphics[*static_cast<int*>(win)]->getWindow()->getNativeWindow()), &DM);
		*w = DM.w;
		*h = DM.h;
	}

	void Application::setFPSCap(std::uint32_t fps) noexcept
	{
		_fps.setMaxFPS(fps);
	}

	void* Application::newGraphicsSuport(std::size_t w, std::size_t h, const char* title)
	{
		MLX_PROFILE_FUNCTION();
		auto it = std::find_if(_textures.begin(), _textures.end(), [=](const Texture& texture)
		{
			return &texture == reinterpret_cast<Texture*>(const_cast<char*>(title));
		});
		if(it != _textures.end())
			_graphics.emplace_back(std::make_unique<GraphicsSupport>(w, h, reinterpret_cast<Texture*>(const_cast<char*>(title)), _graphics.size()));
		else
		{
			if(title == NULL)
			{
				core::error::report(e_kind::fatal_error, "invalid window title (NULL)");
				return nullptr;
			}
			_graphics.emplace_back(std::make_unique<GraphicsSupport>(w, h, title, _graphics.size()));
			_in->addWindow(_graphics.back()->getWindow());
		}
		return static_cast<void*>(&_graphics.back()->getID());
	}

	void Application::clearGraphicsSupport(void* win)
	{
		MLX_PROFILE_FUNCTION();
		CHECK_WINDOW_PTR(win);
		_graphics[*static_cast<int*>(win)]->clearRenderData();
	}

	void Application::destroyGraphicsSupport(void* win)
	{
		MLX_PROFILE_FUNCTION();
		CHECK_WINDOW_PTR(win);
		_graphics[*static_cast<int*>(win)].reset();
	}

	void Application::pixelPut(void* win, int x, int y, std::uint32_t color) const noexcept
	{
		MLX_PROFILE_FUNCTION();
		CHECK_WINDOW_PTR(win);
		_graphics[*static_cast<int*>(win)]->pixelPut(x, y, color);
	}

	void Application::stringPut(void* win, int x, int y, std::uint32_t color, char* str)
	{
		MLX_PROFILE_FUNCTION();
		CHECK_WINDOW_PTR(win);
		if(str == nullptr)
		{
			core::error::report(e_kind::error, "wrong text (NULL)");
			return;
		}
		if(std::strlen(str) == 0)
		{
			core::error::report(e_kind::warning, "trying to put an empty text");
			return;
		}
		_graphics[*static_cast<int*>(win)]->stringPut(x, y, color, str);
	}

	void Application::loadFont(void* win, const std::filesystem::path& filepath, float scale)
	{
		MLX_PROFILE_FUNCTION();
		CHECK_WINDOW_PTR(win);
		_graphics[*static_cast<int*>(win)]->loadFont(filepath, scale);
	}

	void Application::texturePut(void* win, void* img, int x, int y)
	{
		MLX_PROFILE_FUNCTION();
		CHECK_WINDOW_PTR(win);
		CHECK_IMAGE_PTR(img, return);
		Texture* texture = static_cast<Texture*>(img);
		if(!texture->isInit())
			core::error::report(e_kind::error, "trying to put a texture that has been destroyed");
		else
			_graphics[*static_cast<int*>(win)]->texturePut(texture, x, y);
	}

	int Application::getTexturePixel(void* img, int x, int y)
	{
		MLX_PROFILE_FUNCTION();
		CHECK_IMAGE_PTR(img, return 0);
		Texture* texture = static_cast<Texture*>(img);
		if(!texture->isInit())
		{
			core::error::report(e_kind::error, "trying to get a pixel from texture that has been destroyed");
			return 0;
		}
		return texture->getPixel(x, y);
	}

	void Application::setTexturePixel(void* img, int x, int y, std::uint32_t color)
	{
		MLX_PROFILE_FUNCTION();
		CHECK_IMAGE_PTR(img, return);
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
