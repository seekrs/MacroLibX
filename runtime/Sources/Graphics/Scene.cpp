#include <PreCompiled.h>
#include <Graphics/Scene.h>
#include <Renderer/Renderer.h>
#include <Renderer/RenderCore.h>

namespace mlx
{
	Scene::Scene(SceneDescriptor desc)
	: m_descriptor(std::move(desc))
	{
		Verify((bool)m_descriptor.renderer, "invalid renderer");
		m_depth.Init(m_descriptor.renderer->GetSwapchainImages().back().GetWidth(), m_descriptor.renderer->GetSwapchainImages().back().GetHeight());
	}

	Sprite& Scene::CreateSprite(std::shared_ptr<Texture> texture) noexcept
	{
		std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>(texture);
		m_sprites.push_back(sprite);
		return *sprite;
	}
}
