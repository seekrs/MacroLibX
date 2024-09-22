#ifndef __MLX_DESCRIPTOR_SET__
#define __MLX_DESCRIPTOR_SET__

#include <Renderer/Enums.h>
#include <Renderer/RenderCore.h>
#include <Renderer/Pipelines/Shader.h>

namespace mlx
{
	struct Descriptor
	{
		NonOwningPtr<class GPUBuffer> storage_buffer_ptr;
		NonOwningPtr<class GPUBuffer> uniform_buffer_ptr;
		NonOwningPtr<class Image> image_ptr;
		VkDescriptorType type;
		ShaderType shader_type;
		std::uint32_t binding;
	};

	class DescriptorPool
	{
		public:
			DescriptorPool() = default;

			void Init() noexcept;
			void Destroy() noexcept;

			VkDescriptorSet AllocateDescriptorSet(std::uint32_t frame_index, VkDescriptorSetLayout layout);

			void ResetPoolFromFrameIndex(std::size_t frame_index);
			
			[[nodiscard]] inline VkDescriptorPool Get(std::uint32_t index) const noexcept { return m_pools[index]; }
			[[nodiscard]] MLX_FORCEINLINE std::size_t GetNumberOfSetsAllocated() const noexcept { return m_allocation_count; }

			~DescriptorPool() = default;

		private:
			std::array<VkDescriptorPool, MAX_FRAMES_IN_FLIGHT> m_pools;
			std::size_t m_allocation_count = 0;
	};

	class DescriptorPoolManager
	{
		public:
			DescriptorPoolManager() = default;

			void ResetPoolsFromFrameIndex(std::size_t frame_index);
			DescriptorPool& GetAvailablePool();
			void Destroy();

			~DescriptorPoolManager() = default;

		private:
			std::list<DescriptorPool> m_pools;
	};

	class DescriptorSet
	{
		public:
			DescriptorSet() { m_set.fill(VK_NULL_HANDLE); }
			DescriptorSet(DescriptorPoolManager& pools_manager, const ShaderSetLayout& layout, VkDescriptorSetLayout vklayout, ShaderType shader_type);

			void SetImage(std::size_t i, std::uint32_t binding, class Image& image);
			void SetStorageBuffer(std::size_t i, std::uint32_t binding, class GPUBuffer& buffer);
			void SetUniformBuffer(std::size_t i, std::uint32_t binding, class GPUBuffer& buffer);
			void Update(std::size_t i, VkCommandBuffer cmd = VK_NULL_HANDLE) noexcept;
			void Reallocate(std::size_t frame_index) noexcept;

			[[nodiscard]] inline VkDescriptorSet GetSet(std::size_t i) const noexcept { return m_set[i]; }
			[[nodiscard]] inline DescriptorSet Duplicate() const { return DescriptorSet{ *p_pools_manager, m_set_layout, m_descriptors }; }
			[[nodiscard]] inline bool IsInit() const noexcept { return m_set[0] != VK_NULL_HANDLE; }

			~DescriptorSet() = default;

		private:
			DescriptorSet(DescriptorPoolManager& pools_manager, VkDescriptorSetLayout layout, const std::vector<Descriptor>& descriptors);

		private:
			std::vector<Descriptor> m_descriptors;
			std::array<VkDescriptorSet, MAX_FRAMES_IN_FLIGHT> m_set;
			VkDescriptorSetLayout m_set_layout;
			NonOwningPtr<DescriptorPoolManager> p_pools_manager;
	};
}

#endif
