#ifndef __MLX_RENDER_CORE__
#define __MLX_RENDER_CORE__

#include <Renderer/Memory.h>

namespace mlx
{
	constexpr const int MAX_FRAMES_IN_FLIGHT = 3;

	class RenderCore : public Singleton<RenderCore>
	{
		friend class Singleton<RenderCore>;

		public:
			void Init() noexcept;
			void Destroy() noexcept;

			[[nodiscard]] MLX_FORCEINLINE VkInstance GetInstance() const noexcept { return m_instance; }
			[[nodiscard]] MLX_FORCEINLINE VkInstance& GetInstanceRef() noexcept { return m_instance; }
			[[nodiscard]] MLX_FORCEINLINE VkDevice GetDevice() const noexcept { return m_device; }
			[[nodiscard]] MLX_FORCEINLINE VkPhysicalDevice GetPhysicalDevice() const noexcept { return m_physical_device; }
			[[nodiscard]] MLX_FORCEINLINE GPUAllocator& GetAllocator() noexcept { return m_allocator; }

			inline void WaitDeviceIdle() const noexcept { vkDeviceWaitIdle(m_device); }

		private:
			RenderCore() = default;
			~RenderCore() = default;

		private:
			GPUAllocator m_allocator;
			VkInstance m_instance = VK_NULL_HANDLE;
			VkDevice m_device = VK_NULL_HANDLE;
			VkPhysicalDevice m_physical_device = VK_NULL_HANDLE;
	};
}

#endif
