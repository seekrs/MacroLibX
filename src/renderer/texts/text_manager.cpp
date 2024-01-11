/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text_manager.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 16:41:13 by maldavid          #+#    #+#             */
/*   Updated: 2024/01/11 04:54:16 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <renderer/texts/text_descriptor.h>
#include <renderer/texts/text_library.h>
#include <renderer/texts/text.h>
#include <renderer/texts/text_manager.h>
#include <array>
#include <core/profiler.h>
#include <fstream>

#include <utils/dogica_ttf.h>
#include <cstdio>

namespace mlx
{
	void TextManager::init(Renderer& renderer) noexcept
	{
		MLX_PROFILE_FUNCTION();
		_font_in_use = &const_cast<Font&>(*_font_set.emplace(renderer, "default", dogica_ttf, 6.0f).first);
	}

	void TextManager::loadFont(Renderer& renderer, const std::filesystem::path& filepath, float scale)
	{
		MLX_PROFILE_FUNCTION();
		if(filepath.string() == "default") // we're sure it is already loaded
			_font_in_use = &const_cast<Font&>(*_font_set.emplace(renderer, "default", dogica_ttf, scale).first);
		else
			_font_in_use = &const_cast<Font&>(*_font_set.emplace(renderer, filepath, scale).first);
	}

	std::pair<DrawableResource*, bool> TextManager::registerText(int x, int y, uint32_t color, std::string str)
	{
		MLX_PROFILE_FUNCTION();
		auto res = _text_descriptors.emplace(std::move(str), color, x, y);
		if(res.second)
		{
			const_cast<TextDrawDescriptor&>(*res.first).init(_font_in_use);
			return std::make_pair(static_cast<DrawableResource*>(&const_cast<TextDrawDescriptor&>(*res.first)), true);
		}

		auto text_ptr = TextLibrary::get().getTextData(res.first->id);
		if(*_font_in_use != text_ptr->getFontInUse())
		{
			TextLibrary::get().removeTextFromLibrary(res.first->id);
			const_cast<TextDrawDescriptor&>(*res.first).init(_font_in_use);
		}
		return std::make_pair(static_cast<DrawableResource*>(&const_cast<TextDrawDescriptor&>(*res.first)), false);
	}

	void TextManager::destroy() noexcept
	{
		MLX_PROFILE_FUNCTION();
		_text_descriptors.clear();
		_font_set.clear();
	}
}
