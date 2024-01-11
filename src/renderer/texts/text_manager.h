/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text_manager.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 16:24:11 by maldavid          #+#    #+#             */
/*   Updated: 2024/01/11 05:18:42 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_TEXT_MANAGER__
#define __MLX_TEXT_MANAGER__

#include <renderer/renderer.h>
#include <renderer/images/texture_atlas.h>
#include <string>
#include <stb_truetype.h>
#include <cstdint>
#include <unordered_set>
#include <mlx_profile.h>
#include <renderer/texts/text_descriptor.h>
#include <renderer/texts/text_library.h>

namespace mlx
{
	class TextManager
	{
		public:
			TextManager() = default;

			void init(Renderer& renderer) noexcept;
			std::pair<DrawableResource*, bool> registerText(int x, int y, uint32_t color, std::string str);
			inline void clear() { _text_descriptors.clear(); TextLibrary::get().clearLibrary(); }
			void loadFont(Renderer& renderer, const std::filesystem::path& filepath, float scale);
			void destroy() noexcept;

			~TextManager() = default;

		private:
			std::unordered_set<TextDrawDescriptor> _text_descriptors;
			std::unordered_set<Font> _font_set;
			Font* _font_in_use = nullptr;
	};
}

#endif
