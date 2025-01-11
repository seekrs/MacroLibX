#pragma once
#include <Renderer/Vertex.h>

namespace mlx
{
	bool Vertex::operator==(const Vertex& rhs) const noexcept
	{
		return position == rhs.position && uv == rhs.uv;
	}

	VkVertexInputBindingDescription Vertex::GetBindingDescription()
	{
		VkVertexInputBindingDescription binding_description{};
		binding_description.binding = 0;
		binding_description.stride = sizeof(Vertex);
		binding_description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		return binding_description;
	}

	std::array<VkVertexInputAttributeDescription, 2> Vertex::GetAttributeDescriptions()
	{
		std::array<VkVertexInputAttributeDescription, 2> attribute_descriptions;

		attribute_descriptions[0].binding = 0;
		attribute_descriptions[0].location = 0;
		attribute_descriptions[0].format = VK_FORMAT_R32G32B32A32_SFLOAT;
		attribute_descriptions[0].offset = offsetof(Vertex, position);

		attribute_descriptions[1].binding = 0;
		attribute_descriptions[1].location = 1;
		attribute_descriptions[1].format = VK_FORMAT_R32G32_SFLOAT;
		attribute_descriptions[1].offset = offsetof(Vertex, uv);

		return attribute_descriptions;
	}
}
