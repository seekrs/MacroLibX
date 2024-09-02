#ifndef __MLX_SCENE__
#define __MLX_SCENE__

#include <Graphics/Sprite.h>

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

			[[nodiscard]] inline const std::vector<std::shared_ptr<Sprite>>& GetSprites() const noexcept { return m_sprites; }
			[[nodiscard]] inline const SceneDescriptor& GetDescription() const noexcept { return m_descriptor; }

			~Scene() = default;

		private:
			SceneDescriptor m_descriptor;
			std::vector<std::shared_ptr<Sprite>> m_sprites;
	};
}

#endif
