/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text_library.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/10 11:59:57 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/25 16:17:06 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <renderer/texts/text_library.h>
#include <renderer/texts/text.h>
#include <core/errors.h>
#include <renderer/renderer.h>
#include <algorithm>
#include <core/profiler.h>

namespace mlx
{
	std::shared_ptr<Text> TextLibrary::getTextData(TextID id)
	{
		MLX_PROFILE_FUNCTION();
		if(!_cache.count(id))
			core::error::report(e_kind::fatal_error, "Text Library : wrong text ID '%d'", id);
		return _cache[id];
	}

	TextID TextLibrary::addTextToLibrary(std::shared_ptr<Text> text)
	{
		MLX_PROFILE_FUNCTION();
		auto it = std::find_if(_cache.begin(), _cache.end(), [&](const std::pair<TextID, std::shared_ptr<Text>>& v)
		{
			return v.second->getText() == text->getText() && v.second->getColor() == text->getColor();
		});
		if(it != _cache.end())
			return it->first;
		_cache[_current_id] = text;
		_current_id++;
		return _current_id - 1;
	}

	void TextLibrary::removeTextFromLibrary(TextID id)
	{
		MLX_PROFILE_FUNCTION();
		if(!_cache.count(id))
		{
			core::error::report(e_kind::warning, "Text Library : trying to remove a text with an unkown or invalid ID '%d'", id);
			return;
		}
		_cache[id]->destroy();
		_cache.erase(id);
	}

	void TextLibrary::clearLibrary()
	{
		MLX_PROFILE_FUNCTION();
		for(auto& [id, text] : _cache)
			text->destroy();
		_cache.clear();
	}
}
