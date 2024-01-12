/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawable_resource.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 21:00:37 by maldavid          #+#    #+#             */
/*   Updated: 2024/01/11 01:21:15 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_DRAWABLE_RESOURCE__
#define __MLX_DRAWABLE_RESOURCE__

#include <mlx_profile.h>
#include <volk.h>
#include <array>

namespace mlx
{
	class DrawableResource
	{
		public:
			DrawableResource() = default;
			virtual void render(std::array<VkDescriptorSet, 2>& sets, class Renderer& renderer) = 0;
			virtual void resetUpdate() {}
			virtual ~DrawableResource() = default;
	};
}

#endif
