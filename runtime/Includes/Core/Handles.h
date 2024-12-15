#ifndef __MLX_HANDLES__
#define __MLX_HANDLES__

#include <Core/Application.h>
#include <Renderer/Image.h>

extern "C"
{
	struct mlx_context_handler
	{
		mlx::NonOwningPtr<mlx::Application> app;
	};

	struct mlx_window_handler
	{
		int id;
	};

	struct mlx_image_handler
	{
		mlx::NonOwningPtr<mlx::Texture> texture;
	};
}

#endif
