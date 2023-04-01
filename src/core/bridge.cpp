/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bridge.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 17:35:20 by maldavid          #+#    #+#             */
/*   Updated: 2023/04/01 17:48:21 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SDL2/SDL.h>
#include "errors.h"
#include "application.h"
#include <renderer/core/render_core.h>

extern "C"
{
	void* mlx_init()
	{
		mlx::Render_Core::get().init();
		return new mlx::core::Application();
	}

	void* mlx_new_window(void* mlx, int w, int h, const char* title)
	{
		return static_cast<mlx::core::Application*>(mlx)->new_window(w, h, title);
	}

	int	mlx_loop_hook(void* mlx, int (*f)(void*), void* param)
	{
		static_cast<mlx::core::Application*>(mlx)->loop_hook(f, param);
		return 0;
	}

	int mlx_loop(void* mlx)
	{
		static_cast<mlx::core::Application*>(mlx)->run();
		return 0;
	}

	int mlx_loop_end(void* mlx)
	{
		static_cast<mlx::core::Application*>(mlx)->loop_end();
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

	int mlx_mouse_move(void* mlx, void* win_ptr, int x, int y)
	{
		static_cast<mlx::core::Application*>(mlx)->mouse_move(win_ptr, x, y);
		return 0;
	}

	int mlx_mouse_get_pos(void* mlx, int* x, int* y)
	{
		static_cast<mlx::core::Application*>(mlx)->get_mouse_pos(x, y);
		return 0;
	}

	int mlx_put_image_to_window(void* mlx_ptr, void* win_ptr, void* img_ptr, int x, int y)
	{
		static_cast<mlx::core::Application*>(mlx_ptr)->texture_put(win_ptr, img_ptr, x, y);
		return 0;
	}

	int mlx_destroy_image(void* mlx_ptr, void* img_ptr)
	{
		static_cast<mlx::core::Application*>(mlx_ptr)->destroy_texture(img_ptr);
		return 0;
	}

	void* mlx_png_file_to_image(void* mlx_ptr, char* filename, int* width, int* height)
	{
		return static_cast<mlx::core::Application*>(mlx_ptr)->new_stb_texture(filename, width, height);
	}

	int mlx_pixel_put(void* mlx, void* win_ptr, int x, int y, int color)
	{
		static_cast<mlx::core::Application*>(mlx)->pixel_put(win_ptr, x, y, color);
		return 0;
	}

	int mlx_clear_window(void* mlx_ptr, void* win_ptr)
	{
		static_cast<mlx::core::Application*>(mlx_ptr)->clear_window(win_ptr);
		return 0;
	}

	int mlx_destroy_window(void* mlx, void* win_ptr)
	{
		static_cast<mlx::core::Application*>(mlx)->destroy_window(win_ptr);
		return 0;
	}

	int mlx_destroy_display(void* mlx)
	{
		delete static_cast<mlx::core::Application*>(mlx);
		mlx::Render_Core::get().destroy();
		return 0;
	}
}
