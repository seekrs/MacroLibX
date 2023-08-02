/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text_library.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/10 11:59:57 by maldavid          #+#    #+#             */
/*   Updated: 2023/04/19 13:43:43 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <renderer/text_library.h>
#include <core/errors.h>
#include <renderer/renderer.h>
#include <algorithm>

namespace mlx
{
	void TextData::init(std::string text, std::vector<Vertex> vbo_data, std::vector<uint16_t> ibo_data)
	{
		_text = std::move(text);
		_vbo.create(sizeof(Vertex) * vbo_data.size(), vbo_data.data());
		_ibo.create(sizeof(uint16_t) * ibo_data.size(), ibo_data.data());
	}

	void TextData::bind(Renderer& renderer) noexcept
	{
		_vbo.bind(renderer);
		_ibo.bind(renderer);
	}

	void TextData::destroy() noexcept
	{
		_vbo.destroy();
		_ibo.destroy();
	}

	std::shared_ptr<TextData> TextLibrary::getTextData(TextID id)
	{
		if(!_cache.count(id))
			core::error::report(e_kind::fatal_error, "Text Library : wrong text ID '%d'", id);
		return _cache[id];
	}

	TextID TextLibrary::addTextToLibrary(std::shared_ptr<TextData> text)
	{
		auto it = std::find_if(_cache.begin(), _cache.end(), [=](const std::pair<TextID, std::shared_ptr<TextData>>& v)
		{
			return v.second->getText() == text->getText();
		});
		if(it != _cache.end())
			return it->first;
		_cache[_current_id] = text;
		_current_id++;
		return _current_id - 1;
	}

	void TextLibrary::removeTextFromLibrary(TextID id)
	{
		if(_cache.count(id))
		{
			_cache[id]->destroy();
			_cache.erase(id);
		}
	}

	void TextLibrary::clearLibrary()
	{
		for(auto [id, text] : _cache)
			text->destroy();
		_cache.clear();
	}
}
