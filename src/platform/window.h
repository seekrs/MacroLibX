/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 21:53:12 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/26 23:03:50 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_WINDOW__
#define __MLX_WINDOW__

namespace mlx
{
	class Window
	{
		public:
			Window(std::size_t w, std::size_t h, const std::string& title);

			inline GLFWwindow* getNativeWindow() const noexcept { return _win; }
			inline int getWidth() const noexcept { return _width; }
			inline int getHeight() const noexcept { return _height; }
			inline std::uint32_t getID() const noexcept { return _id; }

			void destroy() noexcept;

			~Window() = default;

		private:
			GLFWimage _icon;
			GLFWwindow* _win = nullptr;
			int _width = 0;
			int _height = 0;
			std::uint32_t _id = -1;
	};
}

#endif
