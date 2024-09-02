#pragma once
#include <Core/Graphics.h>

namespace mlx
{
	void GraphicsSupport::ResetRenderData() noexcept
	{
		MLX_PROFILE_FUNCTION();
		p_scene->ResetSprites();
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
	}

	void GraphicsSupport::LoadFont(const std::filesystem::path& filepath, float scale)
	{
		MLX_PROFILE_FUNCTION();
	}

	void GraphicsSupport::TryEraseTextureFromRegistry(NonOwningPtr<Texture> texture) noexcept
	{
		MLX_PROFILE_FUNCTION();
	}
}
