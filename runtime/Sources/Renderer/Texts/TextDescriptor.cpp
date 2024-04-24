/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TextDescriptor.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 00:23:11 by maldavid          #+#    #+#             */
/*   Updated: 2024/04/24 01:38:40 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Preompiled.h>

#include <Renderer/Images/TextureAtlas.h>
#include <Renderer/Texts/TextDescriptor.h>
#include <Renderer/Texts/Font.h>
#include <Renderer/Texts/Text.h>

#define STB_RECT_PACK_IMPLEMENTATION
#include <stb_rect_pack.h>

#include <Core/Memory.h>

#define STB_TRUETYPE_IMPLEMENTATION
#define STB_malloc(x, u) ((void)(u), MemManager::Malloc(x))
#define STB_free(x, u) ((void)(u), MemManager::Free(x))
#include <stb_truetype.h>

constexpr const int RANGE = 1024;

namespace mlx
{
	TextDrawDescriptor::TextDrawDescriptor(std::string text, std::uint32_t _color, int _x, int _y) : color(_color), x(_x), y(_y), m_text(std::move(text))
	{}

	void TextDrawDescriptor::Init(FontID font) noexcept
	{
		MLX_PROFILE_FUNCTION();
		std::vector<Vertex> vertex_data;
		std::vector<std::uint16_t> index_data;

		float stb_x = 0.0f;
		float stb_y = 0.0f;

		{
			std::shared_ptr<Font> font_data = FontLibrary::Get().GetFontData(font);

			for(char c : m_text)
			{
				if(c < 32)
					continue;

				stbtt_aligned_quad q;
				stbtt_GetPackedQuad(font_data->GetCharData().data(), RANGE, RANGE, c - 32, &stb_x, &stb_y, &q, 1);

				std::size_t index = vertex_data.size();

				glm::vec4 vertex_color = {
					static_cast<float>((color & 0x000000FF)) / 255.f,
					static_cast<float>((color & 0x0000FF00) >> 8) / 255.f,
					static_cast<float>((color & 0x00FF0000) >> 16) / 255.f,
					static_cast<float>((color & 0xFF000000) >> 24) / 255.f
				};

				vertex_data.emplace_back(glm::vec2{q.x0, q.y0}, vertex_color, glm::vec2{q.s0, q.t0});
				vertex_data.emplace_back(glm::vec2{q.x1, q.y0}, vertex_color, glm::vec2{q.s1, q.t0});
				vertex_data.emplace_back(glm::vec2{q.x1, q.y1}, vertex_color, glm::vec2{q.s1, q.t1});
				vertex_data.emplace_back(glm::vec2{q.x0, q.y1}, vertex_color, glm::vec2{q.s0, q.t1});

				index_data.emplace_back(index + 0);
				index_data.emplace_back(index + 1);
				index_data.emplace_back(index + 2);
				index_data.emplace_back(index + 2);
				index_data.emplace_back(index + 3);
				index_data.emplace_back(index + 0);
			}
		}
		std::shared_ptr<Text> text_data = std::make_shared<Text>();
		text_data->Init(m_text, font, color, std::move(vertex_data), std::move(index_data));
		id = TextLibrary::Get().AddTextToLibrary(text_data);
		DebugLog("Text put : registered new text to render");
	}

	void TextDrawDescriptor::Render(Renderer& renderer)
	{
		MLX_PROFILE_FUNCTION();
		std::shared_ptr<Text> draw_data = TextLibrary::Get().GetTextData(id);
		std::shared_ptr<Font> font_data = FontLibrary::Get().GetFontData(draw_data->GetFontInUse());
		TextureAtlas& atlas = const_cast<TextureAtlas&>(font_data->GetAtlas());
		draw_data->Bind(renderer);
		if(!atlas.GetSet().IsInit())
			atlas.SetDescriptor(renderer.GetFragDescriptorSet().Duplicate());
		if(!atlas.HasBeenUpdated())
			atlas.UpdateSet(0);
		atlas.GetSet().Bind();
		atlas.Render(renderer, x, y, draw_data->GetIBOsize());
	}

	void TextDrawDescriptor::ResetUpdate()
	{
		std::shared_ptr<Text> draw_data = TextLibrary::Get().GetTextData(id);
		std::shared_ptr<Font> font_data = FontLibrary::Get().GetFontData(draw_data->GetFontInUse());
		TextureAtlas& atlas = const_cast<TextureAtlas&>(font_data->GetAtlas());
		atlas.ResetUpdate();
	}
}
