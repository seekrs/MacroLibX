#ifndef __MLX_APPLICATION__
#define __MLX_APPLICATION__

#include <Core/Graphics.h>
#include <Platform/Inputs.h>
#include <Core/ImagesRegistry.h>
#include <Core/Fps.h>

namespace mlx
{
	class Application
	{
		public:
			Application();

			inline void GetMousePos(int* x, int* y) noexcept;
			inline void MouseMove(Handle win, int x, int y) noexcept;

			inline void OnEvent(Handle win, int event, int (*funct_ptr)(int, void*), void* param) noexcept;

			inline void GetScreenSize(Handle win, int* w, int* h) noexcept;

			inline void SetFPSCap(std::uint32_t fps) noexcept;

			inline Handle NewGraphicsSuport(std::size_t w, std::size_t h, const char* title);
			inline void ClearGraphicsSupport(Handle win);
			inline void DestroyGraphicsSupport(Handle win);
			inline void SetGraphicsSupportPosition(Handle win, int x, int y);

			inline void PixelPut(Handle win, int x, int y, std::uint32_t color) const noexcept;
			inline void StringPut(Handle win, int x, int y, std::uint32_t color, char* str);

			Handle NewTexture(int w, int h);
			Handle NewStbTexture(char* file, int* w, int* h); // stb textures are image files (png, jpg, bpm, ...)
			inline void TexturePut(Handle win, Handle img, int x, int y);
			inline int GetTexturePixel(Handle img, int x, int y);
			inline void SetTexturePixel(Handle img, int x, int y, std::uint32_t color);
			void DestroyTexture(Handle ptr);

			inline void LoopHook(int (*f)(void*), void* param);
			inline void LoopEnd() noexcept;

			inline void LoadFont(Handle win, const std::filesystem::path& filepath, float scale);

			void Run() noexcept;

			~Application();

		private:
			FpsManager m_fps;
			Inputs m_in;
			ImageRegistry m_image_registry;
			std::vector<std::unique_ptr<GraphicsSupport>> m_graphics;
			std::function<int(Handle)> f_loop_hook;
			std::unique_ptr<RenderCore> p_render_core;
			Handle p_param = nullptr;
	};
}

#include <Core/Application.inl>

#endif // __MLX_APPLICATION__
