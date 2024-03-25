/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_semaphore.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 18:59:38 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/25 17:51:47 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_VK_SEMAPHORE__
#define __MLX_VK_SEMAPHORE__

#include <pre_compiled.h>

namespace mlx
{
	class Semaphore
	{
		public:
			void init();
			void destroy() noexcept;

			inline VkSemaphore& getImageSemaphore() noexcept { return _image_available_semaphore; }
			inline VkSemaphore& getRenderImageSemaphore() noexcept { return _render_finished_semaphore; }

		private:
			VkSemaphore _image_available_semaphore = VK_NULL_HANDLE;
			VkSemaphore _render_finished_semaphore = VK_NULL_HANDLE;
	};
}

#endif // __MLX_VK_SEMAPHORE__
