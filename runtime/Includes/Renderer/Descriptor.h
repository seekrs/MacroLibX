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
		std::uint32_t binding;
	};

	class DescriptorPool
	{
		public:
			DescriptorPool() = default;

			void Init() noexcept;
			void Destroy() noexcept;

			std::shared_ptr<class DescriptorSet> RequestDescriptorSet(const ShaderSetLayout& layout, ShaderType shader_type);
			void ReturnDescriptorSet(std::shared_ptr<class DescriptorSet> set);

			[[nodiscard]] inline VkDescriptorPool Get() const noexcept { return m_pool; }
			[[nodiscard]] MLX_FORCEINLINE std::size_t GetNumberOfSetsAllocated() const noexcept { return m_allocation_count; }

			~DescriptorPool() = default;

		private:
			std::vector<std::shared_ptr<class DescriptorSet>> m_free_sets;
			std::vector<std::shared_ptr<class DescriptorSet>> m_used_sets;
			VkDescriptorPool m_pool;
			std::size_t m_allocation_count = 0;
	};

	class DescriptorPoolManager
	{
		public:
			DescriptorPoolManager() = default;

			DescriptorPool& GetAvailablePool();
			void Destroy();

			~DescriptorPoolManager() = default;

		private:
			std::vector<DescriptorPool> m_pools;
	};

	class DescriptorSet : public std::enable_shared_from_this<DescriptorSet>
	{
		friend DescriptorPool;

		public:
			void SetImage(std::size_t i, std::uint32_t binding, class Image& image);
			void SetStorageBuffer(std::size_t i, std::uint32_t binding, class GPUBuffer& buffer);
			void SetUniformBuffer(std::size_t i, std::uint32_t binding, class GPUBuffer& buffer);
			void Update(std::size_t i, VkCommandBuffer cmd = VK_NULL_HANDLE) noexcept;

			void ReturnDescriptorSetToPool();

			[[nodiscard]] inline VkDescriptorSet GetSet(std::size_t i) const noexcept { return m_sets[i]; }
			[[nodiscard]] MLX_FORCEINLINE bool IsInit() const noexcept { return m_sets[0] != VK_NULL_HANDLE; }
			[[nodiscard]] MLX_FORCEINLINE VkDescriptorSetLayout GetVulkanLayout() const noexcept { return m_set_layout; }
			[[nodiscard]] MLX_FORCEINLINE const ShaderSetLayout& GetShaderLayout() const { return m_shader_layout; }
			[[nodiscard]] MLX_FORCEINLINE ShaderType GetShaderType() const noexcept { return m_shader_type; }

			~DescriptorSet() = default;

		private:
			DescriptorSet(DescriptorPool& pool, VkDescriptorSetLayout vulkan_layout, const ShaderSetLayout& layout, std::array<VkDescriptorSet, MAX_FRAMES_IN_FLIGHT> vulkan_sets, ShaderType shader_type);

		private:
			ShaderSetLayout m_shader_layout;
			std::vector<Descriptor> m_descriptors;
			std::array<VkDescriptorSet, MAX_FRAMES_IN_FLIGHT> m_sets;
			VkDescriptorSetLayout m_set_layout;
			ShaderType m_shader_type;
			DescriptorPool& m_pool;
	};
}

#endif
