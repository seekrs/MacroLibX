#ifndef __MLX_VERTEX__
#define __MLX_VERTEX__

#include <Maths/Vec4.h>
#include <Maths/Vec2.h>

namespace mlx
{
	struct Vertex
	{
		alignas(16) Vec4f position = Vec4f{ 0.0f, 0.0f, 0.0f, 1.0f };
		alignas(16) Vec2f uv = Vec2f{ 0.0f, 0.0f };

		Vertex() = default;
		inline Vertex(Vec4f p, Vec2f u) : position(std::move(p)), uv(std::move(u)) {}

		[[nodiscard]] inline bool operator==(const Vertex& rhs) const noexcept;

		[[nodiscard]] inline static VkVertexInputBindingDescription GetBindingDescription();
		[[nodiscard]] inline static std::array<VkVertexInputAttributeDescription, 2> GetAttributeDescriptions();
	};
}

#include <Renderer/Vertex.inl>

#endif
