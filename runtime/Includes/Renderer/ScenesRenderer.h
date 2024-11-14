#ifndef __MLX_SCENES_RENDERER__
#define __MLX_SCENES_RENDERER__

#include <Renderer/RenderPasses/Passes.h>

namespace mlx
{
	class SceneRenderer
	{
		public:
			SceneRenderer() = default;
			void Init(NonOwningPtr<class Texture> render_target);
			void Render(class Scene& scene, class Renderer& renderer);
			void Destroy();
			~SceneRenderer() = default;

		private:
			RenderPasses m_passes;
	};
}

#endif
