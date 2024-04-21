/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FontLibrary.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 09:26:03 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/28 22:21:53 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_FONT_LIBRARY__
#define __MLX_FONT_LIBRARY__

#include <Renderer/Texts/Font.h>
#include <Renderer/Core/RenderCore.h>
#include <Utils/Singleton.h>

namespace mlx
{
	using FontID = std::uint32_t;
	constexpr FontID nullfont = 0;

	class FontLibrary : public Singleton<FontLibrary>
	{
		friend class Singleton<FontLibrary>;

		public:
			std::shared_ptr<class Font> GetFontData(FontID id);
			FontID AddFontToLibrary(std::shared_ptr<Font> font);
			void RemoveFontFromLibrary(FontID id);

			void ClearLibrary();

		private:
			FontLibrary() = default;
			~FontLibrary() = default;

		private:
			std::unordered_map<FontID, std::shared_ptr<class Font>> m_cache;
			std::vector<FontID> m_invalid_ids;
			FontID m_current_id = 1;
	};
}

#endif
