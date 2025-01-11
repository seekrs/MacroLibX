#include <PreCompiled.h>
#include <Graphics/Scene.h>
#include <Renderer/Renderer.h>
#include <Renderer/RenderCore.h>

namespace mlx
{
	Sprite& Scene::CreateSprite(NonOwningPtr<Texture> texture) noexcept
	{
		MLX_PROFILE_FUNCTION();
		Verify((bool)texture, "Scene: invalid texture (internal mlx issue, please report to devs)");

		for(auto& drawable : m_drawables)
		{
			if(!drawable || drawable->GetType() != DrawableType::Sprite)
				continue;
			if(texture->GetWidth() == static_cast<Sprite*>(drawable.get())->GetTexture()->GetWidth() && texture->GetHeight() == static_cast<Sprite*>(drawable.get())->GetTexture()->GetHeight())
			{
				std::shared_ptr<Sprite> new_sprite = std::make_shared<Sprite>(drawable->GetMesh(), texture);
				m_drawables.push_back(new_sprite);
				m_has_scene_changed = true;
				return *new_sprite;
			}
		}

		std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>(texture);
		m_drawables.push_back(sprite);
		m_has_scene_changed = true;
		return *sprite;
	}

	NonOwningPtr<Sprite> Scene::GetSpriteFromTexturePositionScaleRotation(NonOwningPtr<Texture> texture, const Vec2f& position, float scale_x, float scale_y, float rotation) const
	{
		MLX_PROFILE_FUNCTION();
		auto it = std::find_if(m_drawables.begin(), m_drawables.end(), [&texture, &position, scale_x, scale_y, rotation](std::shared_ptr<Drawable> drawable)
		{
			if(!drawable || drawable->GetType() != DrawableType::Sprite)
				return false;
			return  static_cast<Sprite*>(drawable.get())->GetTexture().Get() == texture.Get() &&
					drawable->GetPosition() == position &&
					drawable->GetScale() == Vec2f{ scale_x, scale_y } &&
					drawable->GetRotation().ToEulerAngles() == EulerAnglesf{ 0.0f, 0.0f, rotation };
		});
		return static_cast<Sprite*>(it != m_drawables.end() ? it->get() : nullptr);
	}

	void Scene::TryEraseSpriteFromTexture(NonOwningPtr<Texture> texture)
	{
		MLX_PROFILE_FUNCTION();
		auto it = m_drawables.begin();
		do
		{
			it = std::find_if(m_drawables.begin(), m_drawables.end(), [&texture](std::shared_ptr<Drawable> drawable)
			{
				if(!drawable || drawable->GetType() != DrawableType::Sprite)
					return false;
				return static_cast<Sprite*>(drawable.get())->GetTexture() == texture;
			});
			if(it != m_drawables.end())
				m_drawables.erase(it);
		} while(it != m_drawables.end());
	}

	bool Scene::IsTextureAtGivenDrawLayer(NonOwningPtr<Texture> texture, std::uint64_t draw_layer) const
	{
		MLX_PROFILE_FUNCTION();
		if(draw_layer >= m_drawables.size())
			return false;
		if(!m_drawables[draw_layer] || m_drawables[draw_layer]->GetType() != DrawableType::Sprite)
			return false;
		return static_cast<Sprite*>(m_drawables[draw_layer].get())->GetTexture() == texture;
	}

	Text& Scene::CreateText(const std::string& text) noexcept
	{
		MLX_PROFILE_FUNCTION();

		Assert((bool)p_bound_font, "no font bound");

		for(auto& drawable : m_drawables)
		{
			if(!drawable || drawable->GetType() != DrawableType::Text)
				continue;
			if(text == static_cast<Text*>(drawable.get())->GetText() && p_bound_font == static_cast<Text*>(drawable.get())->GetFont())
			{
				std::shared_ptr<Text> new_text = std::make_shared<Text>(text, p_bound_font, drawable->GetMesh());
				m_drawables.push_back(new_text);
				m_has_scene_changed = true;
				return *new_text;
			}
		}

		std::shared_ptr<Text> new_text = std::make_shared<Text>(text, p_bound_font);
		m_drawables.push_back(new_text);
		m_has_scene_changed = true;
		return *new_text;
	}

	NonOwningPtr<Text> Scene::GetTextFromPositionAndColor(const std::string& text, const Vec2f& position, const Vec4f& color) const
	{
		MLX_PROFILE_FUNCTION();
		auto it = std::find_if(m_drawables.begin(), m_drawables.end(), [&text, &position, &color](std::shared_ptr<Drawable> drawable)
		{
			if(!drawable || drawable->GetType() != DrawableType::Text)
				return false;
			return static_cast<Text*>(drawable.get())->GetText() == text && drawable->GetPosition() == position && drawable->GetColor() == color;
		});
		return static_cast<Text*>(it != m_drawables.end() ? it->get() : nullptr);
	}

	bool Scene::IsTextAtGivenDrawLayer(const std::string& text, std::uint64_t draw_layer) const
	{
		MLX_PROFILE_FUNCTION();
		if(draw_layer >= m_drawables.size())
			return false;
		if(!m_drawables[draw_layer] || m_drawables[draw_layer]->GetType() != DrawableType::Text)
			return false;
		Text* ptr = static_cast<Text*>(m_drawables[draw_layer].get());
		return ptr->GetText() == text && ptr->GetFont() == p_bound_font;
	}

	void Scene::BringToDrawLayer(NonOwningPtr<Drawable> drawable, std::uint64_t draw_layer)
	{
		MLX_PROFILE_FUNCTION();
		if(draw_layer < m_drawables.size())
			return;
		auto it = std::find_if(m_drawables.begin(), m_drawables.end(), [&drawable](std::shared_ptr<Drawable> drawable_ptr)
		{
			return drawable_ptr.get() == drawable.Get();
		});
		if(it == m_drawables.end())
			return;
		std::swap(*it, *(m_drawables.begin() + draw_layer));
		m_has_scene_changed = true;
	}
}
