/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_framebuffer.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 18:19:44 by maldavid          #+#    #+#             */
/*   Updated: 2022/12/18 19:53:00 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_VK_FRAMEBUFFER__
#define __MLX_VK_FRAMEBUFFER__

#include <volk.h>

namespace mlx
{
    class FrameBuffer
    {
        public:
            void init(class Renderer& renderer, class ImageView& image);
            void destroy() noexcept;

            inline VkFramebuffer& operator()() noexcept { return _framebuffer; }
            inline VkFramebuffer& get() noexcept { return _framebuffer; }

        private:
            VkFramebuffer _framebuffer = VK_NULL_HANDLE;
    };
}

#endif // __MLX_VK_FRAMEBUFFER__
