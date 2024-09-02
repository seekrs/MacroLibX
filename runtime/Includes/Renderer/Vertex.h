#ifndef __MLX_VERTEX__
#define __MLX_VERTEX__

#include <Maths/Vec4.h>
#include <Maths/Vec2.h>

namespace mlx
{
	struct Vertex
	{
		alignas(16) Vec2f position = Vec4f{ 0.0f, 0.0f };
		alignas(16) Vec4f color = Vec4f{ 1.0f, 1.0f, 1.0f, 1.0f };
		alignas(16) Vec2f uv = Vec2f{ 0.0f, 0.0f };

		Vertex() = default;
		Vertex(Vec2f p, Vec4f c, Vec2f u) : position(std::move(p)), color(std::move(c)), uv(std::move(u)) {}

		[[nodiscard]] inline static VkVertexInputBindingDescription GetBindingDescription();
		[[nodiscard]] inline static std::array<VkVertexInputAttributeDescription, 3> GetAttributeDescriptions();
	};
}

#include <Renderer/Vertex.inl>

#endif
