#ifndef __MLX_SDL_MANAGER__
#define __MLX_SDL_MANAGER__

#include <Maths/Vec2.h>

namespace mlx
{
	class SDLManager
	{
		public:
			SDLManager();

			Handle CreateWindow(const std::string& title, std::size_t w, std::size_t h, bool hidden, std::int32_t& id);
			void DestroyWindow(Handle window) noexcept;

			void InputsFetcher(func::function<void(mlx_event_type, int, int)> functor);

			VkSurfaceKHR CreateVulkanSurface(Handle window, VkInstance instance) const noexcept;
			std::vector<const char*> GetRequiredVulkanInstanceExtentions(Handle window) const noexcept;
			Vec2ui GetVulkanDrawableSize(Handle window) const noexcept;
			void MoveMouseOnWindow(Handle window, int x, int y) const noexcept;
			void GetScreenSizeWindowIsOn(Handle window, int* x, int* y) const noexcept;
			void SetWindowPosition(Handle window, int x, int y) const noexcept;

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
