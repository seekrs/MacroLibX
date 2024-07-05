/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TextureRegistry.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 00:56:15 by maldavid          #+#    #+#             */
/*   Updated: 2024/07/05 13:43:48 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_TEXTURE_MANAGER__
#define __MLX_TEXTURE_MANAGER__

#include <Renderer/Images/TextureDescriptor.h>

namespace mlx
{
	class TextureRegistry
	{
		public:
			TextureRegistry() = default;

			inline void Clear();
			inline std::pair<NonOwningPtr<DrawableResource>, bool> RegisterTexture(NonOwningPtr<Texture> texture, int x, int y);
			inline bool IsTextureKnown(NonOwningPtr<Texture> texture) noexcept;
			inline void EraseTextures(NonOwningPtr<Texture> texture);

			~TextureRegistry() = default;

		private:
			std::unordered_set<TextureRenderDescriptor> m_texture_descriptors;
	};
}

#include <Renderer/Images/TextureRegistry.inl>

#endif
