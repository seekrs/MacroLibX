/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_buffer.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 23:18:52 by maldavid          #+#    #+#             */
/*   Updated: 2023/11/09 19:38:30 by maldavid         ###   ########.fr       */
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
			enum class kind { dynamic, uniform, constant };

			void create(kind type, VkDeviceSize size, VkBufferUsageFlags usage, const void* data = nullptr);
			void destroy() noexcept;

			inline void mapMem(void** data = nullptr) noexcept
			{
				Render_Core::get().getAllocator().mapMemory(_allocation, data);
				_is_mapped = true;
			}
			inline bool isMapped() const noexcept { return _is_mapped; }
			inline void unmapMem() noexcept { Render_Core::get().getAllocator().unmapMemory(_allocation);_is_mapped = false; }

			void flush(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

			inline VkBuffer& operator()() noexcept { return _buffer; }
			inline VkBuffer& get() noexcept { return _buffer; }
			inline VkDeviceSize getSize() const noexcept { return _alloc_infos.size; }
			inline VkDeviceSize getOffset() const noexcept { return _alloc_infos.offset; }

		protected:
			void pushToGPU() noexcept;
			void swap(Buffer& buffer) noexcept;

		protected:
			VmaAllocation _allocation;
			VmaAllocationInfo _alloc_infos;
			VkBuffer _buffer = VK_NULL_HANDLE;

		private:
			void createBuffer(VkBufferUsageFlags usage, VmaAllocationCreateInfo info, VkDeviceSize size);

		private:
			VkBufferUsageFlags _usage = 0;
			bool _is_mapped = false;
	};
}

#endif
