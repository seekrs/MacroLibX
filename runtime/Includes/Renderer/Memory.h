#ifndef __MLX_VK_MEMORY__
#define __MLX_VK_MEMORY__

namespace mlx
{
	class GPUAllocator
	{
		public:
			GPUAllocator() = default;

			void Init(const VkAllocationCallbacks* callbacks) noexcept;
			void Destroy() noexcept;

			VmaAllocation CreateBuffer(const VkBufferCreateInfo* binfo, const VmaAllocationCreateInfo* vinfo, VkBuffer& buffer, const char* name = nullptr) noexcept;
			void DestroyBuffer(VmaAllocation allocation, VkBuffer buffer, const char* name) noexcept;

			VmaAllocation CreateImage(const VkImageCreateInfo* iminfo, const VmaAllocationCreateInfo* vinfo, VkImage& image, const char* name = nullptr) noexcept;
			void DestroyImage(VmaAllocation allocation, VkImage image, const char* name) noexcept;

			void MapMemory(VmaAllocation allocation, void** data) noexcept;
			void UnmapMemory(VmaAllocation allocation) noexcept;

			void DumpMemoryToJson();

			void Flush(VmaAllocation allocation, VkDeviceSize size, VkDeviceSize offset) noexcept;

			~GPUAllocator() = default;

		private:
			VmaAllocator m_allocator;
			std::int32_t m_active_buffers_allocations = 0;
			std::int32_t m_active_images_allocations = 0;
	};
}

#endif
