/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UniformBuffer.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 18:45:29 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/27 22:15:23 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_VK_UBO__
#define __MLX_VK_UBO__

#include <Renderer/Buffer/Buffer.h>

namespace mlx
{
	class UniformBuffer
	{
		public:
			UniformBuffer() = default;

			void Create(NonOwningPtr<class Renderer> renderer, std::uint32_t size, const char* name);
			void Destroy() noexcept;

			void SetData(std::uint32_t size, const void* data);

			VkDeviceSize GetSize() noexcept;
			VkDeviceSize GetOffset() noexcept;
			VkDeviceMemory GetDeviceMemory() noexcept;
			VkBuffer& operator()() noexcept;
			VkBuffer& Get() noexcept;

			inline VkDeviceSize GetSize(int i) noexcept { return m_buffers[i].getSize(); }
			inline VkDeviceSize GetOffset(int i) noexcept { return m_buffers[i].getOffset(); }
			inline VkBuffer& operator()(int i) noexcept { return m_buffers[i].get(); }
			inline VkBuffer& Get(int i) noexcept { return m_buffers[i].get(); }

			~UniformBuffer() = default;

		private:
			std::array<Buffer, MAX_FRAMES_IN_FLIGHT> m_buffers;
			std::array<void*, MAX_FRAMES_IN_FLIGHT> m_maps;
			NonOwningPtr<class Renderer> m_renderer;
	};
}

#endif // __MLX_VK_UBO__
