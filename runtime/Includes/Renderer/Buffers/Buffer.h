/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Buffer.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 23:18:52 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/27 22:09:07 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_VK_BUFFER__
#define __MLX_VK_BUFFER__

#include <Renderer/Enums.h>
#include <Renderer/Core/RenderCore.h>
#include <Renderer/Command/CommandResource.h>

namespace mlx
{
	class Buffer : public CommandResource
	{
		public:
			Buffer() = default;

			void Create(BufferType type, VkDeviceSize size, VkBufferUsageFlags usage, const char* name, const void* data = nullptr);
			void Destroy() noexcept;

			inline void MapMem(void** data) noexcept { Render_Core::get().getAllocator().mapMemory(m_allocation, data); m_is_mapped = true; }
			inline bool IsMapped() const noexcept { return m_is_mapped; }
			inline void UnmapMem() noexcept { Render_Core::get().getAllocator().unmapMemory(m_allocation); m_is_mapped = false; }

			void Flush(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
			bool CopyFromBuffer(const Buffer& buffer) noexcept;

			inline VkBuffer& operator()() noexcept { return m_buffer; }
			inline VkBuffer& Get() noexcept { return m_buffer; }
			inline VkDeviceSize GetSize() const noexcept { return m_size; }
			inline VkDeviceSize GetOffset() const noexcept { return m_offset; }

			~Buffer() = default;

		protected:
			void PushToGPU() noexcept;
			void Swap(Buffer& buffer) noexcept;

		protected:
			VmaAllocation m_allocation;
			VkBuffer m_buffer = VK_NULL_HANDLE;
			VkDeviceSize m_offset = 0;
			VkDeviceSize m_size = 0;

		private:
			void CreateBuffer(VkBufferUsageFlags usage, VmaAllocationCreateInfo info, VkDeviceSize size, const char* name);

		private:
			#ifdef DEBUG
				std::string m_name;
			#endif
			VkBufferUsageFlags m_usage = 0;
			bool m_is_mapped = false;
	};
}

#endif
