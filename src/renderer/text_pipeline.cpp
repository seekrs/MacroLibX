/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text_pipeline.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 16:41:13 by maldavid          #+#    #+#             */
/*   Updated: 2023/11/25 10:40:39 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <renderer/text_pipeline.h>
#include <fstream>

#include <utils/dogica_ttf.h>
#include <iostream>
#include <cstdio>

#define STB_RECT_PACK_IMPLEMENTATION
#include <stb_rect_pack.h>

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

constexpr const int RANGE = 1024;

namespace mlx
{
	TextDrawData::TextDrawData(std::string _text, int _color, int _x, int _y) :
		x(_x), y(_y), color(_color),
		text(std::move(_text))
	{}

	void TextDrawData::init(TextLibrary& library, std::array<stbtt_packedchar, 96>& cdata) noexcept
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
			stbtt_GetPackedQuad(cdata.data(), RANGE, RANGE, c - 32, &stb_x, &stb_y, &q, 1);

			std::size_t index = vertexData.size();

			vertexData.emplace_back(glm::vec2{q.x0, q.y0}, glm::vec4{color & 0x00FF0000, color & 0x0000FF00, color & 0x000000FF, 0xFFFFFFFF}, glm::vec2{q.s0, q.t0});
			vertexData.emplace_back(glm::vec2{q.x1, q.y0}, glm::vec4{color & 0x00FF0000, color & 0x0000FF00, color & 0x000000FF, 0xFFFFFFFF}, glm::vec2{q.s1, q.t0});
			vertexData.emplace_back(glm::vec2{q.x1, q.y1}, glm::vec4{color & 0x00FF0000, color & 0x0000FF00, color & 0x000000FF, 0xFFFFFFFF}, glm::vec2{q.s1, q.t1});
			vertexData.emplace_back(glm::vec2{q.x0, q.y1}, glm::vec4{color & 0x00FF0000, color & 0x0000FF00, color & 0x000000FF, 0xFFFFFFFF}, glm::vec2{q.s0, q.t1});

			indexData.emplace_back(index + 0);
			indexData.emplace_back(index + 1);
			indexData.emplace_back(index + 2);
			indexData.emplace_back(index + 2);
			indexData.emplace_back(index + 3);
			indexData.emplace_back(index + 0);
		}
		std::shared_ptr<TextData> text_data = std::make_shared<TextData>();
		text_data->init(text, std::move(vertexData), std::move(indexData));
		id = library.addTextToLibrary(text_data);

		#ifdef DEBUG
			core::error::report(e_kind::message, "Text put : registered new text to render");
		#endif
	}

	void TextPutPipeline::init(Renderer* renderer) noexcept
	{
		_renderer = renderer;
		uint8_t tmp_bitmap[RANGE * RANGE];
		uint8_t vulkan_bitmap[RANGE * RANGE * 4];
		stbtt_pack_context pc;
		stbtt_PackBegin(&pc, tmp_bitmap, RANGE, RANGE, RANGE, 1, nullptr);
		stbtt_PackFontRange(&pc, dogica_ttf, 0, 6.0, 32, 96, _cdata.data());
		stbtt_PackEnd(&pc);
		for(int i = 0, j = 0; i < RANGE * RANGE; i++, j += 4)
		{
			vulkan_bitmap[j + 0] = tmp_bitmap[i];
			vulkan_bitmap[j + 1] = tmp_bitmap[i];
			vulkan_bitmap[j + 2] = tmp_bitmap[i];
			vulkan_bitmap[j + 3] = tmp_bitmap[i];
		}
		_atlas.create(vulkan_bitmap, RANGE, RANGE, VK_FORMAT_R8G8B8A8_UNORM, "__mlx_texts_pipeline_texture_atlas", true);
		_atlas.setDescriptor(renderer->getFragDescriptorSet().duplicate());
	}

	void TextPutPipeline::loadFont(const std::filesystem::path& filepath, float scale)
	{
		uint8_t tmp_bitmap[RANGE * RANGE];
		uint8_t vulkan_bitmap[RANGE * RANGE * 4];

		std::ifstream file(filepath, std::ios::binary);
		if(!file.is_open())
		{
			core::error::report(e_kind::error, "Font load : cannot open font file, %s", filepath.string().c_str());
			return;
		}
		std::ifstream::pos_type fileSize = std::filesystem::file_size(filepath);
		file.seekg(0, std::ios::beg);
		std::vector<uint8_t> bytes(fileSize);
		file.read(reinterpret_cast<char*>(bytes.data()), fileSize);
		file.close();

		stbtt_pack_context pc;
		stbtt_PackBegin(&pc, tmp_bitmap, RANGE, RANGE, RANGE, 1, nullptr);
		stbtt_PackFontRange(&pc, bytes.data(), 0, scale, 32, 96, _cdata.data());
		stbtt_PackEnd(&pc);
		for(int i = 0, j = 0; i < RANGE * RANGE; i++, j += 4)
		{
			vulkan_bitmap[j + 0] = tmp_bitmap[i];
			vulkan_bitmap[j + 1] = tmp_bitmap[i];
			vulkan_bitmap[j + 2] = tmp_bitmap[i];
			vulkan_bitmap[j + 3] = tmp_bitmap[i];
		}
		destroy();
		_atlas.create(vulkan_bitmap, RANGE, RANGE, VK_FORMAT_R8G8B8A8_UNORM, "__mlx_texts_pipeline_texture_atlas", true);
		_atlas.setDescriptor(_renderer->getFragDescriptorSet().duplicate());
	}

	void TextPutPipeline::put(int x, int y, int color, std::string str)
	{
		auto res = _drawlist.emplace(std::move(str), color, x, y);
		if(res.second)
			const_cast<TextDrawData&>(*res.first).init(_library, _cdata);
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
		_drawlist.clear();
		_atlas.destroy();
	}
}
