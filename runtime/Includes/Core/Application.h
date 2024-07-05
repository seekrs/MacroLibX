/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Application.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 21:49:46 by maldavid          #+#    #+#             */
/*   Updated: 2024/07/05 14:04:19 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
			inline void MouseMove(void* win, int x, int y) noexcept;

			inline void OnEvent(void* win, int event, int (*funct_ptr)(int, void*), void* param) noexcept;

			inline void GetScreenSize(void* win, int* w, int* h) noexcept;

			inline void SetFPSCap(std::uint32_t fps) noexcept;

			inline void* NewGraphicsSuport(std::size_t w, std::size_t h, const char* title);
			inline void ClearGraphicsSupport(void* win);
			inline void DestroyGraphicsSupport(void* win);

			inline void PixelPut(void* win, int x, int y, std::uint32_t color) const noexcept;
			inline void StringPut(void* win, int x, int y, std::uint32_t color, char* str);

			void* NewTexture(int w, int h);
			void* NewStbTexture(char* file, int* w, int* h); // stb textures are image files (png, jpg, bpm, ...)
			inline void TexturePut(void* win, void* img, int x, int y);
			inline int GetTexturePixel(void* img, int x, int y);
			inline void SetTexturePixel(void* img, int x, int y, std::uint32_t color);
			void DestroyTexture(void* ptr);

			inline void LoopHook(int (*f)(void*), void* param);
			inline void LoopEnd() noexcept;

			inline void LoadFont(void* win, const std::filesystem::path& filepath, float scale);

			void Run() noexcept;

			~Application();

		private:
			FpsManager m_fps;
			Inputs m_in;
			ImageRegistry m_image_registry;
			std::vector<std::unique_ptr<GraphicsSupport>> m_graphics;
			std::function<int(void*)> f_loop_hook;
			void* p_param = nullptr;
	};
}

#include <Core/Application.inl>

#endif // __MLX_APPLICATION__
