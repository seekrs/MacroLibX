/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_manager.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 00:56:15 by maldavid          #+#    #+#             */
/*   Updated: 2024/01/11 01:49:12 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_TEXTURE_MANAGER__
#define __MLX_TEXTURE_MANAGER__

#include <unordered_set>
#include <renderer/images/texture_descriptor.h>
#include <core/profiler.h>
#include <utility>

namespace mlx
{
	class TextureManager
	{
		public:
			TextureManager() = default;

			inline void clear() { _texture_descriptors.clear(); }
			inline std::pair<DrawableResource*, bool> registerTexture(Texture* texture, int x, int y)
			{
				MLX_PROFILE_FUNCTION();
				auto res = _texture_descriptors.emplace(texture, x, y);
				return std::make_pair(static_cast<DrawableResource*>(&const_cast<TextureRenderDescriptor&>(*res.first)), res.second);
			}

			~TextureManager() = default;

		private:
			std::unordered_set<TextureRenderDescriptor> _texture_descriptors;
	};
}

#endif
