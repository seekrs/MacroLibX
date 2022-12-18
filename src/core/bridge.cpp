/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bridge.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 17:35:20 by maldavid          #+#    #+#             */
/*   Updated: 2022/12/18 22:24:25 by maldavid         ###   ########.fr       */
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

	int mlx_mouse_move(void* win_ptr, int x, int y)
	{
	}

	int mlx_mouse_get_pos(void* win_ptr, int* x, int* y)
	{
	}

	int mlx_pixel_put(void* mlx, void* win_ptr, int x, int y, int color)
	{
		static_cast<mlx::core::Application*>(mlx)->pixel_put(win_ptr, x, y, color);
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
