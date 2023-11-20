/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_render_pass.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 18:22:00 by maldavid          #+#    #+#             */
/*   Updated: 2023/11/20 07:24:48 by maldavid         ###   ########.fr       */
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
			void init(VkFormat attachement_format);
			void destroy() noexcept;

			void begin(class CmdBuffer& cmd, class FrameBuffer& fb);
			void end(class CmdBuffer& cmd);
			
			inline VkRenderPass& operator()() noexcept { return _renderPass; }
			inline VkRenderPass& get() noexcept { return _renderPass; }

		private:
			VkRenderPass _renderPass = VK_NULL_HANDLE;
			bool _is_running = false;
	};
}

#endif // __MLX_VK_RENDER_PASS__
