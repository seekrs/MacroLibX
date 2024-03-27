/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Fence.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 17:52:09 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/27 22:48:31 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_VK_FENCE__
#define __MLX_VK_FENCE__

namespace mlx
{
	class Fence
	{
		public:
			Fence() = default;

			void Init();

			inline VkFence& Get() noexcept { return m_fence; }
			void Wait() noexcept;
			void Reset() noexcept;
			bool IsReady() const noexcept;
			MLX_FORCEINLINE void WaitAndReset() noexcept { Wait(); Reset(); }

			void Destroy() noexcept;

			~Fence() = default;

		private:
			VkFence m_fence = VK_NULL_HANDLE;
	};
}

#endif
