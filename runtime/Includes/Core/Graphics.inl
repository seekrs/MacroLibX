#pragma once
#include <Core/Graphics.h>

namespace mlx
{
	void GraphicsSupport::TryEraseSpritesInScene(NonOwningPtr<Texture> texture) noexcept
	{
		MLX_PROFILE_FUNCTION();
		p_scene->TryEraseSpriteFromTexture(texture);
	}
}
