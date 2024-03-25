/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 00:11:56 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/25 16:13:08 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <core/profiler.h>
#include <renderer/texts/text.h>
#include <renderer/renderer.h>

namespace mlx
{
	void Text::init(std::string text, FontID font, std::uint32_t color, std::vector<Vertex> vbo_data, std::vector<std::uint16_t> ibo_data)
	{
		MLX_PROFILE_FUNCTION();
		if(_is_init)
			return;
		_text = std::move(text);
		_color = color;
		_font = font;
		#ifdef DEBUG
			std::string debug_name = _text;
			for(char& c : debug_name)
			{
				if(c == ' ' || c == '"' || c == '\'')
					c = '_';
			}
			for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
				_vbo[i].create(sizeof(Vertex) * vbo_data.size(), static_cast<const void*>(vbo_data.data()), debug_name.c_str());
			_ibo.create(sizeof(std::uint16_t) * ibo_data.size(), ibo_data.data(), debug_name.c_str());
		#else
			for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
				_vbo[i].create(sizeof(Vertex) * vbo_data.size(), static_cast<const void*>(vbo_data.data()), nullptr);
			_ibo.create(sizeof(std::uint16_t) * ibo_data.size(), ibo_data.data(), nullptr);
		#endif
		_is_init = true;
	}

	void Text::bind(Renderer& renderer) noexcept
	{
		MLX_PROFILE_FUNCTION();
		if(!_is_init)
			return;
		_vbo[renderer.getActiveImageIndex()].bind(renderer);
		_ibo.bind(renderer);
	}

	void Text::updateVertexData(int frame, std::vector<Vertex> vbo_data)
	{
		MLX_PROFILE_FUNCTION();
		if(!_is_init)
			return;
		_vbo[frame].setData(sizeof(Vertex) * vbo_data.size(), static_cast<const void*>(vbo_data.data()));
	}

	void Text::destroy() noexcept
	{
		MLX_PROFILE_FUNCTION();
		if(!_is_init)
			return;
		for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
			_vbo[i].destroy();
		_ibo.destroy();
		_is_init = false;
	}

	Text::~Text()
	{
		destroy();
	}
}
