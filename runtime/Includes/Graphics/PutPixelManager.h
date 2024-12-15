#ifndef __MLX_PUT_PIXEL_MANAGER__
#define __MLX_PUT_PIXEL_MANAGER__

#include <Renderer/Image.h>

namespace mlx
{
	class PutPixelManager
	{
		public:
			PutPixelManager(NonOwningPtr<class Renderer> renderer) : p_renderer(renderer) {}

			// Return a valid pointer when a new texture has been created
			NonOwningPtr<Texture> DrawPixel(int x, int y, std::uint64_t draw_layer, int color);
			void ResetRenderData();

			~PutPixelManager();

		private:
			std::unordered_map<std::uint64_t, Texture> m_textures;
			NonOwningPtr<class Renderer> p_renderer;
	};
}

#endif
