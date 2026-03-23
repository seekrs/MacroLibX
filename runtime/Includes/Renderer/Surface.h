#ifndef __MLX_SURFACE__
#define __MLX_SURFACE__

#include <Utils/NonOwningPtr.h>
#include <Renderer/Image.h>

namespace mlx
{
	class Surface
	{
		public:
			Surface() = default;

			void Init(NonOwningPtr<class Window> window);
			void Destroy();

			[[nodiscard]] inline VkSurfaceKHR Get() const noexcept { return m_surface; }

			~Surface() = default;

		private:
			#ifdef VK_USE_PLATFORM_ANDROID_KHR
				// VK_KHR_android_surface
				void Create(ANativeWindow* window, VkAndroidSurfaceCreateFlagsKHR flags = 0, const VkAllocationCallbacks* allocator = nullptr);
			#endif

			#ifdef VK_USE_PLATFORM_XCB_KHR
				// VK_KHR_xcb_surface
				void Create(xcb_connection_t* connection, xcb_window_t window, VkXcbSurfaceCreateFlagsKHR flags = 0, const VkAllocationCallbacks* allocator = nullptr);
			#endif

			#ifdef VK_USE_PLATFORM_XLIB_KHR
				// VK_KHR_xlib_surface
				void Create(Display* display, ::Window window, VkXlibSurfaceCreateFlagsKHR flags = 0, const VkAllocationCallbacks* allocator = nullptr);
			#endif

			#ifdef VK_USE_PLATFORM_WAYLAND_KHR
				// VK_KHR_wayland_surface
				void Create(wl_display* display, wl_surface* surface, VkWaylandSurfaceCreateFlagsKHR flags = 0, const VkAllocationCallbacks* allocator = nullptr);
			#endif

			#ifdef VK_USE_PLATFORM_WIN32_KHR
				// VK_KHR_win32_surface
				void Create(HINSTANCE instance, HWND handle, VkWin32SurfaceCreateFlagsKHR flags = 0, const VkAllocationCallbacks* allocator = nullptr);
			#endif

			#ifdef VK_USE_PLATFORM_METAL_EXT
				void Create(id layer, const VkAllocationCallbacks* allocator = nullptr);
			#endif

		private:
			VkSurfaceKHR m_surface = VK_NULL_HANDLE;
	};
}

#endif
