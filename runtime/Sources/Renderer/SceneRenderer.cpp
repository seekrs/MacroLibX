#include <PreCompiled.h>
#include <Renderer/ScenesRenderer.h>
#include <Renderer/Renderer.h>
#include <Graphics/Scene.h>
#include <Renderer/ViewerData.h>

namespace mlx
{
	void SceneRenderer::Init(NonOwningPtr<class Texture> render_target)
	{
		MLX_PROFILE_FUNCTION();
		m_passes.Init(render_target);
	}

	void SceneRenderer::Render(Scene& scene, Renderer& renderer)
	{
		MLX_PROFILE_FUNCTION();
		m_passes.Pass(scene, renderer, scene.GetClearColor());
	}

	void SceneRenderer::Destroy()
	{
		MLX_PROFILE_FUNCTION();
		m_passes.Destroy();
	}
}
