#ifndef __MLX_SDL_MANAGER__
#define __MLX_SDL_MANAGER__

#include <PreCompiled.h>
#include <mlx.h>
#include <Maths/Vec2.h>
#include <Renderer/Image.h>
#include <Core/Audio.h>

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
			void HandleTextInputEvent(std::function<void(mlx_event_type, int, int)> functor, SDL_Event event);
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
			void SetCursorIcon(int icon) noexcept;

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

			Sound* CreateSoundFromWAV(const char* path, float* duration) noexcept;
			inline bool SoundExists(Sound* sound) const noexcept { return m_sounds.contains(sound); };
			void DestroySound(Sound* sound) noexcept;

			AudioChannel* CreateAudioChannel() noexcept;
			inline bool AudioChannelExists(AudioChannel* channel) const noexcept { return m_audio_channels.contains(channel); };
			void DestroyAudioChannel(AudioChannel* channel) noexcept;

			inline SDL_AudioSpec GetAudioDeviceSpec() const noexcept { return m_audio_device_spec; }
			void CheckAudioAllocs() const noexcept;

			inline static bool IsInit() noexcept { return s_instance != nullptr; }
			inline static SDLManager& Get() noexcept { return *s_instance; }

			~SDLManager();

			struct AudioDeviceLock
			{
				AudioDeviceLock() { SDL_LockAudioDevice(s_instance->m_audio_device); }
				~AudioDeviceLock() { SDL_UnlockAudioDevice(s_instance->m_audio_device); }
			};

		private:
			struct EventRequest
			{
				mlx_event_type type;
				int code = -1;

				EventRequest(mlx_event_type type, int code) : type(type), code(code) {};
			};
			using Controller = SDL_GameController*;

		private:
			static SDLManager* s_instance;

			static void AudioCallback(void*, uint8_t*, int) noexcept;

			std::function<void(SDL_Event*)> m_binding_hook;
			std::unordered_set<Handle> m_windows_registry;
			std::vector<EventRequest> m_inactive_events;
			std::vector<Controller> m_controllers;
			std::unordered_set<AudioChannel*> m_audio_channels;
			std::unordered_set<Sound*> m_sounds;
			SDL_AudioDeviceID m_audio_device;
			SDL_AudioSpec m_audio_device_spec;
			SDL_Cursor* m_cursor = nullptr;
			int m_active_window_id = -1;
			bool m_drop_sdl_responsability = false;
	};
}

#endif
