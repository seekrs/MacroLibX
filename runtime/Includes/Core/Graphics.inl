#pragma once
#include <Core/Graphics.h>

namespace mlx
{
	void GraphicsSupport::ResetRenderData() noexcept
	{
		MLX_PROFILE_FUNCTION();
		p_scene->ResetSprites();
		m_current_depth = 0;
	}

	void GraphicsSupport::PixelPut(int x, int y, std::uint32_t color) noexcept
	{
		MLX_PROFILE_FUNCTION();
	}

	void GraphicsSupport::StringPut(int x, int y, std::uint32_t color, std::string str)
	{
		MLX_PROFILE_FUNCTION();
	}

	void GraphicsSupport::TexturePut(NonOwningPtr<Texture> texture, int x, int y)
	{
		MLX_PROFILE_FUNCTION();
		NonOwningPtr<Sprite> sprite = p_scene->GetSpriteFromTextureAndPosition(texture, Vec2f{ static_cast<float>(x), static_cast<float>(y) });
		if(!sprite)
		{
			Sprite& new_sprite = p_scene->CreateSprite(texture);
			new_sprite.SetPosition(Vec3f{ static_cast<float>(x), static_cast<float>(y), static_cast<float>(m_current_depth) });
		}
		else
			sprite->SetPosition(Vec3f{ static_cast<float>(x), static_cast<float>(y), static_cast<float>(m_current_depth) });
		m_current_depth++; 
	}

	void GraphicsSupport::LoadFont(const std::filesystem::path& filepath, float scale)
	{
		MLX_PROFILE_FUNCTION();
	}

	void GraphicsSupport::TryEraseSpritesInScene(NonOwningPtr<Texture> texture) noexcept
	{
		MLX_PROFILE_FUNCTION();
		p_scene->TryEraseSpriteFromTexture(texture);
	}
}
