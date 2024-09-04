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

	Sprite& Scene::CreateSprite(NonOwningPtr<Texture> texture) noexcept
	{
		std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>(texture);
		m_sprites.push_back(sprite);
		return *sprite;
	}

	NonOwningPtr<Sprite> Scene::GetSpriteFromTextureAndPosition(NonOwningPtr<Texture> texture, const Vec2f& position) const
	{
		auto it = std::find_if(m_sprites.begin(), m_sprites.end(), [texture, position](const Sprite& sprite)
		{
			return sprite.GetPosition().x == position.x && sprite.GetPosition().y == position.y && sprite.GetTexture() == texture;
		});
		return (it != m_sprites.end() ? &(*it) : nullptr);
	}

	void Scene::TryEraseSpriteFromTexture(NonOwningPtr<Texture> texture)
	{
		do
		{
			auto it = std::find_if(m_sprites.begin(), m_sprites.end(), [texture, position](const Sprite& sprite)
			{
				return sprite.GetPosition().x == position.x && sprite.GetPosition().y == position.y && sprite.GetTexture() == texture;
			});
			m_sprites.erase(it);
		} while(it != m_sprites.end());
	}
}
