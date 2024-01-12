/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 00:09:04 by maldavid          #+#    #+#             */
/*   Updated: 2024/01/11 00:13:25 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_TEXT__
#define __MLX_TEXT__

#include <string>
#include <vector>
#include <renderer/texts/font.h>
#include <renderer/buffers/vk_ibo.h>
#include <renderer/buffers/vk_vbo.h>

namespace mlx
{
	class Text
	{
		public:
			Text() = default;

			void init(std::string text, Font const* font, std::vector<Vertex> vbo_data, std::vector<uint16_t> ibo_data);
			void bind(class Renderer& renderer) noexcept;
			inline const Font& getFontInUse() const noexcept { return *_font; }
			void updateVertexData(int frame, std::vector<Vertex> vbo_data);
			inline uint32_t getIBOsize() noexcept { return _ibo.getSize(); }
			inline const std::string& getText() const { return _text; }
			void destroy() noexcept;

			~Text() = default;

		private:
			std::array<D_VBO, MAX_FRAMES_IN_FLIGHT> _vbo;
			C_IBO _ibo;
			std::string _text;
			Font const* _font = nullptr;
	};	
}

#endif
