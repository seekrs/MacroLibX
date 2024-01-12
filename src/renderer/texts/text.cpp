/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 00:11:56 by maldavid          #+#    #+#             */
/*   Updated: 2024/01/11 03:31:57 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <core/profiler.h>
#include <renderer/texts/text.h>
#include <renderer/renderer.h>

namespace mlx
{
	void Text::init(std::string text, Font const* font, std::vector<Vertex> vbo_data, std::vector<uint16_t> ibo_data)
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

	void Text::bind(Renderer& renderer) noexcept
	{
		MLX_PROFILE_FUNCTION();
		_vbo[renderer.getActiveImageIndex()].bind(renderer);
		_ibo.bind(renderer);
	}

	void Text::updateVertexData(int frame, std::vector<Vertex> vbo_data)
	{
		MLX_PROFILE_FUNCTION();
		_vbo[frame].setData(sizeof(Vertex) * vbo_data.size(), static_cast<const void*>(vbo_data.data()));
	}

	void Text::destroy() noexcept
	{
		MLX_PROFILE_FUNCTION();
		for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
			_vbo[i].destroy();
		_ibo.destroy();
	}
}
