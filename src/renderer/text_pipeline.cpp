/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text_pipeline.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 16:41:13 by maldavid          #+#    #+#             */
/*   Updated: 2024/01/10 20:20:30 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <renderer/text_pipeline.h>
#include <core/profiler.h>
#include <fstream>

#include <utils/dogica_ttf.h>
#include <iostream>
#include <cstdio>

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
	TextDrawData::TextDrawData(std::string _text, uint32_t _color, int _x, int _y) :
		x(_x), y(_y), color(_color),
		text(std::move(_text))
	{}

	void TextDrawData::init(TextLibrary& library, Font* const font) noexcept
	{
		MLX_PROFILE_FUNCTION();
		std::vector<Vertex> vertexData;
		std::vector<uint16_t> indexData;

		float stb_x = 0.0f;
		float stb_y = 0.0f;

		for(char c : text)
		{
			if(c < 32 && c != '\n')
				continue;

			stbtt_aligned_quad q;
			stbtt_GetPackedQuad(font->getCharData().data(), RANGE, RANGE, c - 32, &stb_x, &stb_y, &q, 1);

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
		std::shared_ptr<TextData> text_data = std::make_shared<TextData>();
		text_data->init(text, font, std::move(vertexData), std::move(indexData));
		id = library.addTextToLibrary(text_data);

		#ifdef DEBUG
			core::error::report(e_kind::message, "Text put : registered new text to render");
		#endif
	}

	void TextPutPipeline::init(Renderer* renderer) noexcept
	{
		MLX_PROFILE_FUNCTION();
		_renderer = renderer;
		_font_in_use = &const_cast<Font&>(*_font_set.emplace(*_renderer, "default", dogica_ttf, 6.0f).first);
	}

	void TextPutPipeline::loadFont(const std::filesystem::path& filepath, float scale)
	{
		MLX_PROFILE_FUNCTION();
		if(filepath.string() == "default") // we're sure it is already loaded
			_font_in_use = &const_cast<Font&>(*_font_set.emplace(*_renderer, "default", dogica_ttf, scale).first);
		else
			_font_in_use = &const_cast<Font&>(*_font_set.emplace(*_renderer, filepath, scale).first);
	}

	void TextPutPipeline::put(int x, int y, uint32_t color, std::string str)
	{
		MLX_PROFILE_FUNCTION();
		auto res = _drawlist.emplace(std::move(str), color, x, y);
		if(res.second)
			const_cast<TextDrawData&>(*res.first).init(_library, _font_in_use);
		else
		{
			auto text_ptr = _library.getTextData(res.first->id);
			if(*_font_in_use != text_ptr->getFontInUse())
			{
				_library.removeTextFromLibrary(res.first->id);
				const_cast<TextDrawData&>(*res.first).init(_library, _font_in_use);
			}
		}
	}

	void TextPutPipeline::render(std::array<VkDescriptorSet, 2>& sets)
	{
		MLX_PROFILE_FUNCTION();
		for(auto& draw : _drawlist)
		{
			std::shared_ptr<TextData> draw_data = _library.getTextData(draw.id);
			TextureAtlas& atlas = const_cast<TextureAtlas&>(draw_data->getFontInUse().getAtlas());
			draw_data->bind(*_renderer);
			if(atlas.getSet() == VK_NULL_HANDLE)
				atlas.setDescriptor(_renderer->getFragDescriptorSet().duplicate());
			if(!atlas.hasBeenUpdated())
				atlas.updateSet(0);
			sets[1] = const_cast<TextureAtlas&>(atlas).getSet();
			vkCmdBindDescriptorSets(_renderer->getActiveCmdBuffer().get(), VK_PIPELINE_BIND_POINT_GRAPHICS, _renderer->getPipeline().getPipelineLayout(), 0, sets.size(), sets.data(), 0, nullptr);
			atlas.render(*_renderer, draw.x, draw.y, draw_data->getIBOsize());
		}

		for(auto& draw : _drawlist)
		{
			std::shared_ptr<TextData> draw_data = _library.getTextData(draw.id);
			TextureAtlas& atlas = const_cast<TextureAtlas&>(draw_data->getFontInUse().getAtlas());
			atlas.resetUpdate();
		}
	}

	void TextPutPipeline::destroy() noexcept
	{
		MLX_PROFILE_FUNCTION();
		_library.clearLibrary();
		_drawlist.clear();
		_font_set.clear();
	}
}
