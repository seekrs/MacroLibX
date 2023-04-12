/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text_pipeline.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 16:24:11 by maldavid          #+#    #+#             */
/*   Updated: 2023/04/12 13:25:33 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_TEXT_PIPELINE__
#define __MLX_TEXT_PIPELINE__

#include <renderer/renderer.h>
#include <renderer/images/texture_atlas.h>
#include <string>
#include <stb_truetype.h>
#include <cstdint>
#include <unordered_set>
#include <renderer/text_library.h>

namespace mlx
{
	struct TextDrawData
	{
		TextID id;
		int x;
		int y;
		int color;
		std::string text;

		TextDrawData(std::string text, int _color, int _x, int _y);
		void init(TextLibrary& library, std::array<stbtt_bakedchar, 96>& cdata) noexcept;
		bool operator==(const TextDrawData& rhs) const { return text == rhs.text && x == rhs.x && y == rhs.y && color == rhs.color; }
	};
}

namespace std
{
	template <>
	struct hash<mlx::TextDrawData>
	{
		std::size_t operator()(const mlx::TextDrawData& d) const noexcept
		{
			return std::hash<std::string>()(d.text) + std::hash<int>()(d.x) + std::hash<int>()(d.y) + std::hash<int>()(d.color);
		}
	};
}

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
			std::array<stbtt_bakedchar, 96> _cdata;
			TextureAtlas _atlas;
			TextLibrary _library;
			std::unordered_set<TextDrawData> _drawlist;
			Renderer* _renderer = nullptr;
	};
}

#endif
