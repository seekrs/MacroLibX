#ifndef __MLX_GRAPHICS__
#define __MLX_GRAPHICS__

#include <mlx.h>
#include <Platform/Window.h>
#include <Renderer/Renderer.h>
#include <Graphics/Scene.h>
#include <Graphics/Sprite.h>
#include <Graphics/PutPixelManager.h>
#include <Renderer/ScenesRenderer.h>
#include <Maths/Mat4.h>

namespace mlx
{
	class GraphicsSupport : public NonCopyable
	{
		public:
			GraphicsSupport(const mlx_window_create_info* info, int id);

			[[nodiscard]] MLX_FORCEINLINE int& GetID() noexcept { return m_id; }
			[[nodiscard]] inline std::shared_ptr<Window> GetWindow() { return p_window; }

			void Render() noexcept;

			void ResetRenderData(mlx_color color) noexcept;

			void PixelPut(int x, int y, mlx_color color) noexcept;
			void PixelPutArray(int x, int y, mlx_color* color, std::size_t pixels_size) noexcept;
			void PixelPutRegion(int x, int y, int w, int h, mlx_color* color) noexcept;
			void StringPut(int x, int y, mlx_color color, std::string str);
			void TexturePut(NonOwningPtr<class Texture> texture, int x, int y, float scale_x, float scale_y, float angle);

			inline void TryEraseSpritesInScene(NonOwningPtr<Texture> texture) noexcept;

			[[nodiscard]] MLX_FORCEINLINE bool HasWindow() const noexcept  { return m_has_window; }
			[[nodiscard]] MLX_FORCEINLINE Renderer& GetRenderer() { return m_renderer; }
			[[nodiscard]] MLX_FORCEINLINE Scene& GetScene() { return *p_scene; }

			~GraphicsSupport();

		private:
			Renderer m_renderer;
			SceneRenderer m_scene_renderer;
			PutPixelManager m_put_pixel_manager;
			std::shared_ptr<Window> p_window;
			std::unique_ptr<Scene> p_scene;

			std::uint64_t m_draw_layer = 0;

			int m_id;

			bool m_has_window;
			bool m_pixelput_called = false;
	};
}

#include <Core/Graphics.inl>

#endif
