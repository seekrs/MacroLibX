/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Instance.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 19:03:04 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/27 22:48:55 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_VK_INSTANCE__
#define __MLX_VK_INSTANCE__

namespace mlx
{
	class Instance
	{
		public:
			void Init();
			void Destroy() noexcept;

			inline VkInstance& operator()() noexcept { return m_instance; }
			inline VkInstance& Get() noexcept { return m_instance; }

		private:
			std::vector<const char*> GetRequiredExtensions();

		private:
			VkInstance m_instance = VK_NULL_HANDLE;
	};
}

#endif // __MLX_VK_INSTANCE__
