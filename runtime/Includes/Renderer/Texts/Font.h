/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Font.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 21:17:04 by kbz_8             #+#    #+#             */
/*   Updated: 2024/07/05 13:53:11 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_FONT__
#define __MLX_FONT__

#include <Renderer/Images/TextureAtlas.h>
#include <Utils/CombineHash.h>

namespace mlx
{
	class Font
	{
		friend class FontLibrary;

		public:
			Font() = delete;
			Font(class Renderer& renderer, const std::filesystem::path& path, float scale);
			Font(class Renderer& renderer, const std::string& name, const std::vector<std::uint8_t>& ttf_data, float scale);

			inline const std::string& GetName() const { return m_name; }
			inline float GetScale() const noexcept { return m_scale; }
			inline const std::array<stbtt_packedchar, 96>& GetCharData() const { return m_cdata; }
			inline const TextureAtlas& GetAtlas() const noexcept { return m_atlas; }
			inline bool operator==(const Font& rhs) const { return rhs.m_name == m_name && rhs.m_scale == m_scale; }
			inline bool operator!=(const Font& rhs) const { return rhs.m_name != m_name || rhs.m_scale != m_scale; }

			void Destroy();

			~Font();

		private:
			void BuildFont();

		private:
			std::array<stbtt_packedchar, 96> m_cdata;
			TextureAtlas m_atlas;
			std::variant<std::filesystem::path, std::vector<std::uint8_t>> m_build_data;
			std::string m_name;
			class Renderer& m_renderer;
			float m_scale = 0;
			bool m_is_init = false;
	};
}

#endif
