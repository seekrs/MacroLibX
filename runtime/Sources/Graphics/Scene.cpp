#include <PreCompiled.h>
#include <Graphics/Scene.h>
#include <Renderer/Renderer.h>
#include <Renderer/RenderCore.h>

namespace Scop
{
	Scene::Scene(SceneDescriptor desc)
	: m_descriptor(std::move(desc))
	{
	}

	Sprite& Scene::CreateSprite(std::shared_ptr<Texture> texture) noexcept
	{
		std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>(texture);
		m_sprites.push_back(sprite);
		return *sprite;
	}
}
