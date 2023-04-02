/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   application.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 21:49:46 by maldavid          #+#    #+#             */
/*   Updated: 2023/04/02 23:38:05 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_APPLICATION__
#define __MLX_APPLICATION__

#include <list>
#include <memory>
#include <utility>
#include <functional>

#include <core/errors.h>

#include <core/graphics.h>
#include <platform/inputs.h>

#include <renderer/texture_library.h>

namespace mlx::core
{
	class Application
	{
		public:
			Application() : _in() {}

			inline void getMousePos(int* x, int* y) noexcept;
			inline void mouseMove(void* win_ptr, int x, int y) noexcept;

			inline void getScreenSize(int* w, int* h) noexcept;

			inline void* newGraphicsSuport(std::size_t w, std::size_t h, std::string title);
			inline void clearGraphicsSupport(void* win_ptr);
			inline void destroyGraphicsSupport(void* win_ptr);

			inline void pixelPut(void* win_ptr, int x, int y, int color) const noexcept;

			void* newTexture(int w, int h);
			void* newStbTexture(char* file, int* w, int* h); // stb textures are format managed by stb image (png, jpg, bpm, ...)
			char* mapTexture(void* img_ptr, int* bits_per_pixel, int* size_line, int* endian);
			inline void texturePut(void* win_ptr, void* img, int x, int y);
			void destroyTexture(void* ptr);

			inline void loopHook(int (*f)(void*), void* param);
			inline void loopEnd() noexcept;
			
			void run() noexcept;

			~Application() = default;

		private:
			Input _in;
			TextureLibrary _texture_lib;
			std::list<TextureID> _texture_ids;
			std::vector<std::unique_ptr<GraphicsSupport>> _graphics;
			std::function<int(void*)> _loop_hook;
			void* _param = nullptr;
			bool _is_loop_running = false;
	};
}

#include <core/application.inl>

#endif // __MLX_APPLICATION__
