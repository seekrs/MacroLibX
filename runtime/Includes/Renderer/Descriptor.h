#ifndef __MLX_DESCRIPTOR_SET__
#define __MLX_DESCRIPTOR_SET__

#include <Renderer/Enums.h>
#include <Renderer/RenderCore.h>
#include <Renderer/Pipelines/Shader.h>
#include <Renderer/Vulkan/VulkanPrototypes.h>

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

	class DescriptorSet
	{
		public:
			DescriptorSet() { m_set.fill(VK_NULL_HANDLE); }
			DescriptorSet(const ShaderSetLayout& layout, VkDescriptorSetLayout vklayout, ShaderType shader_type);

			void SetImage(std::size_t i, std::uint32_t binding, class Image& image);
			void SetStorageBuffer(std::size_t i, std::uint32_t binding, class GPUBuffer& buffer);
			void SetUniformBuffer(std::size_t i, std::uint32_t binding, class GPUBuffer& buffer);
			void Update(std::size_t i, VkCommandBuffer cmd = VK_NULL_HANDLE) noexcept;

			[[nodiscard]] inline VkDescriptorSet GetSet(std::size_t i) const noexcept { return m_set[i]; }
			[[nodiscard]] inline DescriptorSet Duplicate() const { return DescriptorSet{ m_set_layout, m_descriptors }; }
			[[nodiscard]] inline bool IsInit() const noexcept { return m_set[0] != VK_NULL_HANDLE; }

			~DescriptorSet() = default;

		private:
			DescriptorSet(VkDescriptorSetLayout layout, const std::vector<Descriptor>& descriptors);

		private:
			std::vector<Descriptor> m_descriptors;
			std::array<VkDescriptorSet, MAX_FRAMES_IN_FLIGHT> m_set;
			VkDescriptorSetLayout m_set_layout;
	};
}

#endif
