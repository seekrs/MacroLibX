/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_descriptor.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 01:00:13 by maldavid          #+#    #+#             */
/*   Updated: 2024/01/11 01:21:52 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_TEXTURE_DESCRIPTOR__
#define __MLX_TEXTURE_DESCRIPTOR__

#include <renderer/images/texture.h>
#include <renderer/core/drawable_resource.h>
#include <utils/combine_hash.h>

namespace mlx
{
	struct TextureRenderDescriptor : public DrawableResource
	{
		Texture* texture;
		int x;
		int y;

		TextureRenderDescriptor(Texture* _texture, int _x, int _y) : texture(_texture), x(_x), y(_y) {}
		inline bool operator==(const TextureRenderDescriptor& rhs) const { return texture == rhs.texture && x == rhs.x && y == rhs.y; }
		inline void render(std::array<VkDescriptorSet, 2>& sets, class Renderer& renderer) override
		{
			if(!texture->isInit())
				return;
			texture->render(sets, renderer, x, y);
		}
		inline void resetUpdate() override 
		{
			if(!texture->isInit())
				return;
			texture->resetUpdate();
		}
	};
}

namespace std
{
	template <>
	struct hash<mlx::TextureRenderDescriptor>
	{
		std::size_t operator()(const mlx::TextureRenderDescriptor& d) const noexcept
		{
			std::size_t hash = 0;
			mlx::hashCombine(hash, d.texture, d.x, d.y);
			return hash;
		}
	};
}

#endif
