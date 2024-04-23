/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ImagesRegistry.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 15:11:47 by maldavid          #+#    #+#             */
/*   Updated: 2024/04/21 20:31:00 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_CORE_IMAGES_REGISTRY__
#define __MLX_CORE_IMAGES_REGISTRY__

namespace mlx
{
	class ImageRegistry
	{
		public:
			ImageRegistry() = default;

			inline void RegisterTexture(NonOwningPtr<class Texture> texture);
			inline void UnregisterTexture(NonOwningPtr<class Texture> texture);
			inline bool IsTextureKnown(NonOwningPtr<class Texture> texture);

			~ImageRegistry() = default;

		private:
			std::unordered_set<NonOwningPtr<class Texture>> m_textures_registry;
	};
}

#include <Core/ImagesRegistry.inl>

#endif
