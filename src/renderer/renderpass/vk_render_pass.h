/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_render_pass.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 18:22:00 by maldavid          #+#    #+#             */
/*   Updated: 2023/12/08 19:11:14 by kbz_8            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_VK_RENDER_PASS__
#define __MLX_VK_RENDER_PASS__

#include <volk.h>
#include <mlx_profile.h>

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
