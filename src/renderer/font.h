/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   font.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 21:17:04 by kbz_8             #+#    #+#             */
/*   Updated: 2023/12/13 00:25:30 by kbz_8            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_FONT__
#define __MLX_FONT__

#include <array>
#include <stb_truetype.h>
#include <utils/non_copyable.h>
#include <renderer/images/texture_atlas.h>

namespace mlx
{
	class Font : public non_copyable
	{
		public:
			Font(class Renderer& renderer, const std::filesystem::path& path, float scale);
			Font(class Renderer& renderer, const std::string& name, const std::vector<uint8_t>& ttf_data, float scale);
			inline const std::string& getName() const { return _name; }
			inline const std::array<stbtt_packedchar, 96>& getCharData() const { return _cdata; }
			inline VkDescriptorSet getDescriptorSet() noexcept { return _atlas.getSet(); }
			inline bool operator==(const Font& rhs) { return rhs._name == _name; }
			~Font();

		private:
			std::array<stbtt_packedchar, 96> _cdata;
			TextureAtlas _atlas;
			std::string _name;
	};
}

namespace std
{
	template <>
	struct hash<mlx::Font>
	{
		std::size_t operator()(const mlx::Font& f) const noexcept
		{
			return std::hash<std::string>()(f.getName());
		}
	};
}

#endif
