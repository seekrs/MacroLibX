#pragma once
#include <Graphics/Mesh.h>

namespace mlx
{
	Mesh::SubMesh::SubMesh(const std::vector<Vertex>& vertices, const std::vector<std::uint32_t>& indices)
	{
		CPUBuffer vb(vertices.size() * sizeof(Vertex));
		std::memcpy(vb.GetData(), vertices.data(), vb.GetSize());
		vbo.Init(vb.GetSize(), 0, "mlx_mesh");
		vbo.SetData(std::move(vb));

		CPUBuffer ib(indices.size() * sizeof(std::uint32_t));
		std::memcpy(ib.GetData(), indices.data(), ib.GetSize());
		ibo.Init(ib.GetSize(), 0, "mlx_mesh");
		ibo.SetData(std::move(ib));

		triangle_count = vertices.size() / 3;
	}
}
