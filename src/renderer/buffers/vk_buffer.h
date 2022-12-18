/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_buffer.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 23:18:52 by maldavid          #+#    #+#             */
/*   Updated: 2022/12/18 02:10:08 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_VK_BUFFER__
#define __MLX_VK_BUFFER__

#include <volk.h>
#include <renderer/core/render_core.h>

namespace mlx
{
	class Buffer
	{
		public:
			enum class kind { dynamic, uniform };

			void create(kind type, VkDeviceSize size, VkBufferUsageFlags usage, const void* data = nullptr);
			void destroy() noexcept;

			inline void mapMem(void** data = nullptr, VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0) noexcept
			{
				if(vkMapMemory(Render_Core::get().getDevice().get(), _memory, _offset + offset, size, 0, data) != VK_SUCCESS)
					core::error::report(e_kind::fatal_error, "Vulkan : failed to map a buffer");
			}
			inline void unmapMem() noexcept { vkUnmapMemory(Render_Core::get().getDevice().get(), _memory); }

			void flush(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

			inline unsigned int getSize() noexcept { return _size; }
			inline unsigned int getOffset() noexcept { return _offset; }
			inline VkDeviceMemory getDeviceMemory() noexcept { return _memory; }
			inline VkBuffer& operator()() noexcept { return _buffer; }
			inline VkBuffer& get() noexcept { return _buffer; }

		protected:
			VkDeviceMemory _memory = VK_NULL_HANDLE;
        	VkDeviceSize _offset = 0;
      		VkDeviceSize _size = 0;
			VkBuffer _buffer = VK_NULL_HANDLE;

		private:
			void createBuffer(VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);

			VkBufferUsageFlags _usage = 0;
			VkMemoryPropertyFlags _flags = 0;
	};
}

#endif
