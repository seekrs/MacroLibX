/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_library.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 14:24:00 by maldavid          #+#    #+#             */
/*   Updated: 2023/04/11 18:30:30 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <core/errors.h>
#include <algorithm>
#include <renderer/texture_library.h>

namespace mlx
{
	std::shared_ptr<Texture> TextureLibrary::getTexture(TextureID id)
	{
		if(!_cache.count(id))
			core::error::report(e_kind::fatal_error, "Texture Library : wrong texture ID '%d'", id);
		return _cache[id];
	}

	TextureID TextureLibrary::addTextureToLibrary(std::shared_ptr<Texture> texture)
	{
		auto it = std::find_if(_cache.begin(), _cache.end(), [=](const std::pair<TextureID, std::shared_ptr<Texture>>& v)
		{
			return v.second == texture;
		});
		if(it != _cache.end())
			return it->first;
		_cache[_current_id] = texture;
		_current_id++;
		return _current_id - 1;
	}

	void TextureLibrary::removeTextureFromLibrary(TextureID id)
	{
		if(_cache.count(id))
		{
			_cache[id]->destroy();
			_cache.erase(id);
		}
	}

	void TextureLibrary::clearLibrary()
	{
		for(auto [id, texture] : _cache)
			texture->destroy();
		_cache.clear();
	}
}
