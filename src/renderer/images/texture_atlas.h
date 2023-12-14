/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_atlas.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 16:36:33 by maldavid          #+#    #+#             */
/*   Updated: 2023/12/14 17:12:54 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_TEXTURE_ATLAS__
#define __MLX_TEXTURE_ATLAS__

#include <renderer/images/texture.h>
#include <array>
#include <glm/glm.hpp>
#include <mlx_profile.h>

namespace mlx
{
	class TextureAtlas : public Image
	{
		public:
			TextureAtlas() = default;

			void create(uint8_t* pixels, uint32_t width, uint32_t height, VkFormat format, const char* name, bool dedicated_memory = false);
			void render(class Renderer& renderer, int x, int y, uint32_t ibo_size) const;
			void destroy() noexcept override;

			inline void setDescriptor(DescriptorSet&& set) noexcept { _set = set; }
			inline VkDescriptorSet getSet() noexcept { return _set.isInit() ? _set.get() : VK_NULL_HANDLE; }
			inline void updateSet(int binding) const noexcept { _set.writeDescriptor(binding, getImageView(), getSampler()); }

			~TextureAtlas() = default;

		private:
			DescriptorSet _set;
	};
}

#endif
