/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TextManager.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 16:41:13 by maldavid          #+#    #+#             */
/*   Updated: 2024/04/24 01:42:19 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <PreCompiled.h>

#include <Renderer/Texts/TextDescriptor.h>
#include <Renderer/Texts/TextLibrary.h>
#include <Renderer/Texts/Text.h>
#include <Renderer/Texts/TextManager.h>
#include <Utils/DogicaTTF.h>

namespace mlx
{
	void TextManager::Init(Renderer& renderer) noexcept
	{
		MLX_PROFILE_FUNCTION();
		LoadFont(renderer, "default", 6.f);
	}

	void TextManager::LoadFont(Renderer& renderer, const std::filesystem::path& filepath, float scale)
	{
		MLX_PROFILE_FUNCTION();
		std::shared_ptr<Font> font;
		if(filepath.string() == "default")
			font = std::make_shared<Font>(renderer, "default", dogica_ttf, scale);
		else
			font = std::make_shared<Font>(renderer, filepath, scale);
		m_font_in_use = FontLibrary::Get().AddFontToLibrary(font);
	}

	std::pair<DrawableResource*, bool> TextManager::RegisterText(int x, int y, std::uint32_t color, std::string str)
	{
		MLX_PROFILE_FUNCTION();
		auto res = m_text_descriptors.emplace(std::move(str), color, x, y);
		if(res.second)
		{
			const_cast<TextDrawDescriptor&>(*res.first).Init(m_font_in_use);
			return std::make_pair(static_cast<DrawableResource*>(&const_cast<TextDrawDescriptor&>(*res.first)), true);
		}

		auto text_ptr = TextLibrary::Get().GetTextData(res.first->id);
		if(_font_in_use != text_ptr->GetFontInUse())
		{
			// TODO : update text vertex buffers rather than destroying it and recreating it
			TextLibrary::Get().RemoveTextFromLibrary(res.first->id);
			const_cast<TextDrawDescriptor&>(*res.first).Init(_font_in_use);
		}
		return std::make_pair(static_cast<DrawableResource*>(&const_cast<TextDrawDescriptor&>(*res.first)), false);
	}

	void TextManager::Destroy() noexcept
	{
		MLX_PROFILE_FUNCTION();
		m_text_descriptors.clear();
	}
}
