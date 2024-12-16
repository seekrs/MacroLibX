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

			MLX_FORCEINLINE Handle GetWindowHandle() const noexcept { return p_window; }
			MLX_FORCEINLINE int GetWidth() noexcept { SDLManager::Get().GetWindowSize(p_window, &m_width, &m_height); return m_width; }
			MLX_FORCEINLINE int GetHeight() noexcept { SDLManager::Get().GetWindowSize(p_window, &m_width, &m_height); return m_height; }
			MLX_FORCEINLINE std::uint32_t GetID() const noexcept { return m_id; }
			MLX_FORCEINLINE const std::string& GetName() const { return m_name; }

			MLX_FORCEINLINE void MoveMouse(int x, int y) { SDLManager::Get().MoveMouseOnWindow(p_window, x, y); }
			MLX_FORCEINLINE void GetScreenSizeWindowIsOn(int* x, int* y) { SDLManager::Get().GetScreenSizeWindowIsOn(p_window, x, y); }

			MLX_FORCEINLINE void SetPosition(int x, int y) { SDLManager::Get().SetWindowPosition(p_window, x, y); }
			MLX_FORCEINLINE void SetSize(int x, int y) { SDLManager::Get().SetWindowSize(p_window, x, y); m_width = x; m_height = y; }
			MLX_FORCEINLINE void SetTitle(std::string title) { SDLManager::Get().SetWindowTitle(p_window, title); m_name = std::move(title); }
			MLX_FORCEINLINE void SetFullscreen(bool enable) { SDLManager::Get().SetWindowFullscreen(p_window, enable); }
			MLX_FORCEINLINE void SetMaxSize(int x, int y) { SDLManager::Get().SetWindowMaxSize(p_window, x, y); }
			MLX_FORCEINLINE void SetMinSize(int x, int y) { SDLManager::Get().SetWindowMinSize(p_window, x, y); }
			MLX_FORCEINLINE void Maximize() { SDLManager::Get().MaximizeWindow(p_window); }
			MLX_FORCEINLINE void Minimize() { SDLManager::Get().MinimizeWindow(p_window); }
			MLX_FORCEINLINE void Restore() { SDLManager::Get().RestoreWindow(p_window); }

			MLX_FORCEINLINE void GetPosition(int* x, int* y) { SDLManager::Get().GetWindowPosition(p_window, x, y); }
			MLX_FORCEINLINE void GetSize(int* x, int* y) { *x = GetWidth(); *y = GetHeight(); }

			MLX_FORCEINLINE VkSurfaceKHR CreateVulkanSurface(VkInstance instance) const noexcept { return SDLManager::Get().CreateVulkanSurface(p_window, instance); }
			MLX_FORCEINLINE std::vector<const char*> GetRequiredVulkanInstanceExtentions() const noexcept { return SDLManager::Get().GetRequiredVulkanInstanceExtentions(p_window); }
			MLX_FORCEINLINE Vec2ui GetVulkanDrawableSize() const noexcept { return SDLManager::Get().GetVulkanDrawableSize(p_window); }

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
