#ifndef __MLX_SCENE__
#define __MLX_SCENE__

#include <Renderer/Renderer.h>
#include <Graphics/Sprite.h>
#include <Renderer/ViewerData.h>

namespace mlx
{
	class Scene
	{
		public:
			Scene() = default;

			Sprite& CreateSprite(NonOwningPtr<class Texture> texture) noexcept;
			NonOwningPtr<Sprite> GetSpriteFromTextureAndPosition(NonOwningPtr<Texture> texture, const Vec2f& position) const;
			void BringToFront(NonOwningPtr<Sprite> sprite);
			void TryEraseSpriteFromTexture(NonOwningPtr<Texture> texture);
			bool IsTextureAtGivenDrawLayer(NonOwningPtr<Texture> texture, std::uint64_t draw_layer) const;

			inline void ResetSprites() { m_sprites.clear(); }

			[[nodiscard]] MLX_FORCEINLINE const std::vector<std::shared_ptr<Sprite>>& GetSprites() const noexcept { return m_sprites; }
			[[nodiscard]] MLX_FORCEINLINE ViewerData& GetViewerData() noexcept { return m_viewer_data; }

			~Scene() = default;

		private:
			std::vector<std::shared_ptr<Sprite>> m_sprites;
			ViewerData m_viewer_data;
	};
}

#endif
