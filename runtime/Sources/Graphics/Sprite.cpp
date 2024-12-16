#include <PreCompiled.h>
#include <Graphics/Sprite.h>
#include <Renderer/Image.h>
#include <Renderer/Vertex.h>
#include <Renderer/Renderer.h>

namespace mlx
{
	std::shared_ptr<Mesh> CreateQuad(float x, float y, float width, float height)
	{
		MLX_PROFILE_FUNCTION();
		std::vector<Vertex> data(4);

		data[0].position = Vec4f(x, y, 0.0f, 1.0f);
		data[0].uv = Vec2f(1.0f, 1.0f);

		data[1].position = Vec4f(x + width, y, 0.0f, 1.0f);
		data[1].uv = Vec2f(0.0f, 1.0f);

		data[2].position = Vec4f(x + width, y + height, 0.0f, 1.0f);
		data[2].uv = Vec2f(0.0f, 0.0f);

		data[3].position = Vec4f(x, y + height, 0.0f, 1.0f);
		data[3].uv = Vec2f(1.0f, 0.0f);

		std::vector<std::uint32_t> indices{
			0,
			1,
			2,
			2,
			3,
			0,
		};

		std::shared_ptr<Mesh> mesh = MeshRegistry::Get().FindMesh({ Mesh::SubMesh{ data, indices, Mesh::SubMesh::NoBuild{} } });
		if(mesh)
			return mesh;
		mesh = std::make_shared<Mesh>();
		mesh->AddSubMesh({ std::move(data), std::move(indices) });
		return mesh;
	}

	Sprite::Sprite(NonOwningPtr<Texture> texture) : Drawable(DrawableType::Sprite)
	{
		MLX_PROFILE_FUNCTION();
		Verify((bool)texture, "Sprite: invalid texture (internal mlx issue, please report to devs)");
		p_mesh = CreateQuad(0, 0, texture->GetWidth(), texture->GetHeight());
		MeshRegistry::Get().RegisterMesh(p_mesh);
		p_texture = texture;
	}

	Sprite::Sprite(std::shared_ptr<Mesh> mesh, NonOwningPtr<Texture> texture) : Drawable(DrawableType::Sprite)
	{
		MLX_PROFILE_FUNCTION();
		Verify((bool)texture, "Sprite: invalid texture (internal mlx issue, please report to devs)");
		Verify((bool)mesh, "Sprite: invalid mesh (internal mlx issue, please report to devs)");
		p_mesh = mesh;
		p_texture = texture;
	}
}
