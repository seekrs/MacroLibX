/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text_descriptor.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 00:23:11 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/25 16:13:48 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <core/profiler.h>
#include <renderer/texts/text_descriptor.h>
#include <renderer/images/texture_atlas.h>
#include <renderer/texts/font.h>
#include <renderer/texts/text.h>

#define STB_RECT_PACK_IMPLEMENTATION
#include <stb_rect_pack.h>

#include <core/memory.h>

#define STB_TRUETYPE_IMPLEMENTATION
#define STB_malloc(x, u) ((void)(u), MemManager::malloc(x))
#define STB_free(x, u) ((void)(u), MemManager::free(x))
#include <stb_truetype.h>

constexpr const int RANGE = 1024;

namespace mlx
{
	TextDrawDescriptor::TextDrawDescriptor(std::string text, std::uint32_t _color, int _x, int _y) : color(_color), x(_x), y(_y), _text(std::move(text))
	{}

	void TextDrawDescriptor::init(FontID font) noexcept
	{
		MLX_PROFILE_FUNCTION();
		std::vector<Vertex> vertexData;
		std::vector<std::uint16_t> indexData;

		float stb_x = 0.0f;
		float stb_y = 0.0f;

		{
			std::shared_ptr<Font> font_data = FontLibrary::get().getFontData(font);

			for(char c : _text)
			{
				if(c < 32)
					continue;

				stbtt_aligned_quad q;
				stbtt_GetPackedQuad(font_data->getCharData().data(), RANGE, RANGE, c - 32, &stb_x, &stb_y, &q, 1);

				std::size_t index = vertexData.size();

				glm::vec4 vertex_color = {
					static_cast<float>((color & 0x000000FF)) / 255.f,
					static_cast<float>((color & 0x0000FF00) >> 8) / 255.f,
					static_cast<float>((color & 0x00FF0000) >> 16) / 255.f,
					static_cast<float>((color & 0xFF000000) >> 24) / 255.f
				};

				vertexData.emplace_back(glm::vec2{q.x0, q.y0}, vertex_color, glm::vec2{q.s0, q.t0});
				vertexData.emplace_back(glm::vec2{q.x1, q.y0}, vertex_color, glm::vec2{q.s1, q.t0});
				vertexData.emplace_back(glm::vec2{q.x1, q.y1}, vertex_color, glm::vec2{q.s1, q.t1});
				vertexData.emplace_back(glm::vec2{q.x0, q.y1}, vertex_color, glm::vec2{q.s0, q.t1});

				indexData.emplace_back(index + 0);
				indexData.emplace_back(index + 1);
				indexData.emplace_back(index + 2);
				indexData.emplace_back(index + 2);
				indexData.emplace_back(index + 3);
				indexData.emplace_back(index + 0);
			}
		}
		std::shared_ptr<Text> text_data = std::make_shared<Text>();
		text_data->init(_text, font, color, std::move(vertexData), std::move(indexData));
		id = TextLibrary::get().addTextToLibrary(text_data);

		#ifdef DEBUG
			core::error::report(e_kind::message, "Text put : registered new text to render");
		#endif
	}

	void TextDrawDescriptor::render(std::array<VkDescriptorSet, 2>& sets, Renderer& renderer)
	{
		MLX_PROFILE_FUNCTION();
		std::shared_ptr<Text> draw_data = TextLibrary::get().getTextData(id);
		std::shared_ptr<Font> font_data = FontLibrary::get().getFontData(draw_data->getFontInUse());
		TextureAtlas& atlas = const_cast<TextureAtlas&>(font_data->getAtlas());
		draw_data->bind(renderer);
		if(!atlas.getSet().isInit())
			atlas.setDescriptor(renderer.getFragDescriptorSet().duplicate());
		if(!atlas.hasBeenUpdated())
			atlas.updateSet(0);
		sets[1] = const_cast<TextureAtlas&>(atlas).getVkSet();
		vkCmdBindDescriptorSets(renderer.getActiveCmdBuffer().get(), VK_PIPELINE_BIND_POINT_GRAPHICS, renderer.getPipeline().getPipelineLayout(), 0, sets.size(), sets.data(), 0, nullptr);
		atlas.render(renderer, x, y, draw_data->getIBOsize());
	}

	void TextDrawDescriptor::resetUpdate()
	{
		std::shared_ptr<Text> draw_data = TextLibrary::get().getTextData(id);
		std::shared_ptr<Font> font_data = FontLibrary::get().getFontData(draw_data->getFontInUse());
		TextureAtlas& atlas = const_cast<TextureAtlas&>(font_data->getAtlas());
		atlas.resetUpdate();
	}
}
