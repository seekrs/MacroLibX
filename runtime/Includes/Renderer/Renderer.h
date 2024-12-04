#ifndef __MLX_RENDERER__
#define __MLX_RENDERER__

#include <Platform/Window.h>
#include <Utils/NonOwningPtr.h>
#include <Renderer/RenderCore.h>
#include <Renderer/Image.h>
#include <Core/EventBus.h>
#include <Renderer/Swapchain.h>

namespace mlx
{
	class Renderer
	{
		public:
			Renderer() = default;

			void Init(NonOwningPtr<Window> window);
			void Init(NonOwningPtr<Texture> render_target);

			void BeginFrame();
			void EndFrame();

			[[nodiscard]] inline VkSemaphore GetImageAvailableSemaphore(int index) const noexcept { return m_image_available_semaphores[index]; }
			[[nodiscard]] inline VkSemaphore GetRenderFinishedSemaphore(int index) const noexcept { return m_render_finished_semaphores[index]; }
			[[nodiscard]] inline VkCommandBuffer GetCommandBuffer(int index) const noexcept { return m_cmd_buffers[index]; }
			[[nodiscard]] inline VkCommandBuffer GetActiveCommandBuffer() const noexcept { return m_cmd_buffers[m_current_frame_index]; }
			[[nodiscard]] inline std::size_t& GetDrawCallsCounterRef() noexcept { return m_drawcalls; }
			[[nodiscard]] inline std::size_t& GetPolygonDrawnCounterRef() noexcept { return m_polygons_drawn; }
			[[nodiscard]] inline std::size_t GetCurrentFrameIndex() const noexcept { return m_current_frame_index; }
			[[nodiscard]] inline NonOwningPtr<Window> GetWindow() const noexcept { return p_window; }
			[[nodiscard]] inline NonOwningPtr<Texture> GetRenderTarget() const noexcept { return p_render_target; }
			[[nodiscard]] inline const Swapchain& GetSwapchain() const noexcept { return m_swapchain; }

			void Destroy() noexcept;

			~Renderer() = default;

		private:
			Swapchain m_swapchain;
			std::array<VkSemaphore, MAX_FRAMES_IN_FLIGHT> m_image_available_semaphores;
			std::array<VkSemaphore, MAX_FRAMES_IN_FLIGHT> m_render_finished_semaphores;
			std::array<VkCommandBuffer, MAX_FRAMES_IN_FLIGHT> m_cmd_buffers;
			std::array<VkFence, MAX_FRAMES_IN_FLIGHT> m_cmd_fences;
			NonOwningPtr<Window> p_window;
			NonOwningPtr<Texture> p_render_target;
			std::uint32_t m_current_frame_index = 0;
			std::size_t m_polygons_drawn = 0;
			std::size_t m_drawcalls = 0;
	};
}

#endif
