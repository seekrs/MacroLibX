/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text_library.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/10 11:59:57 by maldavid          #+#    #+#             */
/*   Updated: 2024/01/10 18:26:44 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <renderer/text_library.h>
#include <core/errors.h>
#include <renderer/renderer.h>
#include <algorithm>
#include <core/profiler.h>

namespace mlx
{
	void TextData::init(std::string text, Font const* font, std::vector<Vertex> vbo_data, std::vector<uint16_t> ibo_data)
	{
		MLX_PROFILE_FUNCTION();
		_text = std::move(text);
		_font = font;
		#ifdef DEBUG
			for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
				_vbo[i].create(sizeof(Vertex) * vbo_data.size(), static_cast<const void*>(vbo_data.data()), _text.c_str());
			_ibo.create(sizeof(uint16_t) * ibo_data.size(), ibo_data.data(), _text.c_str());
		#else
			for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
				_vbo[i].create(sizeof(Vertex) * vbo_data.size(), static_cast<const void*>(vbo_data.data()), nullptr);
			_ibo.create(sizeof(uint16_t) * ibo_data.size(), ibo_data.data(), nullptr);
		#endif
	}

	void TextData::bind(Renderer& renderer) noexcept
	{
		MLX_PROFILE_FUNCTION();
		_vbo[renderer.getActiveImageIndex()].bind(renderer);
		_ibo.bind(renderer);
	}

	void TextData::updateVertexData(int frame, std::vector<Vertex> vbo_data)
	{
		MLX_PROFILE_FUNCTION();
		_vbo[frame].setData(sizeof(Vertex) * vbo_data.size(), static_cast<const void*>(vbo_data.data()));
	}

	void TextData::destroy() noexcept
	{
		MLX_PROFILE_FUNCTION();
		for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
			_vbo[i].destroy();
		_ibo.destroy();
	}

	std::shared_ptr<TextData> TextLibrary::getTextData(TextID id)
	{
		MLX_PROFILE_FUNCTION();
		if(!_cache.count(id))
			core::error::report(e_kind::fatal_error, "Text Library : wrong text ID '%d'", id);
		return _cache[id];
	}

	TextID TextLibrary::addTextToLibrary(std::shared_ptr<TextData> text)
	{
		MLX_PROFILE_FUNCTION();
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
		MLX_PROFILE_FUNCTION();
		if(_cache.count(id))
		{
			_cache[id]->destroy();
			_cache.erase(id);
		}
	}

	void TextLibrary::clearLibrary()
	{
		MLX_PROFILE_FUNCTION();
		for(auto [id, text] : _cache)
			text->destroy();
		_cache.clear();
	}
}
