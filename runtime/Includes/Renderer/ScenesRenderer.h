#ifndef __MLX_SCENES_RENDERER__
#define __MLX_SCENES_RENDERER__

#include <Renderer/RenderPasses/Passes.h>

namespace mlx
{
	class SceneRenderer
	{
		public:
			SceneRenderer() = default;
			void Init(class Renderer& renderer);
			void Render(class Scene& scene, class Renderer& renderer); // TODO : add RTT support
			void Destroy();
			~SceneRenderer() = default;

		private:
			RenderPasses m_passes;
	};
}

#endif