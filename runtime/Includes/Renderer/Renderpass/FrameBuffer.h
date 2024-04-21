/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FrameBuffer.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 18:19:44 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/28 22:16:02 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_VK_FRAMEBUFFER__
#define __MLX_VK_FRAMEBUFFER__

namespace mlx
{
	class FrameBuffer
	{
		public:
			void Init(class RenderPass& renderpass, class Image& image);
			void Destroy() noexcept;

			inline VkFramebuffer& operator()() noexcept { return m_framebuffer; }
			inline VkFramebuffer& Get() noexcept { return m_framebuffer; }
			inline std::uint32_t GetWidth() const noexcept { return m_width; }
			inline std::uint32_t GetHeight() const noexcept { return m_height; }

		private:
			VkFramebuffer m_framebuffer = VK_NULL_HANDLE;
			std::uint32_t m_width = 0;
			std::uint32_t m_height = 0;
	};
}

#endif // __MLX_VK_FRAMEBUFFER__
