/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text_manager.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 16:24:11 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/25 17:57:35 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_TEXT_MANAGER__
#define __MLX_TEXT_MANAGER__

#include <pre_compiled.h>

#include <renderer/renderer.h>
#include <renderer/images/texture_atlas.h>
#include <renderer/texts/text_descriptor.h>
#include <renderer/texts/text_library.h>
#include <renderer/texts/font_library.h>

namespace mlx
{
	class TextManager
	{
		public:
			TextManager() = default;

			void init(Renderer& renderer) noexcept;
			std::pair<DrawableResource*, bool> registerText(int x, int y, std::uint32_t color, std::string str);
			inline void clear() { _text_descriptors.clear(); }
			void loadFont(Renderer& renderer, const std::filesystem::path& filepath, float scale);
			void destroy() noexcept;

			~TextManager() = default;

		private:
			std::unordered_set<TextDrawDescriptor> _text_descriptors;
			FontID _font_in_use = nullfont;
	};
}

#endif
