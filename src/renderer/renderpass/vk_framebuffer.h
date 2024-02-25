/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_framebuffer.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 18:19:44 by maldavid          #+#    #+#             */
/*   Updated: 2024/01/03 15:28:19 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_VK_FRAMEBUFFER__
#define __MLX_VK_FRAMEBUFFER__

#include <mlx_profile.h>
#include <volk.h>

namespace mlx
{
	class FrameBuffer
	{
		public:
			void init(class RenderPass& renderpass, class Image& image);
			void destroy() noexcept;

			inline VkFramebuffer& operator()() noexcept { return _framebuffer; }
			inline VkFramebuffer& get() noexcept { return _framebuffer; }
			inline std::uint32_t getWidth() const noexcept { return _width; }
			inline std::uint32_t getHeight() const noexcept { return _height; }

		private:
			VkFramebuffer _framebuffer = VK_NULL_HANDLE;
			std::uint32_t _width = 0;
			std::uint32_t _height = 0;
	};
}

#endif // __MLX_VK_FRAMEBUFFER__
