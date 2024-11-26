#ifndef __MLX_RENDER_CORE__
#define __MLX_RENDER_CORE__

constexpr const int MAX_FRAMES_IN_FLIGHT = 2;

#include <Renderer/Memory.h>
#include <Renderer/Descriptor.h>

namespace mlx
{
	#if defined(DEBUG) && defined(VK_EXT_debug_utils)
		#define MLX_HAS_DEBUG_UTILS_FUNCTIONS
	#endif

	class RenderCore
	{
		public:
			RenderCore();

			[[nodiscard]] MLX_FORCEINLINE VkInstance GetInstance() const noexcept { return m_instance; }
			[[nodiscard]] MLX_FORCEINLINE VkInstance& GetInstanceRef() noexcept { return m_instance; }
			[[nodiscard]] MLX_FORCEINLINE VkDevice GetDevice() const noexcept { return m_device; }
			[[nodiscard]] MLX_FORCEINLINE VkPhysicalDevice GetPhysicalDevice() const noexcept { return m_physical_device; }
			[[nodiscard]] MLX_FORCEINLINE GPUAllocator& GetAllocator() noexcept { return m_allocator; }
			[[nodiscard]] inline DescriptorPoolManager& GetDescriptorPoolManager() noexcept { return m_descriptor_pool_manager; }

			inline void WaitDeviceIdle() const noexcept { vkDeviceWaitIdle(m_device); }

			inline static bool IsInit() noexcept { return s_instance != nullptr; }
			inline static RenderCore& Get() noexcept { return *s_instance; }

			#define MLX_VULKAN_GLOBAL_FUNCTION(fn) PFN_##fn fn = nullptr;
			#define MLX_VULKAN_INSTANCE_FUNCTION(fn) PFN_##fn fn = nullptr;
			#define MLX_VULKAN_DEVICE_FUNCTION(fn) PFN_##fn fn = nullptr;
				#include <Renderer/Vulkan/VulkanDefs.h>
			#undef MLX_VULKAN_GLOBAL_FUNCTION
			#undef MLX_VULKAN_INSTANCE_FUNCTION
			#undef MLX_VULKAN_DEVICE_FUNCTION

			~RenderCore();

		private:
			void LoadKVFGlobalVulkanFunctionPointers() const noexcept;
			void LoadKVFInstanceVulkanFunctionPointers() const noexcept;
			void LoadKVFDeviceVulkanFunctionPointers() const noexcept;

		private:
			static RenderCore* s_instance;

			DescriptorPoolManager m_descriptor_pool_manager;
			GPUAllocator m_allocator;
			VkInstance m_instance = VK_NULL_HANDLE;
			VkDevice m_device = VK_NULL_HANDLE;
			VkPhysicalDevice m_physical_device = VK_NULL_HANDLE;
	};
}

#endif
