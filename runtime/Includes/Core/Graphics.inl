#pragma once
#include <Core/Graphics.h>

namespace mlx
{
	void GraphicsSupport::ResetRenderData() noexcept
	{
		MLX_PROFILE_FUNCTION();
		p_scene->ResetSprites();
		m_put_pixel_manager.ResetRenderData();
		m_draw_layer = 0;
		m_putpixel_called = false;
		// PixelPut(0, 0, 0x00000000); // bozoman solution FIXME
	}

	void GraphicsSupport::PixelPut(int x, int y, std::uint32_t color) noexcept
	{
		MLX_PROFILE_FUNCTION();
		NonOwningPtr<Texture> texture = m_put_pixel_manager.DrawPixel(x, y, m_draw_layer, color);
		if(texture)
		{
			m_putpixel_called = true;
			Sprite& new_sprite = p_scene->CreateSprite(texture);
			new_sprite.SetPosition(Vec2f{ 0.0f, 0.0f });
		}
	}

	void GraphicsSupport::StringPut(int x, int y, std::uint32_t color, std::string str)
	{
		MLX_PROFILE_FUNCTION();
		(void)x;
		(void)y;
		(void)color;
		(void)str;
	}

	void GraphicsSupport::TexturePut(NonOwningPtr<Texture> texture, int x, int y)
	{
		MLX_PROFILE_FUNCTION();
		NonOwningPtr<Sprite> sprite = p_scene->GetSpriteFromTextureAndPosition(texture, Vec2f{ static_cast<float>(x), static_cast<float>(y) });
		if(!sprite)
		{
			
			Sprite& new_sprite = p_scene->CreateSprite(texture);
			new_sprite.SetPosition(Vec2f{ static_cast<float>(x), static_cast<float>(y) });
			if (m_putpixel_called)
			{
				m_draw_layer++;
				m_putpixel_called = false;
			}
		}
		else if(!p_scene->IsTextureAtGivenDrawLayer(texture, m_draw_layer))
		{
			p_scene->BringToFront(std::move(sprite));
			m_draw_layer++;
		}
	}

	void GraphicsSupport::LoadFont(const std::filesystem::path& filepath, float scale)
	{
		MLX_PROFILE_FUNCTION();
		(void)filepath;
		(void)scale;
	}

	void GraphicsSupport::TryEraseSpritesInScene(NonOwningPtr<Texture> texture) noexcept
	{
		MLX_PROFILE_FUNCTION();
		p_scene->TryEraseSpriteFromTexture(texture);
	}
}
