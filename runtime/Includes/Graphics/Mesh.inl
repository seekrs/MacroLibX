#pragma once
#include <Graphics/Mesh.h>

namespace mlx
{
	Mesh::SubMesh::SubMesh(const std::vector<Vertex>& vertices, const std::vector<std::uint32_t>& indices) : vertex_data(vertices), index_data(indices)
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

	Mesh::SubMesh::SubMesh(const std::vector<Vertex>& vertices, const std::vector<std::uint32_t>& indices, NoBuild) : vertex_data(vertices), index_data(indices) {}

	MeshRegistry::MeshRegistry()
	{
		s_instance = this;
	}

	void MeshRegistry::RegisterMesh(std::shared_ptr<Mesh> mesh)
	{
		m_meshes_registry.insert(mesh);
	}

	std::shared_ptr<Mesh> MeshRegistry::FindMesh(const std::vector<Mesh::SubMesh>& sub_meshes)
	{
		for(const std::shared_ptr<Mesh>& mesh : m_meshes_registry)
		{
			if(mesh->GetSubMeshCount() != sub_meshes.size()) // If the number of submeshes is different than the one we want to find no need to test
				continue;
			bool found = true;
			for(std::size_t i = 0; i < sub_meshes.size(); i++)
			{
				try
				{
					const Mesh::SubMesh& registered_sub_mesh = mesh->GetSubMesh(i);
					if(registered_sub_mesh.vertex_data != sub_meshes[i].vertex_data || registered_sub_mesh.index_data != sub_meshes[i].index_data)
					{
						found = false;
						break;
					}
				}
				catch(...)
				{
					found = false;
					break;
				}
			}
			if(found)
				return mesh;
		}
		return nullptr;
	}

	void MeshRegistry::UnregisterMesh(std::shared_ptr<Mesh> mesh)
	{
		m_meshes_registry.erase(mesh);
	}

	void MeshRegistry::Reset()
	{
		m_meshes_registry.clear();
	}

	MeshRegistry::~MeshRegistry()
	{
		s_instance = nullptr;
	}
}
