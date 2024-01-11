/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_buffer.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 23:18:52 by maldavid          #+#    #+#             */
/*   Updated: 2024/01/11 05:16:58 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_VK_BUFFER__
#define __MLX_VK_BUFFER__

#include <mlx_profile.h>
#include <volk.h>
#include <renderer/core/render_core.h>
#include <renderer/core/cmd_resource.h>

namespace mlx
{
	class Buffer : public CmdResource
	{
		public:
			enum class kind { dynamic, dynamic_device_local, uniform, constant };

			void create(kind type, VkDeviceSize size, VkBufferUsageFlags usage, const char* name, const void* data = nullptr);
			void destroy() noexcept;

			inline void mapMem(void** data) noexcept { Render_Core::get().getAllocator().mapMemory(_allocation, data); _is_mapped = true; }
			inline bool isMapped() const noexcept { return _is_mapped; }
			inline void unmapMem() noexcept { Render_Core::get().getAllocator().unmapMemory(_allocation); _is_mapped = false; }

			void flush(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
			bool copyFromBuffer(const Buffer& buffer) noexcept;

			inline VkBuffer& operator()() noexcept { return _buffer; }
			inline VkBuffer& get() noexcept { return _buffer; }
			inline VkDeviceSize getSize() const noexcept { return _size; }
			inline VkDeviceSize getOffset() const noexcept { return _offset; }

		protected:
			void pushToGPU() noexcept;
			void swap(Buffer& buffer) noexcept;

		protected:
			VmaAllocation _allocation;
			VkBuffer _buffer = VK_NULL_HANDLE;
			VkDeviceSize _offset = 0;
			VkDeviceSize _size = 0;

		private:
			void createBuffer(VkBufferUsageFlags usage, VmaAllocationCreateInfo info, VkDeviceSize size, const char* name);

		private:
			#ifdef DEBUG
				std::string _name;
			#endif
			VkBufferUsageFlags _usage = 0;
			bool _is_mapped = false;
	};
}

#endif
