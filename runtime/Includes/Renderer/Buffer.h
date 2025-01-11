#ifndef __MLX_GPU_BUFFER__
#define __MLX_GPU_BUFFER__

#include <Renderer/Enums.h>
#include <Renderer/RenderCore.h>
#include <Utils/Buffer.h>

namespace mlx
{
	class GPUBuffer
	{
		public:
			GPUBuffer() = default;

			void Init(BufferType type, VkDeviceSize size, VkBufferUsageFlags usage, CPUBuffer data, [[maybe_unused]] std::string_view debug_name);
			void Destroy() noexcept;

			bool CopyFrom(const GPUBuffer& buffer) noexcept;

			void Swap(GPUBuffer& buffer) noexcept;

			template<typename T = void*>
			[[nodiscard]] MLX_FORCEINLINE T GetMap() const noexcept { return reinterpret_cast<T>(p_map); }
			[[nodiscard]] MLX_FORCEINLINE VkBuffer Get() const noexcept { return m_buffer; }
			[[nodiscard]] MLX_FORCEINLINE VmaAllocation GetAllocation() const noexcept { return m_allocation; }
			[[nodiscard]] MLX_FORCEINLINE VkDeviceSize GetSize() const noexcept { return m_size; }
			[[nodiscard]] MLX_FORCEINLINE VkDeviceSize GetOffset() const noexcept { return m_offset; }

			[[nodiscard]] inline bool IsInit() const noexcept { return m_buffer != VK_NULL_HANDLE; }

			~GPUBuffer() = default;

		protected:
			void PushToGPU() noexcept;

		protected:
			#ifdef DEBUG
				std::string m_debug_name;
			#endif
			VkBuffer m_buffer = VK_NULL_HANDLE;
			VmaAllocation m_allocation;
			VkDeviceSize m_offset = 0;
			VkDeviceSize m_size = 0;
			void* p_map = nullptr;

		private:
			void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VmaAllocationCreateInfo alloc_info, [[maybe_unused]] std::string_view debug_name);

		private:
			VkBufferUsageFlags m_usage = 0;
	};

	class VertexBuffer : public GPUBuffer
	{
		public:
			inline void Init(std::uint32_t size, VkBufferUsageFlags additional_flags, [[maybe_unused]] std::string_view debug_name) { GPUBuffer::Init(BufferType::LowDynamic, size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | additional_flags, {}, std::move(debug_name)); }
			void SetData(CPUBuffer data);
			inline void Bind(VkCommandBuffer cmd) const noexcept { VkDeviceSize offset = 0; RenderCore::Get().vkCmdBindVertexBuffers(cmd, 0, 1, &m_buffer, &offset); }
	};

	class IndexBuffer : public GPUBuffer
	{
		public:
			inline void Init(std::uint32_t size, VkBufferUsageFlags additional_flags, [[maybe_unused]] std::string_view debug_name) { GPUBuffer::Init(BufferType::LowDynamic, size, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | additional_flags, {}, std::move(debug_name)); }
			void SetData(CPUBuffer data);
			inline void Bind(VkCommandBuffer cmd) const noexcept { RenderCore::Get().vkCmdBindIndexBuffer(cmd, m_buffer, 0, VK_INDEX_TYPE_UINT32); }
	};

	class UniformBuffer
	{
		public:
			void Init(std::uint32_t size, [[maybe_unused]] std::string_view debug_name);
			void SetData(CPUBuffer data, std::size_t frame_index);
			void Destroy() noexcept;

			inline VkDeviceSize GetSize(int i) const noexcept { return m_buffers[i].GetSize(); }
			inline VkDeviceSize GetOffset(int i) const noexcept { return m_buffers[i].GetOffset(); }
			inline VkBuffer GetVk(int i) const noexcept { return m_buffers[i].Get(); }
			inline GPUBuffer& Get(int i) noexcept { return m_buffers[i]; }

		private:
			std::array<GPUBuffer, MAX_FRAMES_IN_FLIGHT> m_buffers;
			std::array<void*, MAX_FRAMES_IN_FLIGHT> m_maps;
	};
}

#endif
