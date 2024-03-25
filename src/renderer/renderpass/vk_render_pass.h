/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_render_pass.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 18:22:00 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/14 17:05:40 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_VK_RENDER_PASS__
#define __MLX_VK_RENDER_PASS__

#include <mlx_profile.h>
#include <volk.h>

namespace mlx
{
	class RenderPass
	{
		public:
			void init(VkFormat attachement_format, VkImageLayout layout);
			void destroy() noexcept;

			void begin(class CmdBuffer& cmd, class FrameBuffer& fb);
			void end(class CmdBuffer& cmd);
			
			inline VkRenderPass& operator()() noexcept { return _render_pass; }
			inline VkRenderPass& get() noexcept { return _render_pass; }

		private:
			VkRenderPass _render_pass = VK_NULL_HANDLE;
			bool _is_running = false;
	};
}

#endif // __MLX_VK_RENDER_PASS__
