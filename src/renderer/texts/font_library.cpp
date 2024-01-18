/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   font_library.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 09:28:14 by maldavid          #+#    #+#             */
/*   Updated: 2024/01/18 13:07:48 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <renderer/texts/font_library.h>
#include <renderer/texts/font.h>
#include <core/errors.h>
#include <renderer/renderer.h>
#include <algorithm>
#include <core/profiler.h>

namespace mlx
{
	std::shared_ptr<Font> FontLibrary::getFontData(FontID id)
	{
		MLX_PROFILE_FUNCTION();
		if(!_cache.count(id) || std::find(_invalid_ids.begin(), _invalid_ids.end(), id) != _invalid_ids.end())
			core::error::report(e_kind::fatal_error, "Font Library : wrong font ID '%d'", id);
		return _cache[id];
	}

	FontID FontLibrary::addFontToLibrary(std::shared_ptr<Font> font)
	{
		MLX_PROFILE_FUNCTION();
		auto it = std::find_if(_cache.begin(), _cache.end(), [&](const std::pair<FontID, std::shared_ptr<Font>>& v)
		{
			return	v.second->getScale() == font->getScale() &&
					v.second->getName() == font->getName() &&
					std::find(_invalid_ids.begin(), _invalid_ids.end(), v.first) == _invalid_ids.end();
		});
		if(it != _cache.end())
			return it->first;
		font->buildFont();
		_cache[_current_id] = font;
		_current_id++;
		return _current_id - 1;
	}

	void FontLibrary::removeFontFromLibrary(FontID id)
	{
		MLX_PROFILE_FUNCTION();
		if(!_cache.count(id) || std::find(_invalid_ids.begin(), _invalid_ids.end(), id) != _invalid_ids.end())
		{
			core::error::report(e_kind::warning, "Font Library : trying to remove a font with an unkown or invalid ID '%d'", id);
			return;
		}
		_cache[id]->destroy();
		_invalid_ids.push_back(id);
	}

	void FontLibrary::clearLibrary()
	{
		MLX_PROFILE_FUNCTION();
		for(auto& [id, font] : _cache)
		{
			font->destroy();
			_invalid_ids.push_back(id);
		}
		// do not `_cache.clear();` as it releases the fonts and may not destroy the texture atlas that is in use by command buffers
	}
}
