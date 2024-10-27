#pragma once
#include <Core/Graphics.h>

namespace mlx
{
	void GraphicsSupport::ResetRenderData() noexcept
	{
		MLX_PROFILE_FUNCTION();
		p_scene->ResetScene();
		m_put_pixel_manager.ResetRenderData();
		m_draw_layer = 0;
		m_pixelput_called = false;
	}

	void GraphicsSupport::PixelPut(int x, int y, std::uint32_t color) noexcept
	{
		MLX_PROFILE_FUNCTION();
		NonOwningPtr<Texture> texture = m_put_pixel_manager.DrawPixel(x, y, m_draw_layer, color);
		if(texture)
		{
			m_pixelput_called = true;
			Sprite& new_sprite = p_scene->CreateSprite(texture);
			new_sprite.SetPosition(Vec2f{ 0.0f, 0.0f });
		}
	}

	void GraphicsSupport::StringPut(int x, int y, std::uint32_t color, std::string str)
	{
		MLX_PROFILE_FUNCTION();
		if(str.empty())
			return;

		Vec4f vec_color = {
			static_cast<float>((color & 0x000000FF)) / 255.f,
			static_cast<float>((color & 0x0000FF00) >> 8) / 255.f,
			static_cast<float>((color & 0x00FF0000) >> 16) / 255.f,
			static_cast<float>((color & 0xFF000000) >> 24) / 255.f
		};

		NonOwningPtr<Text> text = p_scene->GetTextFromPositionAndColor(str, Vec2f{ static_cast<float>(x), static_cast<float>(y) }, vec_color);
		if(!text)
		{
			Text& new_text = p_scene->CreateText(str);
			new_text.SetPosition(Vec2f{ static_cast<float>(x), static_cast<float>(y) });
			new_text.SetColor(std::move(vec_color));
		//	if(m_pixelput_called)
			{
			//	m_draw_layer++;
		//		m_pixelput_called = false;
			}
		}
		else if(!p_scene->IsTextAtGivenDrawLayer(str, m_draw_layer))
		{
			p_scene->BringToDrawLayer(text.Get(), m_draw_layer);
			//m_draw_layer++;
		}
	}

	void GraphicsSupport::TexturePut(NonOwningPtr<Texture> texture, int x, int y)
	{
		MLX_PROFILE_FUNCTION();
		NonOwningPtr<Sprite> sprite = p_scene->GetSpriteFromTextureAndPosition(texture, Vec2f{ static_cast<float>(x), static_cast<float>(y) });
		if(!sprite)
		{
			
			Sprite& new_sprite = p_scene->CreateSprite(texture);
			new_sprite.SetPosition(Vec2f{ static_cast<float>(x), static_cast<float>(y) });
			if(m_pixelput_called)
			{
				m_draw_layer++;
				m_pixelput_called = false;
			}
		}
		else if(!p_scene->IsTextureAtGivenDrawLayer(texture, m_draw_layer))
		{
			p_scene->BringToDrawLayer(sprite.Get(), m_draw_layer);
			//m_draw_layer++;
		}
	}

	void GraphicsSupport::TryEraseSpritesInScene(NonOwningPtr<Texture> texture) noexcept
	{
		MLX_PROFILE_FUNCTION();
		p_scene->TryEraseSpriteFromTexture(texture);
	}
}
