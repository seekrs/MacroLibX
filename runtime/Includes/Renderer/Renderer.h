#ifndef __MLX_RENDERER__
#define __MLX_RENDERER__

#include <Platform/Window.h>
#include <Utils/NonOwningPtr.h>
#include <Renderer/RenderCore.h>
#include <Renderer/Image.h>
#include <Core/EventBus.h>

namespace mlx
{
	class Renderer
	{
		public:
			Renderer() = default;

			void Init(NonOwningPtr<Window> window);

			bool BeginFrame();
			void EndFrame();

			[[nodiscard]] inline VkSwapchainKHR GetSwapchain() const noexcept { return m_swapchain; }
			[[nodiscard]] inline VkSurfaceKHR GetSurface() const noexcept { return m_surface; }
			[[nodiscard]] inline VkSemaphore GetImageAvailableSemaphore(int index) const noexcept { return m_image_available_semaphores[index]; }
			[[nodiscard]] inline VkSemaphore GetRenderFinishedSemaphore(int index) const noexcept { return m_render_finished_semaphores[index]; }
			[[nodiscard]] inline VkCommandBuffer GetCommandBuffer(int index) const noexcept { return m_cmd_buffers[index]; }
			[[nodiscard]] inline VkCommandBuffer GetActiveCommandBuffer() const noexcept { return m_cmd_buffers[m_current_frame_index]; }
			[[nodiscard]] inline const std::vector<Image>& GetSwapchainImages() const { return m_swapchain_images; }
			[[nodiscard]] inline std::size_t& GetDrawCallsCounterRef() noexcept { return m_drawcalls; }
			[[nodiscard]] inline std::size_t& GetPolygonDrawnCounterRef() noexcept { return m_polygons_drawn; }
			[[nodiscard]] inline std::size_t GetSwapchainImageIndex() const noexcept { return m_swapchain_image_index; }
			[[nodiscard]] inline std::size_t GetCurrentFrameIndex() const noexcept { return m_current_frame_index; }
			[[nodiscard]] inline NonOwningPtr<Window> GetWindow() const noexcept { return m_window_ptr; }

			MLX_FORCEINLINE constexpr void RequireFramebufferResize() noexcept { m_framebuffers_resize = true; }

			void Destroy() noexcept;

			~Renderer() = default;

		private:
			void CreateSwapchain();
			void DestroySwapchain();

		private:
			std::array<VkSemaphore, MAX_FRAMES_IN_FLIGHT> m_image_available_semaphores;
			std::array<VkSemaphore, MAX_FRAMES_IN_FLIGHT> m_render_finished_semaphores;
			std::array<VkCommandBuffer, MAX_FRAMES_IN_FLIGHT> m_cmd_buffers;
			std::array<VkFence, MAX_FRAMES_IN_FLIGHT> m_cmd_fences;
			std::vector<Image> m_swapchain_images;
			NonOwningPtr<Window> p_window;
			VkSurfaceKHR m_surface = VK_NULL_HANDLE;
			VkSwapchainKHR m_swapchain = VK_NULL_HANDLE;
			std::uint32_t m_current_frame_index = 0;
			std::uint32_t m_swapchain_image_index = 0;
			std::size_t m_drawcalls = 0;
			std::size_t m_polygons_drawn = 0;
			bool m_framebuffers_resize = false;
	};
}

#endif
