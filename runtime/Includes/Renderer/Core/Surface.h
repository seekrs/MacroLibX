/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Surface.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 18:57:55 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/27 22:58:15 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_VK_SURFACE__
#define __MLX_VK_SURFACE__

namespace mlx
{
	class Surface
	{
		public:
			void Create(class Renderer& renderer);
			void Destroy() noexcept;
			
			VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& available_formats);

			inline VkSurfaceKHR& operator()() noexcept { return m_surface; }
			inline VkSurfaceKHR& Get() noexcept { return m_surface; }

		private:
			VkSurfaceKHR m_surface = VK_NULL_HANDLE;
	};
}

#endif // __MLX_VK_SURFACE__
