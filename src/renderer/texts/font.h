/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   font.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 21:17:04 by kbz_8             #+#    #+#             */
/*   Updated: 2024/01/16 09:14:53 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_FONT__
#define __MLX_FONT__

#include <array>
#include <stb_truetype.h>
#include <renderer/images/texture_atlas.h>
#include <utils/combine_hash.h>

namespace mlx
{
	class Font
	{
		public:
			Font() = delete;
			Font(class Renderer& renderer, const std::filesystem::path& path, float scale);
			Font(class Renderer& renderer, const std::string& name, const std::vector<uint8_t>& ttf_data, float scale);
			inline const std::string& getName() const { return _name; }
			inline float getScale() const noexcept { return _scale; }
			inline const std::array<stbtt_packedchar, 96>& getCharData() const { return _cdata; }
			inline const TextureAtlas& getAtlas() const noexcept { return _atlas; }
			inline bool operator==(const Font& rhs) const { return rhs._name == _name && rhs._scale == _scale; }
			inline bool operator!=(const Font& rhs) const { return rhs._name != _name || rhs._scale != _scale; }
			~Font();

		private:
			std::array<stbtt_packedchar, 96> _cdata;
			TextureAtlas _atlas;
			std::string _name;
			float _scale = 0;
	};
}

#endif
