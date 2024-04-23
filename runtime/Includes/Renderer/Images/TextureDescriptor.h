/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TextureDescriptor.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 01:00:13 by maldavid          #+#    #+#             */
/*   Updated: 2024/04/23 22:08:02 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_TEXTURE_DESCRIPTOR__
#define __MLX_TEXTURE_DESCRIPTOR__

#include <Renderer/Images/Texture.h>
#include <Renderer/Core/DrawableResource.h>
#include <Utils/CombineHash.h>

namespace mlx
{
	struct TextureRenderDescriptor : public DrawableResource
	{
		NonOwningPtr<Texture> texture;
		int x;
		int y;

		TextureRenderDescriptor(NonOwningPtr<Texture> _texture, int _x, int _y) : texture(_texture), x(_x), y(_y) {}
		inline bool operator==(const TextureRenderDescriptor& rhs) const { return texture == rhs.texture && x == rhs.x && y == rhs.y; }
		inline void Render(class Renderer& renderer) override
		{
			if(!texture->IsInit())
				return;
			texture->Render(renderer, x, y);
		}
		inline void ResetUpdate() override 
		{
			if(!texture->IsInit())
				return;
			texture->ResetUpdate();
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
			mlx::HashCombine(hash, d.texture, d.x, d.y);
			return hash;
		}
	};
}

#endif
