#ifndef __MLX_SWAPCHAIN__
#define __MLX_SWAPCHAIN__

#include <Utils/NonOwningPtr.h>
#include <Renderer/Image.h>

namespace mlx
{
	class Swapchain
	{
		public:
			Swapchain() = default;

			void Init(NonOwningPtr<class Window> window);
			void AquireFrame(VkSemaphore signal);
			void Present(VkSemaphore wait) noexcept;
			void Destroy();

			[[nodiscard]] inline VkSwapchainKHR Get() const noexcept { return m_swapchain; }
			[[nodiscard]] inline VkSurfaceKHR GetSurface() const noexcept { return m_surface; }
			[[nodiscard]] inline std::uint32_t GetImagesCount() const noexcept { return m_images_count; }
			[[nodiscard]] inline std::uint32_t GetMinImagesCount() const noexcept { return m_min_images_count; }
			[[nodiscard]] inline std::uint32_t GetImageIndex() const noexcept { return m_current_image_index; }
			[[nodiscard]] inline const std::vector<Image>& GetSwapchainImages() const { return m_swapchain_images; }

			~Swapchain() = default;

		private:
			void CreateSwapchain();

		private:
			std::vector<Image> m_swapchain_images;
			VkSwapchainKHR m_swapchain = VK_NULL_HANDLE;
			VkSurfaceKHR m_surface = VK_NULL_HANDLE;
			NonOwningPtr<class Window> p_window;
			std::uint32_t m_images_count = 0;
			std::uint32_t m_min_images_count = 0;
			std::uint32_t m_current_image_index = 0;
			bool m_resize = false;
	};
}

#endif
