#ifndef __MLX_SDL_MANAGER__
#define __MLX_SDL_MANAGER__

#include <mlx.h>
#include <Maths/Vec2.h>
#include <Renderer/Image.h>

namespace mlx
{
	class SDLManager
	{
		public:
			SDLManager();

			Handle CreateWindow(const mlx_window_create_info* info, std::int32_t& id, bool hidden);
			void DestroyWindow(Handle window) noexcept;

			SDL_Window* GetRawWindow(Handle window) noexcept;

			void InputsFetcher(std::function<void(mlx_event_type, int, int)> functor);
			void SetInputBinding(std::function<void(SDL_Event*)> functor);
			void SendInactiveEvents(std::function<void(mlx_event_type, int, int)> functor);
			void HandleControllerDeviceEvent(std::function<void(mlx_event_type, int, int)> functor, SDL_Event event);

			VkSurfaceKHR CreateVulkanSurface(Handle window, VkInstance instance) const noexcept;
			Vec2ui GetVulkanDrawableSize(Handle window) const noexcept;
			void MoveMouseOnWindow(Handle window, int x, int y) const noexcept;
			void GetScreenSizeWindowIsOn(Handle window, int* x, int* y) const noexcept;
			void SetWindowPosition(Handle window, int x, int y) const noexcept;
			void SetWindowSize(Handle window, int x, int y) const noexcept;
			void SetWindowTitle(Handle window, std::string_view title) const noexcept;
			void SetWindowIcon(Handle window, NonOwningPtr<Texture> texture) const noexcept;
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

			int GetControllerIdFromSDL(int joystick_id) const noexcept;
			bool IsControllerConnected(int controller_id) const noexcept;
			int GetFirstConnectedController() const noexcept;
			int AddController(int joystick_index) noexcept;
			int RemoveController(int joystick_id) noexcept;
			void RemoveAllControllers() noexcept;
			float GetControllerAxis(int controller_id, int axis_kind) const noexcept;
			void RumbleController(int controller_id, float low_freq, float high_freq, float duration) const noexcept;

			inline static bool IsInit() noexcept { return s_instance != nullptr; }
			inline static SDLManager& Get() noexcept { return *s_instance; }

			~SDLManager();

		private:
			struct EventRequest
			{
				mlx_event_type type;
				int code = -1;

				EventRequest(mlx_event_type type, int code) : type(type), code(code) {};
			};

			typedef SDL_GameController* Controller;

		private:
			static SDLManager* s_instance;

			std::function<void(SDL_Event*)> m_binding_hook;
			std::unordered_set<Handle> m_windows_registry;
			std::vector<EventRequest> m_inactive_events;
			std::vector<Controller> m_controllers;
			int m_active_window_id = -1;
			bool m_drop_sdl_responsability = false;
	};
}

#endif
