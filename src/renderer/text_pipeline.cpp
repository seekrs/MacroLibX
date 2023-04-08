/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text_pipeline.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 16:41:13 by maldavid          #+#    #+#             */
/*   Updated: 2023/04/08 00:48:48 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <renderer/text_pipeline.h>
#include <fstream>

#include <utils/opensans-regular.h>

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

namespace mlx
{
	static uint8_t tmp_bitmap[512 * 512];

	void TextPutPipeline::init(Renderer* renderer) noexcept
	{
		_renderer = renderer;
		stbtt_BakeFontBitmap(opensans_regular_ttf, 0, 32.0, tmp_bitmap, 512, 512, 32, 96, _cdata);
		_atlas.create(reinterpret_cast<uint8_t*>(_cdata), 512, 512, VK_FORMAT_R8G8B8A8_UNORM);
		_atlas.setDescriptor(renderer->getFragDescriptorSet().duplicate());
	}

	void TextPutPipeline::put(int x, int y, int color, std::string str)
	{
		_drawlist.emplace_back(std::move(str), color, x, y);
	}

	void TextPutPipeline::render()
	{
		_atlas.updateSet(0);
		for(auto& draw : _drawlist)
		{
			_atlas.setColor(draw.color);
			for(char c : draw.text)
			{
				if(c < 32 && c > 127)
					continue;

				stbtt_aligned_quad q;
				stbtt_GetBakedQuad(_cdata, 512, 512, c - 32, &draw.x, &draw.y, &q, 1);

				std::array<glm::vec2, 4> pos = {
					glm::vec2{q.x0, q.y0},
					glm::vec2{q.x1, q.y0},
					glm::vec2{q.x1, q.y1},
					glm::vec2{q.x0, q.y1}
				};
				
				std::array<glm::vec2, 4> uv = {
					glm::vec2{q.s0, q.t0},
					glm::vec2{q.s1, q.t0},
					glm::vec2{q.s1, q.t1},
					glm::vec2{q.s0, q.t1}
				};

				_atlas.render(*_renderer, 0, 0, std::move(pos), std::move(uv));
			}
		}
	}

	void TextPutPipeline::destroy() noexcept
	{
		_atlas.destroy();
	}
}
