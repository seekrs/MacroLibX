#ifndef __MLX_PASSES__
#define __MLX_PASSES__

#include <Renderer/Image.h>
#include <Renderer/RenderPasses/FinalPass.h>
#include <Renderer/RenderPasses/2DPass.h>

namespace mlx
{
	class RenderPasses
	{
		public:
			RenderPasses() = default;

			void Init(NonOwningPtr<class Texture> render_target);
			void Pass(class Scene& scene, class Renderer& renderer, const Vec4f& clear_color);
			void Destroy();

			~RenderPasses() = default;

		private:
			Render2DPass m_2Dpass;
			FinalPass m_final;
			Texture m_main_render_texture;
			NonOwningPtr<class Texture> p_render_target;
	};
}

#endif
