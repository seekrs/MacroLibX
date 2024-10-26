#ifndef __MLX_SCENE__
#define __MLX_SCENE__

#include <Renderer/Renderer.h>
#include <Graphics/Text.h>
#include <Graphics/Font.h>
#include <Graphics/Sprite.h>
#include <Graphics/Drawable.h>
#include <Renderer/ViewerData.h>

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

			inline void ResetScene() { m_drawables.clear(); }

			[[nodiscard]] MLX_FORCEINLINE const std::vector<std::shared_ptr<Drawable>>& GetDrawables() const noexcept { return m_drawables; }
			[[nodiscard]] MLX_FORCEINLINE ViewerData& GetViewerData() noexcept { return m_viewer_data; }

			~Scene() = default;

		private:
			std::vector<std::shared_ptr<Drawable>> m_drawables;
			ViewerData m_viewer_data;
			std::shared_ptr<Font> p_bound_font;
	};
}

#endif
