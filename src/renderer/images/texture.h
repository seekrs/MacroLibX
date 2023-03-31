/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 02:24:58 by maldavid          #+#    #+#             */
/*   Updated: 2023/03/31 18:06:09 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_TEXTURE__
#define __MLX_TEXTURE__

#include <renderer/images/vk_image.h>

namespace mlx
{
	class Texture : public Image
	{
		public:
			Texture() = default;
			void create(uint8_t* pixels, uint32_t width, uint32_t height, VkFormat format);
			~Texture() = default;
	};
}

#endif
