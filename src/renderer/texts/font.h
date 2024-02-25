/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   font.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 21:17:04 by kbz_8             #+#    #+#             */
/*   Updated: 2024/01/18 13:15:55 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_FONT__
#define __MLX_FONT__

#include <array>
#include <stb_truetype.h>
#include <renderer/images/texture_atlas.h>
#include <utils/combine_hash.h>
#include <variant>

namespace mlx
{
	class Font
	{
		friend class FontLibrary;
		public:
			Font() = delete;
			Font(class Renderer& renderer, const std::filesystem::path& path, float scale);
			Font(class Renderer& renderer, const std::string& name, const std::vector<std::uint8_t>& ttf_data, float scale);

			inline const std::string& getName() const { return _name; }
			inline float getScale() const noexcept { return _scale; }
			inline const std::array<stbtt_packedchar, 96>& getCharData() const { return _cdata; }
			inline const TextureAtlas& getAtlas() const noexcept { return _atlas; }
			inline bool operator==(const Font& rhs) const { return rhs._name == _name && rhs._scale == _scale; }
			inline bool operator!=(const Font& rhs) const { return rhs._name != _name || rhs._scale != _scale; }
			void destroy();

			~Font();

		private:
			void buildFont();

		private:
			std::array<stbtt_packedchar, 96> _cdata;
			TextureAtlas _atlas;
			std::variant<std::filesystem::path, std::vector<std::uint8_t>> _build_data;
			std::string _name;
			class Renderer& _renderer;
			float _scale = 0;
			bool _is_init = false;
	};
}

#endif
