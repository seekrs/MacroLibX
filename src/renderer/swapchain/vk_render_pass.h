/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_render_pass.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 18:22:00 by maldavid          #+#    #+#             */
/*   Updated: 2022/12/18 20:33:28 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_VK_RENDER_PASS__
#define __MLX_VK_RENDER_PASS__

#include <volk.h>

namespace mlx
{
	class RenderPass
	{
		public:
			void init(class Renderer* renderer);
			void destroy() noexcept;

			void begin();
			void end();
			
            inline VkRenderPass& operator()() noexcept { return _renderPass; }
            inline VkRenderPass& get() noexcept { return _renderPass; }

		private:
			VkRenderPass _renderPass = VK_NULL_HANDLE;
			class Renderer* _renderer = nullptr;
			bool _is_running = false;
	};
}

#endif // __MLX_VK_RENDER_PASS__
