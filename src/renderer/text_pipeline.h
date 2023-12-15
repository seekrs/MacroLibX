/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text_pipeline.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 16:24:11 by maldavid          #+#    #+#             */
/*   Updated: 2023/12/14 17:39:51 by maldavid         ###   ########.fr       */
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
#include <utils/combine_hash.h>

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
		TextDrawData() = default;
	};
}

namespace std
{
	template <>
	struct hash<mlx::TextDrawData>
	{
		std::size_t operator()(const mlx::TextDrawData& d) const noexcept
		{
			std::size_t hash = 0;
			mlx::hashCombine(hash, d.text, d.x, d.y, d.color);
			return hash;
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
			inline void clear() { _drawlist.clear(); _library.clearLibrary(); }
			void loadFont(const std::filesystem::path& filepath, float scale);
			void render(std::array<VkDescriptorSet, 2>& sets);
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
