/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bridge.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 17:35:20 by maldavid          #+#    #+#             */
/*   Updated: 2023/04/25 15:23:05 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SDL2/SDL.h>
#include "errors.h"
#include "application.h"
#include <renderer/core/render_core.h>
#include <filesystem>

extern "C"
{
	void* mlx_init()
	{
		mlx::Render_Core::get().init();
		return new mlx::core::Application();
	}

	void* mlx_new_window(mlx::core::Application* mlx, int w, int h, const char* title)
	{
		return mlx->newGraphicsSuport(w, h, title);
	}

	int	mlx_loop_hook(mlx::core::Application* mlx, int (*f)(void*), void* param)
	{
		mlx->loopHook(f, param);
		return 0;
	}

	int mlx_loop(mlx::core::Application* mlx)
	{
		mlx->run();
		return 0;
	}

	int mlx_loop_end(mlx::core::Application* mlx)
	{
		mlx->loopEnd();
		return 0;
	}

	int mlx_mouse_show()
	{
		return SDL_ShowCursor(SDL_ENABLE);
	}

	int mlx_mouse_hide()
	{
		return SDL_ShowCursor(SDL_DISABLE);
	}

	int mlx_mouse_move(mlx::core::Application* mlx, void* win, int x, int y)
	{
		mlx->mouseMove(win, x, y);
		return 0;
	}

	int mlx_mouse_get_pos(mlx::core::Application* mlx, int* x, int* y)
	{
		mlx->getMousePos(x, y);
		return 0;
	}

	int mlx_on_event(mlx::core::Application* mlx, void* win, int event, int (*funct_ptr)(int, void*), void* param)
	{
		mlx->onEvent(win, event, funct_ptr, param);
		return 0;
	}

	void* mlx_new_image(mlx::core::Application* mlx, int width, int height)
	{
		return mlx->newTexture(width, height);
	}

	int mlx_get_image_pixel(mlx::core::Application* mlx, void* img, int x, int y)
	{
		return mlx->getTexturePixel(img, x, y);
	}

	void mlx_set_image_pixel(mlx::core::Application* mlx, void* img, int x, int y, int color)
	{
		unsigned char color_bits[4];
		color_bits[0] = (color & 0x00FF0000) >> 16;
		color_bits[1] = (color & 0x0000FF00) >> 8;
		color_bits[2] = color & 0x000000FF;
		color_bits[3] = 0xFF;
		mlx->setTexturePixel(img, x, y, *reinterpret_cast<unsigned int*>(color_bits));
	}

	int mlx_put_image_to_window(mlx::core::Application* mlx, void* win, void* img, int x, int y)
	{
		mlx->texturePut(win, img, x, y);
		return 0;
	}

	int mlx_destroy_image(mlx::core::Application* mlx, void* img)
	{
		mlx->destroyTexture(img);
		return 0;
	}

	void* mlx_png_file_to_image(mlx::core::Application* mlx, char* filename, int* width, int* height)
	{
		std::filesystem::path file(filename);
		if(file.extension() != ".png")
		{
			mlx::core::error::report(e_kind::error, "PNG loader : not a png file '%s'", filename);
			return nullptr;
		}
		return mlx->newStbTexture(filename, width, height);
	}

	void* mlx_jpg_file_to_image(mlx::core::Application* mlx, char* filename, int* width, int* height)
	{
		std::filesystem::path file(filename);
		if(file.extension() != ".jpg" && file.extension() != ".jpeg")
		{
			mlx::core::error::report(e_kind::error, "PNG loader : not a jpg file '%s'", filename);
			return nullptr;
		}
		return mlx->newStbTexture(filename, width, height);
	}

	void* mlx_bmp_file_to_image(mlx::core::Application* mlx, char* filename, int* width, int* height)
	{
		std::filesystem::path file(filename);
		if(file.extension() != ".bmp" && file.extension() != ".dib")
		{
			mlx::core::error::report(e_kind::error, "PNG loader : not a jpg file '%s'", filename);
			return nullptr;
		}
		return mlx->newStbTexture(filename, width, height);
	}

	int mlx_pixel_put(mlx::core::Application* mlx, void* win, int x, int y, int color)
	{
		unsigned char color_bits[4];
		color_bits[0] = (color & 0x00FF0000) >> 16;
		color_bits[1] = (color & 0x0000FF00) >> 8;
		color_bits[2] = color & 0x000000FF;
		color_bits[3] = 0xFF;
		mlx->pixelPut(win, x, y, *reinterpret_cast<unsigned int*>(color_bits));
		return 0;
	}

	int mlx_string_put(mlx::core::Application* mlx, void* win, int x, int y, int color, char* str)
	{
		unsigned char color_bits[4];
		color_bits[0] = (color & 0x00FF0000) >> 16;
		color_bits[1] = (color & 0x0000FF00) >> 8;
		color_bits[2] = color & 0x000000FF;
		color_bits[3] = 0xFF;
		mlx->stringPut(win, x, y, *reinterpret_cast<unsigned int*>(color_bits), str);
		return 0;
	}

	int mlx_clear_window(mlx::core::Application* mlx, void* win)
	{
		mlx->clearGraphicsSupport(win);
		return 0;
	}

	int mlx_destroy_window(mlx::core::Application* mlx, void* win)
	{
		mlx->destroyGraphicsSupport(win);
		return 0;
	}

	int mlx_destroy_display(mlx::core::Application* mlx)
	{
		delete mlx;
		mlx::Render_Core::get().destroy();
		return 0;
	}

	int mlx_get_screens_size(mlx::core::Application* mlx, int* w, int* h)
	{
		mlx->getScreenSize(w, h);
		return 0;
	}
}
