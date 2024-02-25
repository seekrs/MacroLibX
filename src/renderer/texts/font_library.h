/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   font_library.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 09:26:03 by maldavid          #+#    #+#             */
/*   Updated: 2024/01/18 09:33:30 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_FONT_LIBRARY__
#define __MLX_FONT_LIBRARY__

#include <unordered_map>
#include <memory>
#include <vector>
#include <cstdint>
#include <mlx_profile.h>
#include <renderer/texts/font.h>
#include <renderer/core/render_core.h>
#include <utils/singleton.h>

namespace mlx
{
	using FontID = std::uint32_t;
	constexpr FontID nullfont = 0;

	class FontLibrary : public Singleton<FontLibrary>
	{
		friend class Singleton<FontLibrary>;

		public:
			std::shared_ptr<class Font> getFontData(FontID id);
			FontID addFontToLibrary(std::shared_ptr<Font> font);
			void removeFontFromLibrary(FontID id);

			void clearLibrary();

		private:
			FontLibrary() = default;
			~FontLibrary() = default;

		private:
			std::unordered_map<FontID, std::shared_ptr<class Font>> _cache;
			std::vector<FontID> _invalid_ids;
			FontID _current_id = 1;
	};
}

#endif
