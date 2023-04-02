/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 21:53:12 by maldavid          #+#    #+#             */
/*   Updated: 2023/04/02 15:39:53 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_WINDOW__
#define __MLX_WINDOW__

#include <SDL2/SDL.h>
#include <string>

namespace mlx
{
	class MLX_Window
	{
		public:
			MLX_Window(std::size_t w, std::size_t h, std::string title);

			inline SDL_Window* getNativeWindow() const noexcept { return _win; }
			inline int getWidth() const noexcept { return _width; }
			inline int getHeight() const noexcept { return _height; }

			~MLX_Window();

		private:
			SDL_Window* _win = nullptr;
			int _width = 0;
			int _height = 0;
	};
}

#endif
