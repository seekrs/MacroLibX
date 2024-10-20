#include <PreCompiled.h>
#include <Graphics/Scene.h>
#include <Renderer/Renderer.h>
#include <Renderer/RenderCore.h>

namespace mlx
{
	Sprite& Scene::CreateSprite(NonOwningPtr<Texture> texture) noexcept
	{
		MLX_PROFILE_FUNCTION();
		std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>(texture);
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

	void Scene::BringToFront(NonOwningPtr<Sprite> sprite)
	{
		MLX_PROFILE_FUNCTION();
		auto it = std::find_if(m_sprites.begin(), m_sprites.end(), [&sprite](std::shared_ptr<Sprite> sprite_ptr)
		{
			return sprite_ptr.get() == sprite.Get();
		});
		if(it == m_sprites.end())
			return;
		std::rotate(it, it + 1, m_sprites.end());
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
			if(it != m_sprites.end())
				m_sprites.erase(it);
		} while(it != m_sprites.end());
	}
	
	bool Scene::IsTextureAtGivenDrawLayer(NonOwningPtr<Texture> texture, std::uint64_t draw_layer) const
	{
		if(draw_layer >= m_sprites.size())
			return false;
		return m_sprites[draw_layer]->GetTexture() == texture;
	}
}
