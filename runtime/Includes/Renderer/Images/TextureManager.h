/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TextureManager.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 00:56:15 by maldavid          #+#    #+#             */
/*   Updated: 2024/04/03 16:24:51 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_TEXTURE_MANAGER__
#define __MLX_TEXTURE_MANAGER__

#include <Renderer/Images/TextureDescriptor.h>

namespace mlx
{
	class TextureManager
	{
		public:
			TextureManager() = default;

			inline void Clear() { m_texture_descriptors.clear(); }

			inline std::pair<NonOwningPtr<DrawableResource>, bool> RegisterTexture(NonOwningPtr<Texture> texture, int x, int y)
			{
				MLX_PROFILE_FUNCTION();
				auto res = m_texture_descriptors.emplace(texture, x, y);
				return std::make_pair(static_cast<DrawableResource*>(&const_cast<TextureRenderDescriptor&>(*res.first)), res.second);
			}

			inline bool IsTextureKnown(NonOwningPtr<Texture> texture) noexcept
			{
				MLX_PROFILE_FUNCTION();
				for(const auto& desc : m_texture_descriptors)
				{
					if(desc.texture == texture)
						return true;
				}
				return false;
			}

			inline void EraseTextures(NonOwningPtr<Texture> texture)
			{
				MLX_PROFILE_FUNCTION();
				for(auto it = m_texture_descriptors.begin(); it != m_texture_descriptors.end();)
				{
					if(it->texture == texture)
						it = m_texture_descriptors.erase(it);
					else
						++it;
				}
			}

			~TextureManager() = default;

		private:
			std::unordered_set<TextureRenderDescriptor> m_texture_descriptors;
	};
}

#endif
