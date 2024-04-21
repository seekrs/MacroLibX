/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Text.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 00:09:04 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/28 22:23:50 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_TEXT__
#define __MLX_TEXT__

#include <Renderer/Texts/Font.h>
#include <Renderer/Texts/FontLibrary.h>
#include <Renderer/Buffers/IndexBuffer.h>
#include <Renderer/Buffers/VertexBuffer.h>

namespace mlx
{
	class Text
	{
		public:
			Text() = default;

			void Init(std::string text, FontID font, std::uint32_t color, std::vector<Vertex> vbo_data, std::vector<std::uint16_t> ibo_data);
			void Bind(class Renderer& renderer) noexcept;
			inline FontID GetFontInUse() const noexcept { return m_font; }
			void UpdateVertexData(int frame, std::vector<Vertex> vbo_data);
			inline std::uint32_t GetIBOsize() noexcept { return m_ibo.GetSize(); }
			inline const std::string& GetText() const { return m_text; }
			inline std::uint32_t GetColor() const noexcept { return m_color; }
			void Destroy() noexcept;

			~Text();

		private:
			std::array<DeviceVertexBuffer, MAX_FRAMES_IN_FLIGHT> m_vbo;
			ConstantIndexBuffer m_ibo;
			std::string m_text;
			std::uint32_t m_color;
			FontID m_font = nullfont;
			bool m_is_init = false;
	};	
}

#endif
