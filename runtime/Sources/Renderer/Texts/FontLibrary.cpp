/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FontLibrary.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 09:28:14 by maldavid          #+#    #+#             */
/*   Updated: 2024/04/24 01:28:40 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <PreCompiled.h>

#include <Renderer/Texts/FontLibrary.h>
#include <Renderer/Texts/Font.h>
#include <Renderer/Renderer.h>

namespace mlx
{
	std::shared_ptr<Font> FontLibrary::GetFontData(FontID id)
	{
		MLX_PROFILE_FUNCTION();
		if(!m_cache.count(id) || std::find(m_invalid_ids.begin(), m_invalid_ids.end(), id) != m_invalid_ids.end())
			FatalError("Font Library : wrong font ID '%'", id);
		return m_cache[id];
	}

	FontID FontLibrary::AddFontToLibrary(std::shared_ptr<Font> font)
	{
		MLX_PROFILE_FUNCTION();
		auto it = std::find_if(m_cache.begin(), m_cache.end(), [&](const std::pair<FontID, std::shared_ptr<Font>>& v)
		{
			return	v.second->GetScale() == font->GetScale() &&
					v.second->GetName() == font->GetName() &&
					std::find(m_invalid_ids.begin(), m_invalid_ids.end(), v.first) == m_invalid_ids.end();
		});
		if(it != m_cache.end())
			return it->first;
		font->BuildFont();
		m_cache[m_current_id] = font;
		m_current_id++;
		return m_current_id - 1;
	}

	void FontLibrary::RemoveFontFromLibrary(FontID id)
	{
		MLX_PROFILE_FUNCTION();
		if(!m_cache.count(id) || std::find(m_invalid_ids.begin(), m_invalid_ids.end(), id) != m_invalid_ids.end())
		{
			Warning("Font Library : trying to remove a font with an unkown or invalid ID '%'", id);
			return;
		}
		m_cache[id]->Destroy();
		m_invalid_ids.push_back(id);
	}

	void FontLibrary::ClearLibrary()
	{
		MLX_PROFILE_FUNCTION();
		for(auto& [id, font] : m_cache)
		{
			font->Destroy();
			m_invalid_ids.push_back(id);
		}
		// do not `_cache.clear();` as it releases the fonts and may not destroy the texture atlas that is in use by command buffers
	}
}
