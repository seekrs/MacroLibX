/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text_manager.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 16:41:13 by maldavid          #+#    #+#             */
/*   Updated: 2024/02/25 09:29:36 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <renderer/texts/text_descriptor.h>
#include <renderer/texts/text_library.h>
#include <renderer/texts/text.h>
#include <renderer/texts/text_manager.h>
#include <core/profiler.h>

#include <utils/dogica_ttf.h>

namespace mlx
{
	void TextManager::init(Renderer& renderer) noexcept
	{
		MLX_PROFILE_FUNCTION();
		loadFont(renderer, "default", 6.f);
	}

	void TextManager::loadFont(Renderer& renderer, const std::filesystem::path& filepath, float scale)
	{
		MLX_PROFILE_FUNCTION();
		std::shared_ptr<Font> font;
		if(filepath.string() == "default")
			font = std::make_shared<Font>(renderer, "default", dogica_ttf, scale);
		else
			font = std::make_shared<Font>(renderer, filepath, scale);

		_font_in_use = FontLibrary::get().addFontToLibrary(font);
	}

	std::pair<DrawableResource*, bool> TextManager::registerText(int x, int y, std::uint32_t color, std::string str)
	{
		MLX_PROFILE_FUNCTION();
		auto res = _text_descriptors.emplace(std::move(str), color, x, y);
		if(res.second)
		{
			const_cast<TextDrawDescriptor&>(*res.first).init(_font_in_use);
			return std::make_pair(static_cast<DrawableResource*>(&const_cast<TextDrawDescriptor&>(*res.first)), true);
		}

		auto text_ptr = TextLibrary::get().getTextData(res.first->id);
		if(_font_in_use != text_ptr->getFontInUse())
		{
			// TODO : update text vertex buffers rather than destroying it and recreating it
			TextLibrary::get().removeTextFromLibrary(res.first->id);
			const_cast<TextDrawDescriptor&>(*res.first).init(_font_in_use);
		}
		return std::make_pair(static_cast<DrawableResource*>(&const_cast<TextDrawDescriptor&>(*res.first)), false);
	}

	void TextManager::destroy() noexcept
	{
		MLX_PROFILE_FUNCTION();
		_text_descriptors.clear();
	}
}
