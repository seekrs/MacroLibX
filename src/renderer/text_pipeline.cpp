/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text_pipeline.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 16:41:13 by maldavid          #+#    #+#             */
/*   Updated: 2023/04/11 12:49:59 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <renderer/text_pipeline.h>
#include <fstream>

#include <utils/opensans-regular.h>
#include <cstdio>

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

#include <iostream>

namespace mlx
{
	uint8_t tmp_bitmap[512 * 512];

	TextDrawData::TextDrawData(std::string text, int _color, int _x, int _y, TextLibrary& library, std::array<stbtt_bakedchar, 96>& cdata) : x(_x), y(_y), color(_color)
	{
		std::vector<Vertex> vertexData;
		std::vector<uint16_t> indexData;

		float stb_x = 0.0f;
		float stb_y = 0.0f;

		for(char c : text)
		{
			if(c < 32 || c > 127)
				continue;

			stbtt_aligned_quad q;
			stbtt_GetBakedQuad(cdata.data(), 512, 512, c - 32, &stb_x, &stb_y, &q, 1);

			std::size_t index = vertexData.size();

			vertexData.emplace_back(glm::vec2{q.x0, q.y0}, glm::vec4{color & 0x00FF0000, color & 0x0000FF00, color & 0x000000FF, 0xFF}, glm::vec2{q.s0, q.t0});
			vertexData.emplace_back(glm::vec2{q.x1, q.y0}, glm::vec4{color & 0x00FF0000, color & 0x0000FF00, color & 0x000000FF, 0xFF}, glm::vec2{q.s1, q.t0});
			vertexData.emplace_back(glm::vec2{q.x1, q.y1}, glm::vec4{color & 0x00FF0000, color & 0x0000FF00, color & 0x000000FF, 0xFF}, glm::vec2{q.s1, q.t1});
			vertexData.emplace_back(glm::vec2{q.x0, q.y1}, glm::vec4{color & 0x00FF0000, color & 0x0000FF00, color & 0x000000FF, 0xFF}, glm::vec2{q.s0, q.t1});

			indexData.emplace_back(index + 0);
			indexData.emplace_back(index + 1);
			indexData.emplace_back(index + 2);
			indexData.emplace_back(index + 2);
			indexData.emplace_back(index + 3);
			indexData.emplace_back(index + 0);
		}

		std::shared_ptr<TextData> text_data = std::make_shared<TextData>();
		text_data->init(std::move(vertexData), std::move(indexData));
		id = library.addTextToLibrary(text_data);
	}

	void TextPutPipeline::init(Renderer* renderer) noexcept
	{
		_renderer = renderer;
		stbtt_BakeFontBitmap(opensans_regular_ttf, 0, 18.0f, tmp_bitmap, 512, 512, 32, 96, _cdata.data());
		_atlas.create(tmp_bitmap, 512, 512, VK_FORMAT_R8_UNORM);
		_atlas.setDescriptor(renderer->getFragDescriptorSet().duplicate());
	}

	void TextPutPipeline::put(int x, int y, int color, std::string str)
	{
		_drawlist.emplace(std::move(str), color, x, y, _library, _cdata);
	}

	void TextPutPipeline::render()
	{
		_atlas.updateSet(0);
		for(auto& draw : _drawlist)
		{
			std::shared_ptr<TextData> draw_data = _library.getTextData(draw.id);
			draw_data->bind(*_renderer);
			_atlas.render(*_renderer, draw.x, draw.y, draw_data->getIBOsize());
		}
	}

	void TextPutPipeline::destroy() noexcept
	{
		_library.clearLibrary();
		_atlas.destroy();
	}
}
