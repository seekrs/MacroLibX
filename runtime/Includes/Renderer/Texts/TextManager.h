/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TextManager.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 16:24:11 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/28 22:27:32 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_TEXT_MANAGER__
#define __MLX_TEXT_MANAGER__

#include <Renderer/Renderer.h>
#include <Renderer/Images/TextureAtlas.h>
#include <Renderer/Texts/TextDescriptor.h>
#include <Renderer/Texts/TextLibrary.h>
#include <Renderer/Texts/FontLibrary.h>

namespace mlx
{
	class TextManager
	{
		public:
			TextManager() = default;

			void Init(Renderer& renderer) noexcept;
			std::pair<NonOwningPtr<DrawableResource>, bool> RegisterText(int x, int y, std::uint32_t color, std::string str);
			inline void Clear() { m_text_descriptors.clear(); }
			void LoadFont(Renderer& renderer, const std::filesystem::path& filepath, float scale);
			void Destroy() noexcept;

			~TextManager() = default;

		private:
			std::unordered_set<TextDrawDescriptor> m_text_descriptors;
			FontID m_font_in_use = nullfont;
	};
}

#endif
