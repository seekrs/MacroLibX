#ifndef __MLX_WINDOW__
#define __MLX_WINDOW__

#include <Maths/Vec2.h>
#include <Core/SDLManager.h>

namespace mlx
{
	class Window
	{
		public:
			Window(std::size_t w, std::size_t h, const std::string& title, bool is_resizable, bool hidden = false);

			inline Handle GetWindowHandle() const noexcept { return p_window; }
			inline int GetWidth() const noexcept { return m_width; }
			inline int GetHeight() const noexcept { return m_height; }
			inline std::uint32_t GetID() const noexcept { return m_id; }
			inline const std::string& GetName() const { return m_name; }

			inline void MoveMouse(int x, int y) { SDLManager::Get().MoveMouseOnWindow(p_window, x, y); }
			inline void GetScreenSizeWindowIsOn(int* x, int* y) { SDLManager::Get().GetScreenSizeWindowIsOn(p_window, x, y); }
			inline void SetPosition(int x, int y) { SDLManager::Get().SetWindowPosition(p_window, x, y); }

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
