/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_atlas.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 16:36:33 by maldavid          #+#    #+#             */
/*   Updated: 2023/04/08 00:27:46 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_TEXTURE_ATLAS__
#define __MLX_TEXTURE_ATLAS__

#include <renderer/images/texture.h>
#include <array>
#include <glm/glm.hpp>

namespace mlx
{
	class TextureAtlas : public Image
	{
		public:
			TextureAtlas() = default;

			void create(uint8_t* pixels, uint32_t width, uint32_t height, VkFormat format);
			void render(class Renderer& renderer, int x, int y, std::array<glm::vec2, 4> pos, std::array<glm::vec2, 4> uv);
			void destroy() noexcept override;

			inline void setColor(int color) noexcept { _color = color; }

			inline void setDescriptor(DescriptorSet set) noexcept { _set = std::move(set); }
			inline VkDescriptorSet getSet() noexcept { return _set.isInit() ? _set.get() : VK_NULL_HANDLE; }
			inline void updateSet(int binding) noexcept { _set.writeDescriptor(binding, getImageView(), getSampler()); }

			~TextureAtlas() = default;

		private:
			VBO _vbo;
			C_IBO _ibo;
			DescriptorSet _set;
			int _color;
	};
}

#endif
