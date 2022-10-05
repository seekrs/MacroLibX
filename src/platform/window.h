/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 21:53:12 by maldavid          #+#    #+#             */
/*   Updated: 2022/10/05 19:10:35 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_WINDOW__
#define __MLX_WINDOW__

#include <SDL2/SDL.h>
#include <string>

namespace mlx
{
	class Window
	{
		public:
			Window(std::size_t w, std::size_t h, std::string title, int id);

			inline int& get_id() noexcept { return _id; }
			
			~Window();

		private:
			SDL_Window* _win = nullptr;
			int _id;
	};
}

#endif
