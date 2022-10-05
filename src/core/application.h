/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   application.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 21:49:46 by maldavid          #+#    #+#             */
/*   Updated: 2022/10/05 19:23:15 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_APPLICATION__
#define __MLX_APPLICATION__

#include <vector>
#include <memory>
#include <utility>

#include "errors.h"

#include <platform/inputs.h>
#include <platform/window.h>

namespace mlx::core
{
	class Application
	{
		public:
			Application() : _in(_wins) {}

			inline void* new_window(std::size_t w, std::size_t h, std::string title)
			{
				_wins.emplace_back(std::make_shared<Window>(w, h, std::move(title), _wins.size()));
				return reinterpret_cast<void*>(&_wins.back()->get_id());
			}

			inline int get_mouse_pos(void* win_ptr, int* x, int* y) noexcept
			{
				if(*reinterpret_cast<int*>(win_ptr) > _wins.size())
				{
					error::report(e_kind::error, "Invalid window pointer");
					return -1;
				}
			}

			void run() noexcept;

			~Application() = default;

		private:
			Input _in;
			std::vector<std::shared_ptr<Window>> _wins;
			bool _is_loop_running = false;
	};
}

#endif // __MLX_APPLICATION__
