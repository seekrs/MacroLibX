#include <PreCompiled.h>
#include <Renderer/ScenesRenderer.h>
#include <Renderer/Renderer.h>
#include <Graphics/Scene.h>
#include <Renderer/ViewerData.h>

namespace mlx
{
	void SceneRenderer::Init()
	{
		m_passes.Init();
	}

	void SceneRenderer::Render(Scene& scene, Renderer& renderer)
	{
		m_passes.Pass(scene, renderer);
	}

	void SceneRenderer::Destroy()
	{
		m_passes.Destroy();
	}
}
