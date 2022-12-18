/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_semaphore.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 18:59:38 by maldavid          #+#    #+#             */
/*   Updated: 2022/12/18 19:50:10 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_VK_SEMAPHORE__
#define __MLX_VK_SEMAPHORE__

#include <volk.h>
#include <vector>

namespace mlx
{
	class Semaphore
	{
		public:
			void init(class Renderer& renderer);
			void destroy() noexcept;

			inline VkSemaphore& getImageSemaphore(int i) noexcept { return _imageAvailableSemaphores[i]; }
			inline VkSemaphore& getRenderImageSemaphore(int i) noexcept { return _renderFinishedSemaphores[i]; }
			inline VkFence& getInFlightFence(int i) noexcept { return _inFlightFences[i]; }
			inline VkFence& getInFlightImage(int i) noexcept { return _imagesInFlight[i]; }

		private:
			std::vector<VkSemaphore> _imageAvailableSemaphores;
			std::vector<VkSemaphore> _renderFinishedSemaphores;
			std::vector<VkFence> _inFlightFences;
			std::vector<VkFence> _imagesInFlight;
	};
}

#endif // __MLX_VK_SEMAPHORE__
