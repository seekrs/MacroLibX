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
		PixelPut(0, 0, 0x00000000); // bozoman solution FIXME WTF
	}

	void GraphicsSupport::PixelPut(int x, int y, std::uint32_t color) noexcept
	{
		MLX_PROFILE_FUNCTION();
		NonOwningPtr<Texture> texture = m_put_pixel_manager.DrawPixel(x, y, m_draw_layer, color);
		if(texture)
		{
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
			m_draw_layer++;
		}
		else if(!p_scene->IsTextureAtGivenDrawLayer(texture, m_draw_layer))
			p_scene->BringToFront(std::move(sprite));
	}

	void GraphicsSupport::TryEraseSpritesInScene(NonOwningPtr<Texture> texture) noexcept
	{
		MLX_PROFILE_FUNCTION();
		p_scene->TryEraseSpriteFromTexture(texture);
	}
}
