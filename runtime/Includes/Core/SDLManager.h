#ifndef __MLX_SDL_MANAGER__
#define __MLX_SDL_MANAGER__

#include <mlx.h>
#include <Maths/Vec2.h>

namespace mlx
{
	class SDLManager
	{
		public:
			SDLManager();

			Handle CreateWindow(const mlx_window_create_info* info, std::int32_t& id, bool hidden);
			void DestroyWindow(Handle window) noexcept;

			void InputsFetcher(std::function<void(mlx_event_type, int, int)> functor);

			VkSurfaceKHR CreateVulkanSurface(Handle window, VkInstance instance) const noexcept;
			std::vector<const char*> GetRequiredVulkanInstanceExtentions(Handle window) const noexcept;
			Vec2ui GetVulkanDrawableSize(Handle window) const noexcept;
			void MoveMouseOnWindow(Handle window, int x, int y) const noexcept;
			void GetScreenSizeWindowIsOn(Handle window, int* x, int* y) const noexcept;
			void SetWindowPosition(Handle window, int x, int y) const noexcept;
			void SetWindowSize(Handle window, int x, int y) const noexcept;
			void SetWindowTitle(Handle window, std::string_view title) const noexcept;
			void SetWindowFullscreen(Handle window, bool enable) const noexcept;
			void SetWindowMaxSize(Handle window, int x, int y) const noexcept;
			void SetWindowMinSize(Handle window, int x, int y) const noexcept;
			void MaximizeWindow(Handle window) const noexcept;
			void MinimizeWindow(Handle window) const noexcept;
			void RestoreWindow(Handle window) const noexcept;

			void GetWindowPosition(Handle window, int* x, int* y) const noexcept;
			void GetWindowSize(Handle window, int* x, int* y) const noexcept;

			static void HideCursor() noexcept;
			static void ShowCursor() noexcept;

			std::int32_t GetX() const noexcept;
			std::int32_t GetY() const noexcept;
			std::int32_t GetXRel() const noexcept;
			std::int32_t GetYRel() const noexcept;

			inline static bool IsInit() noexcept { return s_instance != nullptr; }
			inline static SDLManager& Get() noexcept { return *s_instance; }

			~SDLManager();

		private:
			static SDLManager* s_instance;

			std::unordered_set<Handle> m_windows_registry;
			bool m_drop_sdl_responsability = false;
	};
}

#endif
