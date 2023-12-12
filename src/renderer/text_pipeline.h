/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text_pipeline.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 16:24:11 by maldavid          #+#    #+#             */
/*   Updated: 2023/12/13 00:24:21 by kbz_8            ###   ########.fr       */
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
#include <mlx_profile.h>

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
		void init(TextLibrary& library, Font* const font) noexcept;
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
			inline void clear() { _drawlist.clear(); }
			void loadFont(const std::filesystem::path& filepath, float scale);
			void render();
			void destroy() noexcept;

			~TextPutPipeline() = default;

		private:
			std::unordered_set<Font> _font_set;
			TextLibrary _library;
			std::unordered_set<TextDrawData> _drawlist;
			Renderer* _renderer = nullptr;
			Font* _font_in_use = nullptr;
	};
}

#endif
