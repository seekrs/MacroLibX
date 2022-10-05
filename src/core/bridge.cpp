/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bridge.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 17:35:20 by maldavid          #+#    #+#             */
/*   Updated: 2022/10/05 18:45:52 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <SDL2/SDL.h>
#include "errors.h"
#include "application.h"

extern "C"
{
	static std::unique_ptr<mlx::core::Application> __main_app = nullptr;

	void mlx_init()
	{
		if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
			mlx::core::error::report(e_kind::fatal_error, "Unable to init the SDL2");
		__main_app.reset(new mlx::core::Application());
	}

	void* mlx_new_window(int w, int h, const char* title)
	{
		if(__main_app == nullptr)
			mlx::core::error::report(e_kind::fatal_error, "You must initialize the mlx before you can create new windows");
		return __main_app->new_window(w, h, title);
	}

	int mlx_loop()
	{
		if(__main_app == nullptr)
			mlx::core::error::report(e_kind::fatal_error, "You must initialize the mlx before you can run the main loop");
		__main_app->run();
	}

	int mlx_mouse_show()
	{
		if(__main_app == nullptr)
			mlx::core::error::report(e_kind::fatal_error, "You must initialize the mlx before you can modify the cursor");
		return SDL_ShowCursor(SDL_ENABLE);
	}

	int mlx_mouse_hide()
	{
		if(__main_app == nullptr)
			mlx::core::error::report(e_kind::fatal_error, "You must initialize the mlx before you can modify the cursor");
		return SDL_ShowCursor(SDL_DISABLE);
	}

	int mlx_mouse_move(void* win_ptr, int x, int y)
	{
		if(__main_app == nullptr)
			mlx::core::error::report(e_kind::fatal_error, "You must initialize the mlx before you can modify the mouse");
	}

	int mlx_mouse_get_pos(void* win_ptr, int* x, int* y)
	{
		if(__main_app == nullptr)
			mlx::core::error::report(e_kind::fatal_error, "You must initialize the mlx before you can access to the mouse");
	}
}
