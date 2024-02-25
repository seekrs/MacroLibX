/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_ubo.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 18:45:29 by maldavid          #+#    #+#             */
/*   Updated: 2023/12/08 19:06:28 by kbz_8            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_VK_UBO__
#define __MLX_VK_UBO__

#include "vk_buffer.h"
#include <array>
#include <cstddef>
#include <mlx_profile.h>

namespace mlx
{
	class UBO
	{
		public:
			void create(class Renderer* renderer, std::uint32_t size, const char* name);

			void setData(std::uint32_t size, const void* data);
			void setDynamicData(std::uint32_t size, const void* data);

			void destroy() noexcept;

			unsigned int getSize() noexcept;
			unsigned int getOffset() noexcept;
			VkDeviceMemory getDeviceMemory() noexcept;
			VkBuffer& operator()() noexcept;
			VkBuffer& get() noexcept;

			inline unsigned int getSize(int i) noexcept { return _buffers[i].getSize(); }
			inline unsigned int getOffset(int i) noexcept { return _buffers[i].getOffset(); }
			inline VkBuffer& operator()(int i) noexcept { return _buffers[i].get(); }
			inline VkBuffer& get(int i) noexcept { return _buffers[i].get(); }

		private:
			std::array<Buffer, MAX_FRAMES_IN_FLIGHT> _buffers;
			std::array<void*, MAX_FRAMES_IN_FLIGHT> _maps;
			class Renderer* _renderer = nullptr;
	};
}

#endif // __MLX_VK_UBO__
