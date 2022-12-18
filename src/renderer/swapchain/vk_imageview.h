/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_imageview.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 18:20:19 by maldavid          #+#    #+#             */
/*   Updated: 2022/12/18 19:54:44 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_VK_IMAGE_VIEW__
#define __MLX_VK_IMAGE_VIEW__

#include <volk.h>

namespace mlx
{
	class ImageView
	{
		public:
			void init(class SwapChain& swapchain, VkImage& image);
			void destroy() noexcept;

            inline VkImageView& operator()() noexcept { return _image; }
            inline VkImageView& get() noexcept { return _image; }

		private:
			VkImageView _image = VK_NULL_HANDLE;
	};
}

#endif // __MLX_VK_IMAGE_VIEW__
