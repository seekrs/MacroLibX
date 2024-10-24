#include <PreCompiled.h>

#include <Core/Application.h>
#include <Renderer/RenderCore.h>
#include <mlx.h>
#include <Core/Memory.h>

static void* __mlx_ptr = nullptr;

#ifndef DISABLE_ALL_SAFETIES
	#define MLX_CHECK_APPLICATION_POINTER(ptr) \
		if(ptr != __mlx_ptr || ptr == NULL) \
			mlx::FatalError("invalid mlx pointer passed to '%'", MLX_FUNC_SIG); \
		else {} // just to avoid issues with possible if-else statements outside this macro
#else
	#define MLX_CHECK_APPLICATION_POINTER(ptr)
#endif

extern "C"
{
	void* mlx_init()
	{
		if(__mlx_ptr != nullptr)
		{
			mlx::Error("MLX cannot be initialized multiple times");
			return nullptr;
		}
		mlx::MemManager::Get(); // just to initialize the C garbage collector
		mlx::Application* app = new mlx::Application;
		if(app == nullptr)
			mlx::FatalError("Tout a pété");
		__mlx_ptr = static_cast<void*>(app);
		return __mlx_ptr;
	}

	void* mlx_new_window(void* mlx, int w, int h, const char* title)
	{
		MLX_CHECK_APPLICATION_POINTER(mlx);
		if(w <= 0 || h <= 0)
		{
			mlx::FatalError("invalid window size (%d x %d)", w, h);
			return NULL; // not nullptr for the C compatibility
		}
		return static_cast<mlx::Application*>(mlx)->NewGraphicsSuport(w, h, title);
	}

	void mlx_set_window_position(void *mlx, void *win, int x, int y)
	{
		MLX_CHECK_APPLICATION_POINTER(mlx);
		static_cast<mlx::Application*>(mlx)->SetGraphicsSupportPosition(win, x, y);
	}

	int	mlx_loop_hook(void* mlx, int (*f)(void*), void* param)
	{
		MLX_CHECK_APPLICATION_POINTER(mlx);
		static_cast<mlx::Application*>(mlx)->LoopHook(f, param);
		return 0;
	}

	int mlx_loop(void* mlx)
	{
		MLX_CHECK_APPLICATION_POINTER(mlx);
		static_cast<mlx::Application*>(mlx)->Run();
		return 0;
	}

	int mlx_loop_end(void* mlx)
	{
		MLX_CHECK_APPLICATION_POINTER(mlx);
		static_cast<mlx::Application*>(mlx)->LoopEnd();
		return 0;
	}

	int mlx_mouse_show()
	{
		return 0;
	}

	int mlx_mouse_hide()
	{
		return 0;
	}

	int mlx_mouse_move(void* mlx, void* win, int x, int y)
	{
		MLX_CHECK_APPLICATION_POINTER(mlx);
		static_cast<mlx::Application*>(mlx)->MouseMove(win, x, y);
		return 0;
	}

	int mlx_mouse_get_pos(void* mlx, int* x, int* y)
	{
		MLX_CHECK_APPLICATION_POINTER(mlx);
		static_cast<mlx::Application*>(mlx)->GetMousePos(x, y);
		return 0;
	}

	int mlx_on_event(void* mlx, void* win, mlx_event_type event, int (*funct_ptr)(int, void*), void* param)
	{
		MLX_CHECK_APPLICATION_POINTER(mlx);
		static_cast<mlx::Application*>(mlx)->OnEvent(win, static_cast<int>(event), funct_ptr, param);
		return 0;
	}

	void* mlx_new_image(void* mlx, int width, int height)
	{
		MLX_CHECK_APPLICATION_POINTER(mlx);
		if (width <= 0 || height <= 0)
		{
			mlx::Error("invalid image size (% x %)", width, height);
			return nullptr;
		}
		return static_cast<mlx::Application*>(mlx)->NewTexture(width, height);
	}

	int mlx_get_image_pixel(void* mlx, void* img, int x, int y)
	{
		MLX_CHECK_APPLICATION_POINTER(mlx);
		int color = static_cast<mlx::Application*>(mlx)->GetTexturePixel(img, x, y);
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
		static_cast<mlx::Application*>(mlx)->SetTexturePixel(img, x, y, *reinterpret_cast<unsigned int*>(color_bits));
	}

	int mlx_put_image_to_window(void* mlx, void* win, void* img, int x, int y)
	{
		MLX_CHECK_APPLICATION_POINTER(mlx);
		static_cast<mlx::Application*>(mlx)->TexturePut(win, img, x, y);
		return 0;
	}

	int mlx_destroy_image(void* mlx, void* img)
	{
		MLX_CHECK_APPLICATION_POINTER(mlx);
		static_cast<mlx::Application*>(mlx)->DestroyTexture(img);
		return 0;
	}

	void* mlx_png_file_to_image(void* mlx, char* filename, int* width, int* height)
	{
		MLX_CHECK_APPLICATION_POINTER(mlx);
		if (filename == nullptr)
		{
			mlx::Error("PNG loader : filename is NULL");
			return nullptr;
		}
		std::filesystem::path file(filename);
		if(file.extension() != ".png")
		{
			mlx::Error("PNG loader : not a png file '%'", filename);
			return nullptr;
		}
		return static_cast<mlx::Application*>(mlx)->NewStbTexture(filename, width, height);
	}

	void* mlx_jpg_file_to_image(void* mlx, char* filename, int* width, int* height)
	{
		MLX_CHECK_APPLICATION_POINTER(mlx);
		if (filename == nullptr)
		{
			mlx::Error("JPG loader : filename is NULL");
			return nullptr;
		}
		std::filesystem::path file(filename);
		if(file.extension() != ".jpg" && file.extension() != ".jpeg")
		{
			mlx::Error("JPG loader : not a jpg file '%'", filename);
			return nullptr;
		}
		return static_cast<mlx::Application*>(mlx)->NewStbTexture(filename, width, height);
	}

	void* mlx_bmp_file_to_image(void* mlx, char* filename, int* width, int* height)
	{
		MLX_CHECK_APPLICATION_POINTER(mlx);
		if (filename == nullptr)
		{
			mlx::Error("BMP loader : filename is NULL");
			return nullptr;
		}
		std::filesystem::path file(filename);
		if(file.extension() != ".bmp" && file.extension() != ".dib")
		{
			mlx::Error("BMP loader : not a bmp file '%'", filename);
			return nullptr;
		}
		return static_cast<mlx::Application*>(mlx)->NewStbTexture(filename, width, height);
	}

	int mlx_pixel_put(void* mlx, void* win, int x, int y, int color)
	{
		MLX_CHECK_APPLICATION_POINTER(mlx);
		unsigned char color_bits[4];
		color_bits[0] = (color & 0x00FF0000) >> 16;
		color_bits[1] = (color & 0x0000FF00) >> 8;
		color_bits[2] = (color & 0x000000FF);
		color_bits[3] = (color & 0xFF000000) >> 24;
		static_cast<mlx::Application*>(mlx)->PixelPut(win, x, y, *reinterpret_cast<unsigned int*>(color_bits));
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
		static_cast<mlx::Application*>(mlx)->StringPut(win, x, y, *reinterpret_cast<unsigned int*>(color_bits), str);
		return 0;
	}

	void mlx_set_font(void* mlx, char* filepath)
	{
		MLX_CHECK_APPLICATION_POINTER(mlx);
		if (filepath == nullptr)
		{
			mlx::Error("Font loader : filepath is NULL");
			return;
		}
		std::filesystem::path file(filepath);
		if(std::strcmp(filepath, "default") != 0 && file.extension() != ".ttf" && file.extension() != ".tte")
		{
			mlx::Error("TTF loader : not a truetype font file '%'", filepath);
			return;
		}
		if(std::strcmp(filepath, "default") == 0)
			static_cast<mlx::Application*>(mlx)->LoadFont(file, 6.f);
		else
			static_cast<mlx::Application*>(mlx)->LoadFont(file, 16.f);
	}

	void mlx_set_font_scale(void* mlx, char* filepath, float scale)
	{
		MLX_CHECK_APPLICATION_POINTER(mlx);
		if (filepath == nullptr)
		{
			mlx::Error("Font loader : filepath is NULL");
			return;
		}
		std::filesystem::path file(filepath);
		if(std::strcmp(filepath, "default") != 0 && file.extension() != ".ttf" && file.extension() != ".tte")
		{
			mlx::Error("TTF loader : not a truetype font file '%'", filepath);
			return;
		}
		static_cast<mlx::Application*>(mlx)->LoadFont(file, scale);
	}

	int mlx_clear_window(void* mlx, void* win)
	{
		MLX_CHECK_APPLICATION_POINTER(mlx);
		static_cast<mlx::Application*>(mlx)->ClearGraphicsSupport(win);
		return 0;
	}

	int mlx_destroy_window(void* mlx, void* win)
	{
		MLX_CHECK_APPLICATION_POINTER(mlx);
		static_cast<mlx::Application*>(mlx)->DestroyGraphicsSupport(win);
		return 0;
	}

	int mlx_destroy_display(void* mlx)
	{
		MLX_CHECK_APPLICATION_POINTER(mlx);
		delete static_cast<mlx::Application*>(mlx);
		__mlx_ptr = nullptr;
		return 0;
	}

	int mlx_get_screens_size(void* mlx, void* win, int* w, int* h)
	{
		MLX_CHECK_APPLICATION_POINTER(mlx);
		static_cast<mlx::Application*>(mlx)->GetScreenSize(win, w, h);
		return 0;
	}

	int mlx_set_fps_goal(void* mlx, int fps)
	{
		MLX_CHECK_APPLICATION_POINTER(mlx);
		if(fps < 0)
		{
			mlx::Error("You cannot set a negative FPS cap (nice try)");
			return 0;
		}
		if(fps == 0)
		{
			mlx::Error("You cannot set a FPS cap to 0 (nice try)");
			return 0;
		}
		static_cast<mlx::Application*>(mlx)->SetFPSCap(static_cast<std::uint32_t>(fps));
		return 0;
	}
}
