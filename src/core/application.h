/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   application.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 21:49:46 by maldavid          #+#    #+#             */
/*   Updated: 2023/04/21 19:24:12 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_APPLICATION__
#define __MLX_APPLICATION__

#include <list>
#include <memory>
#include <string>
#include <vector>
#include <utility>
#include <functional>

#include <core/errors.h>

#include <core/graphics.h>
#include <platform/inputs.h>

namespace mlx::core
{
	class Application
	{
		public:
			Application() : _in(std::make_unique<Input>()) {}

			inline void getMousePos(int* x, int* y) noexcept;
			inline void mouseMove(void* win, int x, int y) noexcept;

			inline void onEvent(void* win, int event, int (*funct_ptr)(int, void*), void* param) noexcept;

			inline void getScreenSize(int* w, int* h) noexcept;

			inline void* newGraphicsSuport(std::size_t w, std::size_t h, std::string title);
			inline void clearGraphicsSupport(void* win);
			inline void destroyGraphicsSupport(void* win);

			inline void pixelPut(void* win, int x, int y, uint32_t color) const noexcept;
			inline void stringPut(void* win, int x, int y, int color, char* str);

			void* newTexture(int w, int h);
			void* newStbTexture(char* file, int* w, int* h); // stb textures are format managed by stb image (png, jpg, bpm, ...)
			char* mapTexture(Texture* img, int* bits_per_pixel, int* size_line, int* endian);
			inline void texturePut(void* win, void* img, int x, int y);
			void destroyTexture(void* ptr);

			inline void loopHook(int (*f)(void*), void* param);
			inline void loopEnd() noexcept;
			
			void run() noexcept;

			~Application() = default;

		private:
			std::list<Texture> _textures;
			std::vector<std::unique_ptr<GraphicsSupport>> _graphics;
			std::function<int(void*)> _loop_hook;
			std::unique_ptr<Input> _in;
			void* _param = nullptr;
			bool _is_loop_running = false;
	};
}

#include <core/application.inl>

#endif // __MLX_APPLICATION__
