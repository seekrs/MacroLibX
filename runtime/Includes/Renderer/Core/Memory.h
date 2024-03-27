/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Memory.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 02:13:03 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/27 22:49:57 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_VK_MEMORY__
#define __MLX_VK_MEMORY__

namespace mlx
{
	class GPUallocator
	{
		public:
			GPUallocator() = default;

			void Init() noexcept;
			void Destroy() noexcept;

			VmaAllocation CreateBuffer(const VkBufferCreateInfo* binfo, const VmaAllocationCreateInfo* vinfo, VkBuffer& buffer, const char* name = nullptr) noexcept;
			void DestroyBuffer(VmaAllocation allocation, VkBuffer buffer) noexcept;

			VmaAllocation CreateImage(const VkImageCreateInfo* iminfo, const VmaAllocationCreateInfo* vinfo, VkImage& image, const char* name = nullptr) noexcept;
			void DestroyImage(VmaAllocation allocation, VkImage image) noexcept;

			void MapMemory(VmaAllocation allocation, void** data) noexcept;
			void UnmapMemory(VmaAllocation allocation) noexcept;

			void DumpMemoryToJson();

			void Flush(VmaAllocation allocation, VkDeviceSize size, VkDeviceSize offset) noexcept;

			~GPUallocator() = default;

		private:
			VmaAllocator m_allocator;
			std::int32_t m_active_buffers_allocations = 0;
			std::int32_t m_active_images_allocations = 0;
	};
}

#endif
