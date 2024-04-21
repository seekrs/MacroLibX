/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RenderPass.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 18:22:00 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/28 22:16:44 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_VK_RENDER_PASS__
#define __MLX_VK_RENDER_PASS__

namespace mlx
{
	class RenderPass
	{
		public:
			void Init(VkFormat attachement_format, VkImageLayout layout);
			void Destroy() noexcept;

			void Begin(class CmommandBuffer& cmd, class FrameBuffer& fb);
			void End(class CommandBuffer& cmd);
			
			inline VkRenderPass& operator()() noexcept { return m_render_pass; }
			inline VkRenderPass& Get() noexcept { return m_render_pass; }

		private:
			VkRenderPass m_render_pass = VK_NULL_HANDLE;
			bool m_is_running = false;
	};
}

#endif // __MLX_VK_RENDER_PASS__
