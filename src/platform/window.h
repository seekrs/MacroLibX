/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 21:53:12 by maldavid          #+#    #+#             */
/*   Updated: 2023/12/21 00:24:26 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_WINDOW__
#define __MLX_WINDOW__

#include <SDL2/SDL.h>
#include <string>
#include <mlx_profile.h>

namespace mlx
{
	class MLX_Window
	{
		public:
			MLX_Window(std::size_t w, std::size_t h, const std::string& title);

			inline SDL_Window* getNativeWindow() const noexcept { return _win; }
			inline int getWidth() const noexcept { return _width; }
			inline int getHeight() const noexcept { return _height; }
			inline std::uint32_t getID() const noexcept { return _id; }

			void destroy() noexcept;

			~MLX_Window() = default;

		private:
			SDL_Surface* _icon = nullptr;
			SDL_Window* _win = nullptr;
			int _width = 0;
			int _height = 0;
			std::uint32_t _id = -1;
	};
}

#endif
