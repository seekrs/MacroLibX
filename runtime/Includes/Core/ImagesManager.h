/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ImagesManager.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 15:11:47 by maldavid          #+#    #+#             */
/*   Updated: 2024/04/21 15:13:43 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_CORE_IMAGES_MANAGER__
#define __MLX_CORE_IMAGES_MANAGER__

namespace mlx
{
	class ImagesManager
	{
		public:

		private:
			std::unordered_set<Texture*> m_textures_registry;
	};
}

#endif
