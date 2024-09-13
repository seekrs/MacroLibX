/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bridge.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 17:35:20 by maldavid          #+#    #+#             */
/*   Updated: 2024/09/12 01:29:33 by tdelage          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SDL2/SDL.h>
#include "errors.h"
#include "application.h"
#include <renderer/core/render_core.h>
#include <filesystem>
#include <mlx.h>
#include <core/memory.h>
#include <mlx_profile.h>

static void* __mlx_ptr = nullptr;

#define MLX_CHECK_APPLICATION_POINTER(ptr) \
	if(ptr != __mlx_ptr || ptr == NULL) \
		mlx::core::error::report(e_kind::fatal_error, "invalid mlx pointer passed to '%s'", MLX_FUNC_SIG); \
	else {} // just to avoid issues with possible if-else statements outside this macro

extern "C"
{
	void* mlx_init()
	{
		if(__mlx_ptr != nullptr)
		{
			mlx::core::error::report(e_kind::error, "MLX cannot be initialized multiple times");
			return NULL; // not nullptr for the C compatibility
		}
		mlx::MemManager::get(); // just to initialize the C garbage collector
		mlx::core::Application* app = new mlx::core::Application;
		mlx::Render_Core::get().init();
		if(app == nullptr)
			mlx::core::error::report(e_kind::fatal_error, "Tout a pété");
		__mlx_ptr = static_cast<void*>(app);
		return __mlx_ptr;
	}

	void* mlx_new_window(void* mlx, int w, int h, const char* title)
	{
		MLX_CHECK_APPLICATION_POINTER(mlx);
		if(w <= 0 || h <= 0)
		{
			mlx::core::error::report(e_kind::fatal_error, "invalid window size (%d x %d)", w, h);
			return NULL; // not nullptr for the C compatibility
		}
		return static_cast<mlx::core::Application*>(mlx)->newGraphicsSuport(w, h, title);
	}

	int	mlx_loop_hook(void* mlx, int (*f)(void*), void* param)
	{
		MLX_CHECK_APPLICATION_POINTER(mlx);
		static_cast<mlx::core::Application*>(mlx)->loopHook(f, param);
		return 0;
	}

        void	mlx_set_window_position(void *mlx, void *win, int x, int y)
        {
		MLX_CHECK_APPLICATION_POINTER(mlx);
		static_cast<mlx::core::Application*>(mlx)->setWindowPosition(win, x, y);
        }

	int mlx_loop(void* mlx)
	{
		MLX_CHECK_APPLICATION_POINTER(mlx);
		static_cast<mlx::core::Application*>(mlx)->run();
		return 0;
	}

	int mlx_loop_end(void* mlx)
	{
		MLX_CHECK_APPLICATION_POINTER(mlx);
		static_cast<mlx::core::Application*>(mlx)->loopEnd();
		return 0;
	}

	int mlx_mouse_show()
	{
		SDL_ShowCursor(SDL_ENABLE);
		return 0;
	}

	int mlx_mouse_hide()
	{
		SDL_ShowCursor(SDL_DISABLE);
		return 0;
	}

	int mlx_mouse_move(void* mlx, void* win, int x, int y)
	{
		MLX_CHECK_APPLICATION_POINTER(mlx);
		static_cast<mlx::core::Application*>(mlx)->mouseMove(win, x, y);
		return 0;
	}

	int mlx_mouse_get_pos(void* mlx, int* x, int* y)
	{
		MLX_CHECK_APPLICATION_POINTER(mlx);
		static_cast<mlx::core::Application*>(mlx)->getMousePos(x, y);
		return 0;
	}

	int mlx_on_event(void* mlx, void* win, mlx_event_type event, int (*funct_ptr)(int, void*), void* param)
	{
		MLX_CHECK_APPLICATION_POINTER(mlx);
		static_cast<mlx::core::Application*>(mlx)->onEvent(win, static_cast<int>(event), funct_ptr, param);
		return 0;
	}

	void* mlx_new_image(void* mlx, int width, int height)
	{
		MLX_CHECK_APPLICATION_POINTER(mlx);
		if (width <= 0 || height <= 0)
			mlx::core::error::report(e_kind::fatal_error, "invalid image size (%d x %d)", width, height);
		return static_cast<mlx::core::Application*>(mlx)->newTexture(width, height);
	}

	int mlx_get_image_pixel(void* mlx, void* img, int x, int y)
	{
		MLX_CHECK_APPLICATION_POINTER(mlx);
		int color = static_cast<mlx::core::Application*>(mlx)->getTexturePixel(img, x, y);
		unsigned char color_bits[4];
		color_bits[0] = (color & 0x000000FF);
		color_bits[1] = (color & 0x0000FF00) >> 8;
		color_bits[2] = (color & 0x00FF0000) >> 16;
		color_bits[3] = (color & 0xFF000000) >> 24;
		return *reinterpret_cast<int*>(color_bits);
	}

	void mlx_set_image_pixel(void* mlx, void* img, int x, int y, int color)
	{
		MLX_CHECK_APPLICATION_POINTER(mlx);
		unsigned char color_bits[4];
		color_bits[0] = (color & 0x00FF0000) >> 16;
		color_bits[1] = (color & 0x0000FF00) >> 8;
		color_bits[2] = (color & 0x000000FF);
		color_bits[3] = (color & 0xFF000000) >> 24;
		static_cast<mlx::core::Application*>(mlx)->setTexturePixel(img, x, y, *reinterpret_cast<unsigned int*>(color_bits));
	}

	int mlx_put_image_to_window(void* mlx, void* win, void* img, int x, int y)
	{
		MLX_CHECK_APPLICATION_POINTER(mlx);
		static_cast<mlx::core::Application*>(mlx)->texturePut(win, img, x, y);
		return 0;
	}

	int mlx_destroy_image(void* mlx, void* img)
	{
		MLX_CHECK_APPLICATION_POINTER(mlx);
		static_cast<mlx::core::Application*>(mlx)->destroyTexture(img);
		return 0;
	}

	void* mlx_png_file_to_image(void* mlx, char* filename, int* width, int* height)
	{
		MLX_CHECK_APPLICATION_POINTER(mlx);
		if (filename == nullptr)
			mlx::core::error::report(e_kind::fatal_error, "PNG loader : filename is NULL");
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
		MLX_CHECK_APPLICATION_POINTER(mlx);
		if (filename == nullptr)
			mlx::core::error::report(e_kind::fatal_error, "JPG loader : filename is NULL");
		std::filesystem::path file(filename);
		if(file.extension() != ".jpg" && file.extension() != ".jpeg")
		{
			mlx::core::error::report(e_kind::error, "JPG loader : not a jpg file '%s'", filename);
			return nullptr;
		}
		return static_cast<mlx::core::Application*>(mlx)->newStbTexture(filename, width, height);
	}

	void* mlx_bmp_file_to_image(void* mlx, char* filename, int* width, int* height)
	{
		MLX_CHECK_APPLICATION_POINTER(mlx);
		if (filename == nullptr)
			mlx::core::error::report(e_kind::fatal_error, "BMP loader : filename is NULL");
		std::filesystem::path file(filename);
		if(file.extension() != ".bmp" && file.extension() != ".dib")
		{
			mlx::core::error::report(e_kind::error, "BMP loader : not a bmp file '%s'", filename);
			return nullptr;
		}
		return static_cast<mlx::core::Application*>(mlx)->newStbTexture(filename, width, height);
	}

	int mlx_pixel_put(void* mlx, void* win, int x, int y, int color)
	{
		MLX_CHECK_APPLICATION_POINTER(mlx);
		unsigned char color_bits[4];
		color_bits[0] = (color & 0x00FF0000) >> 16;
		color_bits[1] = (color & 0x0000FF00) >> 8;
		color_bits[2] = (color & 0x000000FF);
		color_bits[3] = (color & 0xFF000000) >> 24;
		static_cast<mlx::core::Application*>(mlx)->pixelPut(win, x, y, *reinterpret_cast<unsigned int*>(color_bits));
		return 0;
	}

	int mlx_string_put(void* mlx, void* win, int x, int y, int color, char* str)
	{
		MLX_CHECK_APPLICATION_POINTER(mlx);
		unsigned char color_bits[4];
		color_bits[0] = (color & 0x00FF0000) >> 16;
		color_bits[1] = (color & 0x0000FF00) >> 8;
		color_bits[2] = (color & 0x000000FF);
		color_bits[3] = (color & 0xFF000000) >> 24;
		static_cast<mlx::core::Application*>(mlx)->stringPut(win, x, y, *reinterpret_cast<unsigned int*>(color_bits), str);
		return 0;
	}

	void mlx_set_font(void* mlx, void* win, char* filepath)
	{
		MLX_CHECK_APPLICATION_POINTER(mlx);
		if (filepath == nullptr)
		{
			mlx::core::error::report(e_kind::error, "Font loader : filepath is NULL");
			return;
		}
		std::filesystem::path file(filepath);
		if(std::strcmp(filepath, "default") != 0 && file.extension() != ".ttf" && file.extension() != ".tte")
		{
			mlx::core::error::report(e_kind::error, "TTF loader : not a truetype font file '%s'", filepath);
			return;
		}
		if(std::strcmp(filepath, "default") == 0)
			static_cast<mlx::core::Application*>(mlx)->loadFont(win, file, 6.f);
		else
			static_cast<mlx::core::Application*>(mlx)->loadFont(win, file, 16.f);
	}

	void mlx_set_font_scale(void* mlx, void* win, char* filepath, float scale)
	{
		MLX_CHECK_APPLICATION_POINTER(mlx);
		if (filepath == nullptr)
		{
			mlx::core::error::report(e_kind::error, "Font loader : filepath is NULL");
			return;
		}
		std::filesystem::path file(filepath);
		if(std::strcmp(filepath, "default") != 0 && file.extension() != ".ttf" && file.extension() != ".tte")
		{
			mlx::core::error::report(e_kind::error, "TTF loader : not a truetype font file '%s'", filepath);
			return;
		}
		static_cast<mlx::core::Application*>(mlx)->loadFont(win, file, scale);
	}

	int mlx_clear_window(void* mlx, void* win)
	{
		MLX_CHECK_APPLICATION_POINTER(mlx);
		static_cast<mlx::core::Application*>(mlx)->clearGraphicsSupport(win);
		return 0;
	}

	int mlx_destroy_window(void* mlx, void* win)
	{
		MLX_CHECK_APPLICATION_POINTER(mlx);
		static_cast<mlx::core::Application*>(mlx)->destroyGraphicsSupport(win);
		return 0;
	}

	int mlx_destroy_display(void* mlx)
	{
		MLX_CHECK_APPLICATION_POINTER(mlx);
		delete static_cast<mlx::core::Application*>(mlx);
		mlx::Render_Core::get().destroy();
		__mlx_ptr = nullptr;
		return 0;
	}

	int mlx_get_screens_size(void* mlx, void* win, int* w, int* h)
	{
		MLX_CHECK_APPLICATION_POINTER(mlx);
		static_cast<mlx::core::Application*>(mlx)->getScreenSize(win, w, h);
		return 0;
	}

	int mlx_set_fps_goal(void* mlx, int fps)
	{
		MLX_CHECK_APPLICATION_POINTER(mlx);
		if(fps < 0)
		{
			mlx::core::error::report(e_kind::error, "You cannot set a negative FPS cap (nice try)");
			fps = -fps;
		}
		if(fps == 0)
		{
			mlx::core::error::report(e_kind::error, "You cannot set a FPS cap to 0 (nice try)");
			return 0;
		}
		static_cast<mlx::core::Application*>(mlx)->setFPSCap(static_cast<std::uint32_t>(fps));
		return 0;
	}
}
