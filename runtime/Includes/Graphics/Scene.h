#ifndef __MLX_SCENE__
#define __MLX_SCENE__

#include <Renderer/Renderer.h>
#include <Graphics/Text.h>
#include <Graphics/Font.h>
#include <Graphics/Sprite.h>
#include <Graphics/Drawable.h>
#include <Renderer/ViewerData.h>
#include <Maths/Vec4.h>

namespace mlx
{
	class Scene
	{
		public:
			Scene() = default;

			Sprite& CreateSprite(NonOwningPtr<class Texture> texture) noexcept;
			NonOwningPtr<Sprite> GetSpriteFromTextureAndPosition(NonOwningPtr<Texture> texture, const Vec2f& position) const;
			void TryEraseSpriteFromTexture(NonOwningPtr<Texture> texture);
			bool IsTextureAtGivenDrawLayer(NonOwningPtr<Texture> texture, std::uint64_t draw_layer) const;

			Text& CreateText(const std::string& text) noexcept;
			NonOwningPtr<Text> GetTextFromPositionAndColor(const std::string& text, const Vec2f& position, const Vec4f& color) const;
			bool IsTextAtGivenDrawLayer(const std::string& text, std::uint64_t draw_layer) const;

			inline void BindFont(std::shared_ptr<Font> font) { Verify((bool)font, "invalid fond pointer"); p_bound_font = font; }

			void BringToFront(NonOwningPtr<Drawable> drawable);
			void BringToDrawLayer(NonOwningPtr<Drawable> drawable, std::uint64_t draw_layer);

			inline void ResetScene(Vec4f clear) { m_drawables.clear(); m_clear_color = std::move(clear); }
			inline const Vec4f& GetClearColor() const noexcept { return m_clear_color; }

			[[nodiscard]] MLX_FORCEINLINE const std::vector<std::shared_ptr<Drawable>>& GetDrawables() const noexcept { return m_drawables; }
			[[nodiscard]] MLX_FORCEINLINE ViewerData& GetViewerData() noexcept { return m_viewer_data; }

			~Scene() = default;

		private:
			std::vector<std::shared_ptr<Drawable>> m_drawables;
			ViewerData m_viewer_data;
			std::shared_ptr<Font> p_bound_font;
			Vec4f m_clear_color = { 0.0f, 0.0f, 0.0f, 1.0f };
	};
}

#endif
