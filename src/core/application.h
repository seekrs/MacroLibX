/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   application.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 21:49:46 by maldavid          #+#    #+#             */
/*   Updated: 2023/04/01 14:46:14 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_APPLICATION__
#define __MLX_APPLICATION__

#include <vector>
#include <memory>
#include <utility>
#include <functional>

#include "errors.h"

#include <platform/inputs.h>
#include <platform/window.h>

#include <renderer/texture_library.h>

namespace mlx::core
{
	class Application
	{
		public:
			Application() : _in() {}

			inline void* new_window(std::size_t w, std::size_t h, std::string title)
			{
				_wins.emplace_back(std::make_shared<MLX_Window>(w, h, std::move(title), _wins.size()));
				return static_cast<void*>(&_wins.back()->get_id());
			}

			inline int get_mouse_pos(void* win_ptr, int* x, int* y) noexcept
			{
				if(*static_cast<int*>(win_ptr) > _wins.size())
					return -1;
			}

			inline void loop_hook(int (*f)(void*), void* param) { _loop_hook = f; _param = param; }
			inline void loop_end() noexcept { _in.finish(); }

			inline void pixel_put(void* win_ptr, int x, int y, int color) const noexcept { _wins[*static_cast<int*>(win_ptr)]->pixel_put(x, y, color); }

			void* new_stb_texture(char* file, int* w, int* h); // stb textures are format managed by stb image (png, jpg, bpm, ...)
			void texture_put(void* win, void* img, int x, int y);
			void destroy_texture(void* ptr);
			
			inline void destroy_window(void* win_ptr) { _wins[*static_cast<int*>(win_ptr)].reset(); }

			void run() noexcept;

			~Application();

		private:
			Input _in;
			TextureLibrary _texture_lib;
			std::vector<TextureID> _texture_ids;
			std::vector<std::shared_ptr<MLX_Window>> _wins;
			std::function<int(void*)> _loop_hook;
			void* _param = nullptr;
			bool _is_loop_running = false;
	};
}

#endif // __MLX_APPLICATION__
