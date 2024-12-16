#ifndef __MLX_PUT_PIXEL_MANAGER__
#define __MLX_PUT_PIXEL_MANAGER__

#include <Renderer/Image.h>

namespace mlx
{
	class PutPixelManager
	{
		public:
			PutPixelManager(NonOwningPtr<class Renderer> renderer) : p_renderer(renderer) {}

			// Returns a valid pointer when a new texture has been created
			NonOwningPtr<Texture> DrawPixel(int x, int y, std::uint64_t draw_layer, mlx_color color);
			NonOwningPtr<Texture> DrawPixelsArray(int x, int y, std::uint64_t draw_layer, mlx_color* pixels, std::size_t pixels_size);
			NonOwningPtr<Texture> DrawPixelsRegion(int x, int y, int w, int h, std::uint64_t draw_layer, mlx_color* pixels);
			void ResetRenderData();

			~PutPixelManager() = default;

		private:
			NonOwningPtr<Texture> GetLayer(std::uint64_t draw_layer, bool& is_newlayer);

		private:
			std::unordered_map<std::uint64_t, NonOwningPtr<Texture>> m_placements;
			std::vector<std::unique_ptr<Texture>> m_textures;
			NonOwningPtr<class Renderer> p_renderer;
			std::size_t m_current_texture_index = 0;
	};
}

#endif
