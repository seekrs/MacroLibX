/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DrawableResource.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 21:00:37 by maldavid          #+#    #+#             */
/*   Updated: 2024/04/23 18:10:56 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_DRAWABLE_RESOURCE__
#define __MLX_DRAWABLE_RESOURCE__

namespace mlx
{
	class DrawableResource
	{
		public:
			DrawableResource() = default;
			virtual void Render(class Renderer& renderer) = 0;
			virtual void ResetUpdate() {}
			virtual ~DrawableResource() = default;
	};
}

#endif
