/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TextLibrary.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/10 11:59:57 by maldavid          #+#    #+#             */
/*   Updated: 2024/04/24 01:40:28 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <PreCompiled.h>

#include <Renderer/Texts/TextLibrary.h>
#include <Renderer/Texts/Text.h>
#include <Renderer/Renderer.h>

namespace mlx
{
	std::shared_ptr<Text> TextLibrary::GetTextData(TextID id)
	{
		MLX_PROFILE_FUNCTION();
		if(!m_cache.count(id))
			FatalError("Text Library : wrong text ID '%d'", id);
		return m_cache[id];
	}

	TextID TextLibrary::AddTextToLibrary(std::shared_ptr<Text> text)
	{
		MLX_PROFILE_FUNCTION();
		auto it = std::find_if(m_cache.begin(), m_cache.end(), [&](const std::pair<TextID, std::shared_ptr<Text>>& v)
		{
			return v.second->GetText() == text->GetText() && v.second->GetColor() == text->GetColor();
		});
		if(it != m_cache.end())
			return it->first;
		m_cache[m_current_id] = text;
		m_current_id++;
		return m_current_id - 1;
	}

	void TextLibrary::RemoveTextFromLibrary(TextID id)
	{
		MLX_PROFILE_FUNCTION();
		if(!m_cache.count(id))
		{
			Warning("Text Library : trying to remove a text with an unkown or invalid ID '%d'", id);
			return;
		}
		m_cache[id]->Destroy();
		m_cache.erase(id);
	}

	void TextLibrary::ClearLibrary()
	{
		MLX_PROFILE_FUNCTION();
		for(auto& [id, text] : m_cache)
			text->Destroy();
		m_cache.clear();
	}
}
