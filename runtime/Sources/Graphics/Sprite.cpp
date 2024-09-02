#include <PreCompiled.h>
#include <Graphics/Sprite.h>
#include <Renderer/Image.h>
#include <Renderer/Vertex.h>

namespace mlx
{
	std::shared_ptr<Mesh> CreateQuad(float x, float y, float width, float height)
	{
		std::vector<Vertex> data(4);

		data[0].position = Vec4f(x, y, 0.0f, 1.0f);
		data[0].uv = Vec2f(1.0f, 1.0f);

		data[1].position = Vec4f(x + width, y, 0.0f, 1.0f);
		data[1].uv = Vec2f(0.0f, 1.0f);

		data[2].position = Vec4f(x + width, y + height, 0.0f, 1.0f);
		data[2].uv = Vec2f(0.0f, 0.0f);

		data[3].position = Vec4f(x, y + height, 0.0f, 1.0f);
		data[3].uv = Vec2f(1.0f, 0.0f);

		std::vector<std::uint32_t> indices = {
			0,
			1,
			2,
			2,
			3,
			0,
		};

		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
		mesh->AddSubMesh({ std::move(data), std::move(indices) });
		return mesh;
	}

	Sprite::Sprite(std::shared_ptr<Texture> texture)
	{
		Verify((bool)texture, "Sprite: invalid texture");
		p_mesh = CreateQuad(0, 0, texture->GetWidth(), texture->GetHeight());
		p_texture = texture;

		func::function<void(const EventBase&)> functor = [this](const EventBase& event)
		{
			if(event.What() == Event::DescriptorPoolResetEventCode)
				m_set.Reallocate();
		};
		EventBus::RegisterListener({ functor, "__Sprite" + std::to_string(reinterpret_cast<std::uintptr_t>(this)) });
	}
}
