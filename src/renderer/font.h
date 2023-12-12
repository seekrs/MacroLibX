/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   font.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 21:17:04 by kbz_8             #+#    #+#             */
/*   Updated: 2023/12/11 22:31:22 by kbz_8            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_FONT__
#define __MLX_FONT__

#include <array>
#include <stb_truetype.h>
#include <utils/non_copyable.h>
#include <renderer/images/texture_atlas.h>

namespace mlx
{
	class Font : public non_copyable
	{
		public:
			Font(class Renderer& renderer, const std::filesystem::path& path, float scale);
			inline const std::string& getName() const { return _name; }
			~Font();

		private:
			std::array<stbtt_packedchar, 96> _cdata;
			TextureAtlas _atlas;
			std::string _name;
	};
}

#endif
