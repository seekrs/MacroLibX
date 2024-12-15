#ifndef __MLX_APPLICATION__
#define __MLX_APPLICATION__

#include <Core/Graphics.h>
#include <Platform/Inputs.h>
#include <Core/ImagesRegistry.h>
#include <Core/SDLManager.h>
#include <Core/Memory.h>
#include <Core/Fps.h>
#include <Graphics/Font.h>

namespace mlx
{
	class Application
	{
		public:
			Application();

			inline void GetMousePos(int* x, int* y) noexcept;
			inline void GetScreenSize(mlx_window win, int* w, int* h) noexcept;
			inline void SetFPSCap(std::uint32_t fps) noexcept;

			inline void OnEvent(mlx_window win, int event, int (*funct_ptr)(int, void*), void* param) noexcept;

			inline mlx_window NewGraphicsSuport(const mlx_window_create_info* info);
			inline NonOwningPtr<GraphicsSupport> GetGraphicsSupport(mlx_window win);
			inline void DestroyGraphicsSupport(mlx_window win);

			mlx_image NewTexture(int w, int h);
			mlx_image NewStbTexture(char* file, int* w, int* h); // stb textures are image files (png, jpg, bpm, ...)
			inline NonOwningPtr<Texture> GetTexture(mlx_image image);
			void DestroyTexture(mlx_image img);

			inline void LoopHook(int (*f)(void*), void* param);
			inline void LoopEnd() noexcept;

			inline void LoadFont(const std::filesystem::path& filepath, float scale);

			void Run() noexcept;

			~Application();

		private:
			std::unique_ptr<MemManager> p_mem_manager; // Putting ptr here to initialise them before inputs, even if it f*cks the padding
			std::unique_ptr<SDLManager> p_sdl_manager;
			FpsManager m_fps;
			Inputs m_in;
			FontRegistry m_font_registry;
			ImageRegistry m_image_registry;
			std::vector<std::unique_ptr<GraphicsSupport>> m_graphics;
			std::shared_ptr<Font> p_last_font_bound;
			std::function<int(Handle)> f_loop_hook;
			std::unique_ptr<RenderCore> p_render_core;
			#ifdef PROFILER
				std::unique_ptr<Profiler> p_profiler;
			#endif
			Handle p_param = nullptr;
	};
}

#include <Core/Application.inl>

#endif // __MLX_APPLICATION__
