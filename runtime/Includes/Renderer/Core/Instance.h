/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Instance.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 19:03:04 by maldavid          #+#    #+#             */
/*   Updated: 2024/04/23 18:44:02 by maldavid         ###   ########.fr       */
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

			inline std::uint32_t GetInstanceVersion() const noexcept { return m_instance_version; }

			inline VkInstance& operator()() noexcept { return m_instance; }
			inline VkInstance& Get() noexcept { return m_instance; }

		private:
			std::vector<const char*> GetRequiredExtensions();

		private:
			VkInstance m_instance = VK_NULL_HANDLE;
			std::uint32_t m_instance_version = 0;
	};
}

#endif // __MLX_VK_INSTANCE__
