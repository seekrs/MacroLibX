/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphics.inl                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 15:13:55 by maldavid          #+#    #+#             */
/*   Updated: 2023/04/02 15:26:16 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Core/Graphics.h>

namespace mlx
{
	int& GraphicsSupport::GetID() noexcept { return m_id; }
	std::shared_ptr<Window> GraphicsSupport::GetWindow() { return p_window; }

	void GraphicsSupport::ClearRenderData() noexcept
	{
		MLX_PROFILE_FUNCTION();
		m_drawlist.clear();
		m_pixel_put_pipeline.Clear();
		m_text_manager.Clear();
		m_texture_registry.Clear();
	}

	void GraphicsSupport::PixelPut(int x, int y, std::uint32_t color) noexcept
	{
		MLX_PROFILE_FUNCTION();
		m_pixel_put_pipeline.SetPixel(x, y, color);
	}

	void GraphicsSupport::StringPut(int x, int y, std::uint32_t color, std::string str)
	{
		MLX_PROFILE_FUNCTION();
		std::pair<NonOwningPtr<DrawableResource>, bool> res = m_text_manager.RegisterText(x, y, color, str);
		if(!res.second) // if this is not a completly new text draw
		{
			auto it = std::find(m_drawlist.begin(), m_drawlist.end(), res.first);
			if(it != m_drawlist.end())
				m_drawlist.erase(it);
		}
		m_drawlist.push_back(res.first);
	}

	void GraphicsSupport::TexturePut(NonOwningPtr<Texture> texture, int x, int y)
	{
		MLX_PROFILE_FUNCTION();
		auto res = m_texture_registry.RegisterTexture(texture, x, y);
		if(!res.second) // if this is not a completly new texture draw
		{
			auto it = std::find(m_drawlist.begin(), m_drawlist.end(), res.first);
			if(it != m_drawlist.end())
				m_drawlist.erase(it);
		}
		m_drawlist.push_back(res.first);
	}

	void GraphicsSupport::LoadFont(const std::filesystem::path& filepath, float scale)
	{
		MLX_PROFILE_FUNCTION();
		m_text_manager.LoadFont(m_renderer, filepath, scale);
	}

	void GraphicsSupport::TryEraseTextureFromManager(NonOwningPtr<Texture> texture) noexcept
	{
		MLX_PROFILE_FUNCTION();
		for(auto it = m_drawlist.begin(); it != m_drawlist.end();)
		{
			if(m_texture_registry.IsTextureKnown(texture))
				it = m_drawlist.erase(it);
			else
				++it;
		}
		m_texture_registry.EraseTextures(texture);
	}
}
