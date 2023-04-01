/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_library.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 14:16:13 by maldavid          #+#    #+#             */
/*   Updated: 2023/04/01 14:42:39 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_TEXTURE_LIBRARY__
#define __MLX_TEXTURE_LIBRARY__

#include <vector>
#include <memory>
#include <unordered_map>
#include <filesystem>
#include <renderer/images/texture.h>

namespace mlx
{
	using TextureID = uint32_t;
	constexpr TextureID nulltexture = 0;

	class TextureLibrary
	{
		public:
			TextureLibrary() = default;

			std::shared_ptr<Texture> getTexture(TextureID id);
			TextureID addTextureToLibrary(std::shared_ptr<Texture> texture);
			void removeTextureFromLibrary(TextureID id);

			void clearLibrary();

			~TextureLibrary() = default;

		private:
			std::unordered_map<TextureID, std::shared_ptr<Texture>> _cache;
			TextureID _current_id = 1;
	};
}

#endif
