/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   application.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 21:49:46 by maldavid          #+#    #+#             */
/*   Updated: 2022/10/05 16:58:25 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_APPLICATION__
#define __MLX_APPLICATION__

#include <vector>
#include <memory>
#include <utility>

#include <platform/inputs.h>
#include <platform/window.h>

namespace mlx::core
{
	class Application
	{
		public:
			Application() = default;

			inline void new_window(std::size_t w, std::size_t h, std::string title)
			{
				_wins.emplace_back(std::make_unique<Window>(w, h, std::move(title)));
			}

			void run() noexcept;

			~Application() = default;

		private:
			Input _in;
			std::vector<std::unique_ptr<Window>> _wins;
			bool _is_loop_running = false;
	};
}

#endif // __MLX_APPLICATION__
