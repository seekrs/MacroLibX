#include <PreCompiled.h>
#include <Graphics/Scene.h>
#include <Renderer/Renderer.h>
#include <Renderer/RenderCore.h>

namespace mlx
{
	Scene::Scene(SceneDescriptor desc)
	: m_descriptor(std::move(desc))
	{
		MLX_PROFILE_FUNCTION();
		Verify((bool)m_descriptor.renderer, "invalid renderer");
		m_depth.Init(m_descriptor.renderer->GetSwapchainImages().back().GetWidth(), m_descriptor.renderer->GetSwapchainImages().back().GetHeight(), false, "mlx_scene_depth");
	}

	Sprite& Scene::CreateSprite(NonOwningPtr<Texture> texture) noexcept
	{
		MLX_PROFILE_FUNCTION();
		std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>(*m_descriptor.renderer, texture);
		m_sprites.push_back(sprite);
		return *sprite;
	}

	NonOwningPtr<Sprite> Scene::GetSpriteFromTextureAndPosition(NonOwningPtr<Texture> texture, const Vec2f& position) const
	{
		MLX_PROFILE_FUNCTION();
		auto it = std::find_if(m_sprites.begin(), m_sprites.end(), [&texture, &position](std::shared_ptr<Sprite> sprite)
		{
			return sprite->GetTexture() == texture && sprite->GetPosition().x == position.x && sprite->GetPosition().y == position.y;
		});
		return (it != m_sprites.end() ? it->get() : nullptr);
	}

	void Scene::TryEraseSpriteFromTexture(NonOwningPtr<Texture> texture)
	{
		MLX_PROFILE_FUNCTION();
		auto it = m_sprites.begin();
		do
		{
			it = std::find_if(m_sprites.begin(), m_sprites.end(), [&texture](std::shared_ptr<Sprite> sprite)
			{
				return sprite->GetTexture() == texture;
			});
			m_sprites.erase(it);
		} while(it != m_sprites.end());
	}
}
