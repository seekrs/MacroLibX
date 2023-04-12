/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bridge.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 17:35:20 by maldavid          #+#    #+#             */
/*   Updated: 2023/04/12 11:16:52 by maldavid         ###   ########.fr       */
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

	void* mlx_new_window(void* mlx, int w, int h, const char* title)
	{
		return static_cast<mlx::core::Application*>(mlx)->newGraphicsSuport(w, h, title);
	}

	int	mlx_loop_hook(void* mlx, int (*f)(void*), void* param)
	{
		static_cast<mlx::core::Application*>(mlx)->loopHook(f, param);
		return 0;
	}

	int mlx_loop(void* mlx)
	{
		static_cast<mlx::core::Application*>(mlx)->run();
		return 0;
	}

	int mlx_loop_end(void* mlx)
	{
		static_cast<mlx::core::Application*>(mlx)->loopEnd();
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

	int mlx_mouse_move(void* mlx, void* win, int x, int y)
	{
		static_cast<mlx::core::Application*>(mlx)->mouseMove(win, x, y);
		return 0;
	}

	int mlx_mouse_get_pos(void* mlx, int* x, int* y)
	{
		static_cast<mlx::core::Application*>(mlx)->getMousePos(x, y);
		return 0;
	}

	int mlx_on_event(void* mlx, int event, int (*funct_ptr)(int, void*), void* param)
	{
		static_cast<mlx::core::Application*>(mlx)->onEvent(event, funct_ptr, param);
		return 0;
	}

	int mlx_do_key_autorepeaton(void* mlx)
	{
		static_cast<mlx::core::Application*>(mlx)->enableAutoRepeat();
		return 0;
	}

	int mlx_do_key_autorepeatoff(void* mlx)
	{
		static_cast<mlx::core::Application*>(mlx)->disableAutoRepeat();
		return 0;
	}

	void* mlx_new_image(void* mlx, int width, int height)
	{
		return static_cast<mlx::core::Application*>(mlx)->newTexture(width, height);
	}

	char* mlx_get_data_addr(void* mlx, void* img, int* bits_per_pixel, int* size_line, int* endian)
	{
		return static_cast<mlx::core::Application*>(mlx)->mapTexture(img, bits_per_pixel, size_line, endian);
	}

	int mlx_put_image_to_window(void* mlx, void* win, void* img, int x, int y)
	{
		static_cast<mlx::core::Application*>(mlx)->texturePut(win, img, x, y);
		return 0;
	}

	int mlx_destroy_image(void* mlx, void* img)
	{
		static_cast<mlx::core::Application*>(mlx)->destroyTexture(img);
		return 0;
	}

	void* mlx_png_file_to_image(void* mlx, char* filename, int* width, int* height)
	{
		std::filesystem::path file(filename);
		if(file.extension() != ".png")
		{
			mlx::core::error::report(e_kind::error, "PNG loader : not a png file '%s'", filename);
			return nullptr;
		}
		return static_cast<mlx::core::Application*>(mlx)->newStbTexture(filename, width, height);
	}

	void* mlx_jpg_file_to_image(void* mlx, char* filename, int* width, int* height)
	{
		std::filesystem::path file(filename);
		if(file.extension() != ".jpg" && file.extension() != ".jpeg")
		{
			mlx::core::error::report(e_kind::error, "PNG loader : not a jpg file '%s'", filename);
			return nullptr;
		}
		return static_cast<mlx::core::Application*>(mlx)->newStbTexture(filename, width, height);
	}

	void* mlx_bmp_file_to_image(void* mlx, char* filename, int* width, int* height)
	{
		std::filesystem::path file(filename);
		if(file.extension() != ".bmp" && file.extension() != ".dib")
		{
			mlx::core::error::report(e_kind::error, "PNG loader : not a jpg file '%s'", filename);
			return nullptr;
		}
		return static_cast<mlx::core::Application*>(mlx)->newStbTexture(filename, width, height);
	}

	int mlx_pixel_put(void* mlx, void* win, int x, int y, int color)
	{
		static_cast<mlx::core::Application*>(mlx)->pixelPut(win, x, y, color);
		return 0;
	}

	int mlx_string_put(void* mlx, void* win, int x, int y, int color, char* str)
	{
		static_cast<mlx::core::Application*>(mlx)->stringPut(win, x, y, color, str);
		return 0;
	}

	int mlx_clear_window(void* mlx, void* win)
	{
		static_cast<mlx::core::Application*>(mlx)->clearGraphicsSupport(win);
		return 0;
	}

	int mlx_destroy_window(void* mlx, void* win)
	{
		static_cast<mlx::core::Application*>(mlx)->destroyGraphicsSupport(win);
		return 0;
	}

	int mlx_destroy_display(void* mlx)
	{
		delete static_cast<mlx::core::Application*>(mlx);
		mlx::Render_Core::get().destroy();
		return 0;
	}

	int mlx_get_screens_size(void* mlx, int* w, int* h)
	{
		static_cast<mlx::core::Application*>(mlx)->getScreenSize(w, h);
		return 0;
	}
}
