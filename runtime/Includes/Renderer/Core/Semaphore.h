/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Semaphore.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 18:59:38 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/27 22:56:51 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_VK_SEMAPHORE__
#define __MLX_VK_SEMAPHORE__

namespace mlx
{
	class Semaphore
	{
		public:
			void Init();
			void Destroy() noexcept;

			inline VkSemaphore& Get() noexcept { return m_semaphore; }

		private:
			VkSemaphore m_semaphore = VK_NULL_HANDLE;
	};
}

#endif // __MLX_VK_SEMAPHORE__
