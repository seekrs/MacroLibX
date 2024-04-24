/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Text.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 00:11:56 by maldavid          #+#    #+#             */
/*   Updated: 2024/04/24 01:33:58 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <PreCompiled.h>

#include <Renderer/Texts/Text.h>
#include <Renderer/Renderer.h>

namespace mlx
{
	void Text::Init(std::string text, FontID font, std::uint32_t color, std::vector<Vertex> vbo_data, std::vector<std::uint16_t> ibo_data)
	{
		MLX_PROFILE_FUNCTION();
		if(m_is_init)
			return;
		m_text = std::move(text);
		m_color = color;
		m_font = font;
		#ifdef DEBUG
			std::string debug_name = m_text;
			for(char& c : debug_name)
			{
				if(c == ' ' || c == '"' || c == '\'')
					c = '_';
			}
			for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
				m_vbo[i].Create(sizeof(Vertex) * vbo_data.size(), static_cast<const void*>(vbo_data.data()), debug_name.c_str());
			m_ibo.Create(sizeof(std::uint16_t) * ibo_data.size(), ibo_data.data(), debug_name.c_str());
		#else
			for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
				m_vbo[i].Create(sizeof(Vertex) * vbo_data.size(), static_cast<const void*>(vbo_data.data()), nullptr);
			m_ibo.Create(sizeof(std::uint16_t) * ibo_data.size(), ibo_data.data(), nullptr);
		#endif
		m_is_init = true;
	}

	void Text::Bind(Renderer& renderer) noexcept
	{
		MLX_PROFILE_FUNCTION();
		if(!m_is_init)
			return;
		m_vbo[renderer.GetActiveImageIndex()].Bind(renderer);
		m_ibo.Bind(renderer);
	}

	void Text::updateVertexData(int frame, std::vector<Vertex> vbo_data)
	{
		MLX_PROFILE_FUNCTION();
		if(!m_is_init)
			return;
		m_vbo[frame].SetData(sizeof(Vertex) * vbo_data.size(), static_cast<const void*>(vbo_data.data()));
	}

	void Text::destroy() noexcept
	{
		MLX_PROFILE_FUNCTION();
		if(!m_is_init)
			return;
		for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
			m_vbo[i].Destroy();
		m_ibo.Destroy();
		m_is_init = false;
	}

	Text::~Text()
	{
		Destroy();
	}
}
