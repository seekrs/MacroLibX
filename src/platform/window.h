/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 21:53:12 by maldavid          #+#    #+#             */
/*   Updated: 2023/01/25 15:15:25 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_WINDOW__
#define __MLX_WINDOW__

#include <SDL2/SDL.h>
#include <string>
#include <memory>
#include <renderer/renderer.h>
#include <renderer/images/vk_image.h>
#include <renderer/buffers/vk_buffer.h>
#include <renderer/buffers/vk_vbo.h>
#include <renderer/buffers/vk_ibo.h>

namespace mlx
{
	class MLX_Window
	{
		public:
			MLX_Window(std::size_t w, std::size_t h, std::string title, int id);

			inline int& get_id() noexcept { return _id; }
			inline SDL_Window* getNativeWindow() const noexcept { return _win; }

			bool beginFrame();
			void endFrame();

			void pixel_put(int x, int y, int color);
			
			~MLX_Window();

		private:
			Image _vk_image;
			Buffer _staging_buffer;
			C_VBO _vbo;
			C_IBO _ibo;
			glm::mat4 _proj = glm::mat4(1.0);
			std::unique_ptr<Renderer> _renderer;
			SDL_Window* _win = nullptr;
			int _width = 0;
			int _height = 0;
			int _id;
	};
}

#endif
