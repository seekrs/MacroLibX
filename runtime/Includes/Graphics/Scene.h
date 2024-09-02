#ifndef __MLX_SCENE__
#define __MLX_SCENE__

#include <Graphics/Sprite.h>
#include <Renderer/ViewerData.h>

namespace mlx
{
	struct SceneDescriptor
	{
		NonOwningPtr<Renderer> renderer;
		// More description may come in future
	};

	class Scene
	{
		public:
			Scene(SceneDescriptor desc);

			Sprite& CreateSprite(std::shared_ptr<class Texture> texture) noexcept;

			inline void ResetSprites() { m_sprites.clear(); }

			[[nodiscard]] MLX_FORCEINLINE const std::vector<std::shared_ptr<Sprite>>& GetSprites() const noexcept { return m_sprites; }
			[[nodiscard]] MLX_FORCEINLINE const SceneDescriptor& GetDescription() const noexcept { return m_descriptor; }
			[[nodiscard]] MLX_FORCEINLINE DepthImage& GetDepth() noexcept { return m_depth; }
			[[nodiscard]] MLX_FORCEINLINE ViewerData& GetViewerData() noexcept { return m_viewer_data; }

			~Scene() = default;

		private:
			SceneDescriptor m_descriptor;
			std::vector<std::shared_ptr<Sprite>> m_sprites;
			DepthImage m_depth;
			ViewerData m_viewer_data;
	};
}

#endif
