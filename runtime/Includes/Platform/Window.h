#ifndef __MLX_WINDOW__
#define __MLX_WINDOW__

#include <mlx.h>
#include <Maths/Vec2.h>
#include <Core/SDLManager.h>

namespace mlx
{
	class Window
	{
		public:
			Window(const mlx_window_create_info* info, bool hidden = false);

			inline Handle GetWindowHandle() const noexcept { return p_window; }
			inline int GetWidth() noexcept { SDLManager::Get().GetWindowSize(p_window, &m_width, &m_height); return m_width; }
			inline int GetHeight() noexcept { SDLManager::Get().GetWindowSize(p_window, &m_width, &m_height); return m_height; }
			inline std::uint32_t GetID() const noexcept { return m_id; }
			inline const std::string& GetName() const { return m_name; }

			inline void MoveMouse(int x, int y) { SDLManager::Get().MoveMouseOnWindow(p_window, x, y); }
			inline void GetScreenSizeWindowIsOn(int* x, int* y) { SDLManager::Get().GetScreenSizeWindowIsOn(p_window, x, y); }

			inline void SetPosition(int x, int y) { SDLManager::Get().SetWindowPosition(p_window, x, y); }
			inline void SetSize(int x, int y) { SDLManager::Get().SetWindowSize(p_window, x, y); m_width = x; m_height = y; }
			inline void SetTitle(std::string title) { SDLManager::Get().SetWindowTitle(p_window, title); m_name = std::move(title); }
			inline void SetFullscreen(bool enable) { SDLManager::Get().SetWindowFullscreen(p_window, enable); }

			inline void GetPosition(int* x, int* y) { SDLManager::Get().GetWindowPosition(p_window, x, y); }
			inline void GetSize(int* x, int* y) { *x = GetWidth(); *y = GetHeight(); }

			inline VkSurfaceKHR CreateVulkanSurface(VkInstance instance) const noexcept { return SDLManager::Get().CreateVulkanSurface(p_window, instance); }
			inline std::vector<const char*> GetRequiredVulkanInstanceExtentions() const noexcept { return SDLManager::Get().GetRequiredVulkanInstanceExtentions(p_window); }
			inline Vec2ui GetVulkanDrawableSize() const noexcept { return SDLManager::Get().GetVulkanDrawableSize(p_window); }

			void Destroy() noexcept;

			~Window() { Destroy(); }

		private:
			std::string m_name;
			Handle p_window = nullptr;
			std::int32_t m_id;
			int m_width = 0;
			int m_height = 0;
	};
}

#endif
