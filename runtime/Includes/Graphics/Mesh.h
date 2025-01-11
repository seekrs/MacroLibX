#ifndef __MLX_RENDERER_MESH__
#define __MLX_RENDERER_MESH__

#include <Renderer/Vertex.h>
#include <Renderer/Buffer.h>
#include <Utils/Buffer.h>

namespace mlx
{
	class Mesh
	{
		public:
			struct SubMesh
			{
				struct NoBuild {};

				VertexBuffer vbo;
				IndexBuffer ibo;
				std::vector<Vertex> vertex_data;
				std::vector<std::uint32_t> index_data;
				std::size_t triangle_count = 0;

				inline SubMesh(const std::vector<Vertex>& vertices, const std::vector<std::uint32_t>& indices);
				inline SubMesh(const std::vector<Vertex>& vertices, const std::vector<std::uint32_t>& indices, NoBuild);
			};

		public:
			Mesh() = default;

			void Draw(VkCommandBuffer cmd, std::size_t& drawcalls, std::size_t& polygondrawn) const noexcept;
			void Draw(VkCommandBuffer cmd, std::size_t& drawcalls, std::size_t& polygondrawn, std::size_t submesh_index) const noexcept;

			inline std::size_t GetSubMeshCount() const { return m_sub_meshes.size(); }

			inline void AddSubMesh(SubMesh mesh) { m_sub_meshes.emplace_back(std::move(mesh)); }
			[[nodiscard]] inline SubMesh& GetSubMesh(std::size_t index) { return m_sub_meshes.at(index); }

			~Mesh();

		private:
			std::vector<SubMesh> m_sub_meshes;
	};

	// A registry just to avoid destroying meshes when clearing a window
	class MeshRegistry
	{
		public:
			inline MeshRegistry();

			inline void RegisterMesh(std::shared_ptr<Mesh> mesh);
			inline std::shared_ptr<Mesh> FindMesh(const std::vector<Mesh::SubMesh>& sub_meshes);
			inline void UnregisterMesh(std::shared_ptr<Mesh> mesh);
			inline void Reset();

			inline static bool IsInit() noexcept { return s_instance != nullptr; }
			inline static MeshRegistry& Get() noexcept { return *s_instance; }

			inline ~MeshRegistry();

		private:
			inline static MeshRegistry* s_instance = nullptr;
			std::unordered_set<std::shared_ptr<Mesh>> m_meshes_registry;
	};
}

#include <Graphics/Mesh.inl>

#endif
