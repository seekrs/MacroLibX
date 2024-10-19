/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text_library.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/10 11:52:30 by maldavid          #+#    #+#             */
/*   Updated: 2024/10/19 10:49:02 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_TEXT_LIBRARY__
#define __MLX_TEXT_LIBRARY__

#include <renderer/buffers/vk_vbo.h>
#include <renderer/buffers/vk_ibo.h>
#include <unordered_map>
#include <memory>
#include <cstdint>
#include <mlx_profile.h>
#include <renderer/texts/font.h>
#include <renderer/core/render_core.h>
#include <utils/singleton.h>

namespace mlx
{
	using TextID = std::uint32_t;
	constexpr TextID nulltext = 0;

	class TextLibrary : public Singleton<TextLibrary>
	{
		friend class Singleton<TextLibrary>;

		public:
			std::shared_ptr<class Text> getTextData(TextID id);
			TextID addTextToLibrary(std::shared_ptr<Text> text);
			void removeTextFromLibrary(TextID id);

			void clearLibrary();

			inline void reset() { _cache.clear(); _current_id = 1; }

		private:
			TextLibrary() = default;
			~TextLibrary() = default;

		private:
			std::unordered_map<TextID, std::shared_ptr<class Text>> _cache;
			TextID _current_id = 1;
	};
}

#endif
