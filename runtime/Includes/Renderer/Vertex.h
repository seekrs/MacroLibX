/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Vertex.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 22:24:33 by maldavid          #+#    #+#             */
/*   Updated: 2024/04/23 22:25:01 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_RENDERER_VERTEX__
#define __MLX_RENDERER_VERTEX__

namespace mlx
{
	struct Vertex
	{
		glm::vec2 pos;
		glm::vec4 color;
		glm::vec2 uv;

		Vertex(glm::vec2 _pos, glm::vec4 _color, glm::vec2 _uv) : pos(std::move(_pos)), color(std::move(_color)), uv(std::move(_uv)) {}

		static VkVertexInputBindingDescription GetBindingDescription()
		{
			VkVertexInputBindingDescription binding_description{};
			binding_description.binding = 0;
			binding_description.stride = sizeof(Vertex);
			binding_description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

			return binding_description;
		}

		static std::array<VkVertexInputAttributeDescription, 3> GetAttributeDescriptions()
		{
			std::array<VkVertexInputAttributeDescription, 3> attribute_descriptions;

			attribute_descriptions[0].binding = 0;
			attribute_descriptions[0].location = 0;
			attribute_descriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
			attribute_descriptions[0].offset = offsetof(Vertex, pos);

			attribute_descriptions[1].binding = 0;
			attribute_descriptions[1].location = 1;
			attribute_descriptions[1].format = VK_FORMAT_R32G32B32A32_SFLOAT;
			attribute_descriptions[1].offset = offsetof(Vertex, color);

			attribute_descriptions[2].binding = 0;
			attribute_descriptions[2].location = 2;
			attribute_descriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
			attribute_descriptions[2].offset = offsetof(Vertex, uv);

			return attribute_descriptions;
		}
	};
}

#endif
