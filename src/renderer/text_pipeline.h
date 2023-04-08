/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text_pipeline.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 16:24:11 by maldavid          #+#    #+#             */
/*   Updated: 2023/04/08 00:46:36 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_TEXT_PIPELINE__
#define __MLX_TEXT_PIPELINE__

#include <renderer/renderer.h>
#include <renderer/images/texture_atlas.h>
#include <string>
#include <stb_truetype.h>
#include <cstdint>
#include <vector>

namespace mlx
{
	class TextPutPipeline
	{
		public:
			TextPutPipeline() = default;

			void init(Renderer* renderer) noexcept;
			void put(int x, int y, int color, std::string str);
			inline VkDescriptorSet getDescriptorSet() noexcept { return _atlas.getSet(); }
			inline void clear() { _drawlist.clear(); }
			void render();
			void destroy() noexcept;

			~TextPutPipeline() = default;

		private:
			struct DrawList
			{
				std::string text;
				float x;
				float y;
				int color;

				DrawList(std::string _text, int _color, int _x, int _y) :
					text(std::move(_text)), color(_color), x(_x), y(_y)
				{}
			};

			stbtt_bakedchar _cdata[96];
			TextureAtlas _atlas;
			std::vector<DrawList> _drawlist;
			Renderer* _renderer = nullptr;
	};
}

#endif
