#include <PreCompiled.h>
#include <Graphics/Mesh.h>
#include <Utils/Buffer.h>

namespace mlx
{
	void Mesh::Draw(VkCommandBuffer cmd, std::size_t& drawcalls, std::size_t& polygondrawn) const noexcept
	{
		MLX_PROFILE_FUNCTION();
		for(std::size_t i = 0; i < m_sub_meshes.size(); i++)
			Draw(cmd, drawcalls, polygondrawn, i);
	}

	void Mesh::Draw(VkCommandBuffer cmd, std::size_t& drawcalls, std::size_t& polygondrawn, std::size_t submesh_index) const noexcept
	{
		MLX_PROFILE_FUNCTION();
		Verify(submesh_index < m_sub_meshes.size(), "invalid submesh index");
		m_sub_meshes[submesh_index].vbo.Bind(cmd);
		m_sub_meshes[submesh_index].ibo.Bind(cmd);
		mlx::RenderCore::Get().vkCmdDrawIndexed(cmd, static_cast<std::uint32_t>(m_sub_meshes[submesh_index].ibo.GetSize() / sizeof(std::uint32_t)), 1, 0, 0, 0);
		polygondrawn += m_sub_meshes[submesh_index].triangle_count;
		drawcalls++;
	}

	Mesh::~Mesh()
	{
		MLX_PROFILE_FUNCTION();
		for(auto& mesh : m_sub_meshes)
		{
			mesh.vbo.Destroy();
			mesh.ibo.Destroy();
		}
	}
}
