#include <PreCompiled.h>
#include <Core/SDLManager.h>
#include <Embedded/IconMlx.h>
#include <Utils/Bits.h>
#include <mlx_keycodes.h>

namespace mlx
{
	namespace Internal
	{
		struct WindowInfos
		{
			SDL_Window* window;
			SDL_Surface* icon;
		};
	}

	SDLManager* SDLManager::s_instance = nullptr;

	SDLManager::SDLManager()
	{
		MLX_PROFILE_FUNCTION();
		s_instance = this;

		m_drop_sdl_responsability = SDL_WasInit(SDL_INIT_VIDEO) || std::getenv("MLX_HEADLESS_MODE") != nullptr;
		if(m_drop_sdl_responsability) // is case the mlx is running in a sandbox like MacroUnitTester where SDL is already init
			return;
		//SDL_SetMemoryFunctions(MemManager::Get().Malloc, MemManager::Get().Calloc, MemManager::Get().Realloc, MemManager::Get().Free);

		#ifdef FORCE_WAYLAND
			SDL_SetHint(SDL_HINT_VIDEODRIVER, "wayland,x11");
		#endif

		//SDL_SetHintWithPriority(SDL_HINT_SHUTDOWN_DBUS_ON_QUIT, "1", SDL_HINT_OVERRIDE);

		if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER | SDL_INIT_TIMER | SDL_INIT_AUDIO) != 0)
			FatalError("SDL: unable to init all subsystems; %", SDL_GetError());

		SDL_AudioSpec	desired{
			.freq = 44100,
			.format = AUDIO_S16LSB,
			.channels = 2, .samples = 2048,
			.callback = SDLManager::AudioCallback,
		};
		m_audio_device = SDL_OpenAudioDevice(nullptr, 0, &desired, &m_audio_device_spec, 0);
		if (m_audio_device == 0)
			FatalError("SDL: unable to init audio device; %", SDL_GetError());
		SDL_PauseAudioDevice(m_audio_device, 0);

	    SDL_StartTextInput();

		DebugLog("SDL Manager initialized");
	}

	Handle SDLManager::CreateWindow(const mlx_window_create_info* info, std::int32_t& id, bool hidden)
	{
		Internal::WindowInfos* infos = new Internal::WindowInfos;
		Verify(infos != nullptr, "SDL: window allocation failed");

		const char* title = info->title ? info->title : "[Unnamed MLX Window]";

		std::uint32_t flags = 0;
		if(hidden)
			flags |= SDL_WINDOW_HIDDEN;
		else
			flags |= SDL_WINDOW_SHOWN;
		if(info->is_resizable)
			flags |= SDL_WINDOW_RESIZABLE;
		if(info->is_fullscreen)
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

		infos->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, info->width, info->height, flags);
		if(!infos->window)
			FatalError("SDL: unable to open a new window; %", SDL_GetError());

		mlx_color* buffer = new mlx_color[logo_mlx_size];
		std::memcpy(buffer, logo_mlx, logo_mlx_size);
		infos->icon = SDL_CreateRGBSurfaceFrom(buffer, logo_mlx_width, logo_mlx_height, 32, logo_mlx_width * 4, Rmask(), Gmask(), Bmask(), Amask());
		if(!infos->icon)
			FatalError("SDL: unable to create a window icon; %", SDL_GetError());
		SDL_SetWindowIcon(infos->window, infos->icon);

		m_windows_registry.insert(infos);

		id = SDL_GetWindowID(infos->window);

		return infos;
	}

	void SDLManager::DestroyWindow(Handle window) noexcept
	{
		Verify(m_windows_registry.find(window) != m_windows_registry.end(), "SDL: cannot destroy window; unknown window pointer");

		Internal::WindowInfos* infos = static_cast<Internal::WindowInfos*>(window);
		if(infos->window != nullptr)
			SDL_DestroyWindow(infos->window);
		if(infos->icon != nullptr)
		{
			delete[] reinterpret_cast<mlx_color*>(infos->icon->pixels);
			SDL_FreeSurface(infos->icon);
		}

		m_windows_registry.erase(infos);
		delete infos;
	}

	SDL_Window* SDLManager::GetRawWindow(Handle window) noexcept
	{
		Internal::WindowInfos* infos = static_cast<Internal::WindowInfos*>(window);
		return infos->window;
	}

	VkSurfaceKHR SDLManager::CreateVulkanSurface(Handle window, VkInstance instance) const noexcept
	{
		SDL_SysWMinfo info;
		SDL_VERSION(&info.version);
		if(SDL_GetWindowWMInfo(static_cast<Internal::WindowInfos*>(window)->window, &info) != SDL_TRUE)
			FatalError("SDL Manager: cannot retrieve window informations");

		switch(info.subsystem)
		{
			#ifdef SDL_VIDEO_DRIVER_WINDOWS
				case SDL_SYSWM_WINDOWS: return kvfCreateSurfaceKHR(instance, KVF_SURFACE_WINDOWS, static_cast<void*>(info.info.win.hinstance), static_cast<void*>(info.info.win.window));
			#endif
			#ifdef SDL_VIDEO_DRIVER_X11
				case SDL_SYSWM_X11: return kvfCreateSurfaceKHR(instance, KVF_SURFACE_XLIB, static_cast<void*>(info.info.x11.display), static_cast<void*>(&info.info.x11.window));
			#endif
			#ifdef SDL_VIDEO_DRIVER_COCOA
				case SDL_SYSWM_COCOA: FatalError("SDL Manager: Vulkan surfaces over Metal are not supported yet. Please fill an issue here https://github.com/seekrs/MacroLibX/issues/new");
			#endif
			#ifdef SDL_VIDEO_DRIVER_WAYLAND
				case SDL_SYSWM_WAYLAND: return kvfCreateSurfaceKHR(instance, KVF_SURFACE_WAYLAND, static_cast<void*>(info.info.wl.display), static_cast<void*>(info.info.wl.surface));
			#endif
			#ifdef SDL_VIDEO_DRIVER_ANDROID
				case SDL_SYSWM_ANDROID: return kvfCreateSurfaceKHR(instance, KVF_SURFACE_ANDROID, nullptr, static_cast<void*>(info.info.android.window));
			#endif

			default : FatalError("SDL Manager: unsupported windowing system"); break;
		}
		return VK_NULL_HANDLE;
	}

	Vec2ui SDLManager::GetVulkanDrawableSize(Handle window) const noexcept
	{
		Vec2i extent;
		SDL_GetWindowSize(static_cast<Internal::WindowInfos*>(window)->window, &extent.x, &extent.y);
		return Vec2ui{ extent };
	}

	void SDLManager::MoveMouseOnWindow(Handle window, int x, int y) const noexcept
	{
		SDL_WarpMouseInWindow(static_cast<Internal::WindowInfos*>(window)->window, x, y);
		SDL_PumpEvents();
	}

	void SDLManager::GetScreenSizeWindowIsOn(Handle window, int* x, int* y) const noexcept
	{
		SDL_DisplayMode DM;
		SDL_GetDesktopDisplayMode(SDL_GetWindowDisplayIndex(static_cast<Internal::WindowInfos*>(window)->window), &DM);
		*x = DM.w;
		*y = DM.h;
	}

	void SDLManager::SetWindowPosition(Handle window, int x, int y) const noexcept
	{
		SDL_SetWindowPosition(static_cast<Internal::WindowInfos*>(window)->window, x, y);
	}

	void SDLManager::SetWindowSize(Handle window, int x, int y) const noexcept
	{
		SDL_SetWindowSize(static_cast<Internal::WindowInfos*>(window)->window, x, y);
	}

	void SDLManager::SetWindowTitle(Handle window, std::string_view title) const noexcept
	{
		SDL_SetWindowTitle(static_cast<Internal::WindowInfos*>(window)->window, title.data());
	}

	void SDLManager::SetWindowIcon(Handle window, NonOwningPtr<Texture> texture) const noexcept
	{
		Internal::WindowInfos* infos = static_cast<Internal::WindowInfos*>(window);
		if(infos->icon != nullptr)
		{
			delete[] reinterpret_cast<mlx_color*>(infos->icon->pixels);
			SDL_FreeSurface(infos->icon);
		}

		int width = texture->GetWidth(), height = texture->GetHeight();

		infos->icon = SDL_CreateRGBSurfaceFrom(texture->GetBufferCopy(), width, height, 32, width * 4, Rmask(), Gmask(), Bmask(), Amask());
		SDL_SetWindowIcon(infos->window, infos->icon);
	}

	void SDLManager::SetWindowFullscreen(Handle window, bool enable) const noexcept
	{
		SDL_SetWindowFullscreen(static_cast<Internal::WindowInfos*>(window)->window, (enable ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0));
	}

	void SDLManager::SetWindowMaxSize(Handle window, int x, int y) const noexcept
	{
		SDL_SetWindowMaximumSize(static_cast<Internal::WindowInfos*>(window)->window, x, y);
	}

	void SDLManager::SetWindowMinSize(Handle window, int x, int y) const noexcept
	{
		SDL_SetWindowMinimumSize(static_cast<Internal::WindowInfos*>(window)->window, x, y);
	}

	void SDLManager::MaximizeWindow(Handle window) const noexcept
	{
		SDL_MaximizeWindow(static_cast<Internal::WindowInfos*>(window)->window);
	}

	void SDLManager::MinimizeWindow(Handle window) const noexcept
	{
		SDL_MinimizeWindow(static_cast<Internal::WindowInfos*>(window)->window);
	}

	void SDLManager::RestoreWindow(Handle window) const noexcept
	{
		SDL_RestoreWindow(static_cast<Internal::WindowInfos*>(window)->window);
	}

	void SDLManager::GetWindowPosition(Handle window, int* x, int* y) const noexcept
	{
		SDL_GetWindowPosition(static_cast<Internal::WindowInfos*>(window)->window, x, y);
	}

	void SDLManager::GetWindowSize(Handle window, int* x, int* y) const noexcept
	{
		SDL_GetWindowSize(static_cast<Internal::WindowInfos*>(window)->window, x, y);
	}

	void SDLManager::HideCursor() noexcept
	{
		SDL_ShowCursor(SDL_DISABLE);
	}

	void SDLManager::ShowCursor() noexcept
	{
		SDL_ShowCursor(SDL_ENABLE);
	}

	void SDLManager::SetCursorIcon(int icon) noexcept
	{
		if (icon < 0 || icon >= SDL_NUM_SYSTEM_CURSORS)
			return;
		SDL_Cursor* cursor = SDL_CreateSystemCursor(static_cast<SDL_SystemCursor>(icon));
		if (!cursor)
			return;

		if (m_cursor)
			SDL_FreeCursor(m_cursor);
		m_cursor = cursor;
		SDL_SetCursor(m_cursor);
	}

	std::int32_t SDLManager::GetX() const noexcept
	{
		int dummy;
		int x;
		SDL_GetMouseState(&x, &dummy);
		return x;
	}

	std::int32_t SDLManager::GetY() const noexcept
	{
		int dummy;
		int y;
		SDL_GetMouseState(&dummy, &y);
		return y;
	}

	std::int32_t SDLManager::GetXRel() const noexcept
	{
		int dummy;
		int x;
		SDL_GetRelativeMouseState(&x, &dummy);
		return x;
	}

	std::int32_t SDLManager::GetYRel() const noexcept
	{
		int dummy;
		int y;
		SDL_GetRelativeMouseState(&dummy, &y);
		return y;
	}

	int SDLManager::GetControllerIdFromSDL(int joystick_id) const noexcept
	{
		Controller controller = SDL_GameControllerFromInstanceID(joystick_id);

		if (!controller)
			return -1;

		auto match = std::find(m_controllers.begin(), m_controllers.end(), controller);
		if (match == m_controllers.end())
			return -1;
		return std::distance(m_controllers.begin(), match);
	}

	bool SDLManager::IsControllerConnected(int controller_id) const noexcept
	{
		if (controller_id < 0 || controller_id >= static_cast<int>(m_controllers.size()))
			return false;
		return (m_controllers[controller_id] != nullptr);
	}

	int SDLManager::GetFirstConnectedController() const noexcept
	{
		auto first = std::find_if(m_controllers.cbegin(), m_controllers.cend(),
			[](Controller ptr) { return (ptr != nullptr); });
		if (first == m_controllers.cend())
			return -1;
		return std::distance(m_controllers.cbegin(), first);
	}

	int SDLManager::AddController(int device_index) noexcept
	{
		Controller controller = SDL_GameControllerOpen(device_index);

		if (!controller)
		{
			Error("SDL: Unable to connect controller; %", SDL_GetError());
			return -1;
		}

		auto first_empty = std::find(m_controllers.begin(), m_controllers.end(), nullptr);
		if (first_empty == m_controllers.end())
		{
			m_controllers.push_back(controller);
			first_empty = m_controllers.end() - 1;
		}
		else
			*first_empty = controller;

		int controller_id = std::distance(m_controllers.begin(), first_empty);
		SDL_GameControllerSetPlayerIndex(controller, controller_id);

		DebugLog("SDL: Connected \"%\" to slot % %",
			SDL_GameControllerName(controller), controller_id,
			SDL_GameControllerHasRumble(controller)? "": "(No rumble)");
		return controller_id;
	}

	int SDLManager::RemoveController(int joystick_id) noexcept
	{
		int controller_id = GetControllerIdFromSDL(joystick_id);

		if (controller_id == -1)
		{
			Error("SDL: Tried to disconnect an invalid controller");
			return -1;
		}

		const char* name = SDL_GameControllerName(m_controllers[controller_id]);

		SDL_GameControllerClose(m_controllers[controller_id]);
		m_controllers[controller_id] = nullptr;

		DebugLog("SDL: Disconnected \"%\" at slot %", name, controller_id);
		return controller_id;
	}

	void SDLManager::RemoveAllControllers() noexcept
	{
		for (auto controller_it = m_controllers.begin(); controller_it != m_controllers.end(); ++controller_it)
		{
			if (*controller_it == nullptr)
				continue;

			const char* name = SDL_GameControllerName(*controller_it);
			SDL_GameControllerClose(*controller_it);
			*controller_it = nullptr;

			int controller_id = std::distance(m_controllers.begin(), controller_it);
			DebugLog("SDL: Disconnected \"%\" at slot %", name, controller_id);
		}
	}

	float SDLManager::GetControllerAxis(int controller_id, int axis) const noexcept
	{
		if (!IsControllerConnected(controller_id))
			return 0.0;

		float value = SDL_GameControllerGetAxis(m_controllers[controller_id], (SDL_GameControllerAxis)axis);
		return std::clamp<float>(value / SDL_JOYSTICK_AXIS_MAX, -1.0, 1.0);
	}

	void SDLManager::RumbleController(int controller_id, float low_freq, float high_freq, float duration) const noexcept
	{
		if (!IsControllerConnected(controller_id))
			return;

		SDL_GameControllerRumble(m_controllers[controller_id],
			low_freq * 0xFFFF, high_freq * 0xFFFF, duration * 1000);
	}

	void SDLManager::SetInputBinding(std::function<void(SDL_Event*)> functor)
	{
		m_binding_hook = std::move(functor);
	}

	void SDLManager::SendInactiveEvents(std::function<void(mlx_event_type, int, int)> functor)
	{
		while (!m_inactive_events.empty())
		{
			EventRequest request = m_inactive_events.back();
			functor(request.type, m_active_window_id, request.code);
			m_inactive_events.pop_back();
		}
	}

	void SDLManager::HandleTextInputEvent(std::function<void(mlx_event_type, int, int)> functor, SDL_Event event)
	{
		const int id = event.window.windowID;
		std::string str = event.text.text;

		for (auto it = str.cbegin(); it < str.cend(); ++it)
			functor(MLX_TEXTINPUT, id, static_cast<unsigned char>(*it));
		if (str.length() < SDL_TEXTINPUTEVENT_TEXT_SIZE)
			functor(MLX_TEXTINPUT, id, static_cast<unsigned char>('\0'));
	}

	#define CONTROLLER_CODE(id, code) ((id << (sizeof(short) * 8)) | code)

	void SDLManager::HandleControllerDeviceEvent(std::function<void(mlx_event_type, int, int)> functor, SDL_Event event)
	{
		int code;

		if (event.type == SDL_CONTROLLERDEVICEADDED)
		{
			int controller_id = AddController(event.cbutton.which);
			code = CONTROLLER_CODE(controller_id, MLX_CONTROLLER_CONNECT);
		}
		else
		{
			int controller_id = RemoveController(event.cbutton.which);
			code = CONTROLLER_CODE(controller_id, MLX_CONTROLLER_DISCONNECT);
		}

		if (m_active_window_id == -1)
			m_inactive_events.push_back(EventRequest(MLX_CONTROLLERDOWN, code));
		else
			functor(MLX_CONTROLLERDOWN, m_active_window_id, code);
	}

	void SDLManager::InputsFetcher(std::function<void(mlx_event_type, int, int)> functor)
	{
		SDL_Event event;
		while(SDL_PollEvent(&event))
		{
			int id = event.window.windowID;
			switch(event.type)
			{
				case SDL_KEYUP: functor(MLX_KEYUP, id, event.key.keysym.scancode); break;
				case SDL_KEYDOWN: functor(MLX_KEYDOWN, id, event.key.keysym.scancode); break;
				case SDL_MOUSEBUTTONUP: functor(MLX_MOUSEUP, id, event.button.button); break;
				case SDL_MOUSEBUTTONDOWN: functor(MLX_MOUSEDOWN, id, event.button.button); break;
				case SDL_MOUSEWHEEL:
				{
					if(event.wheel.y > 0) // scroll up
						functor(MLX_MOUSEWHEEL, id, 1);
					else if(event.wheel.y < 0) // scroll down
						functor(MLX_MOUSEWHEEL, id, 2);
					if(event.wheel.x > 0) // scroll right
						functor(MLX_MOUSEWHEEL, id, 3);
					else if(event.wheel.x < 0) // scroll left
						functor(MLX_MOUSEWHEEL, id, 4);
					break;
				}
				case SDL_WINDOWEVENT:
				{
					switch(event.window.event)
					{
						case SDL_WINDOWEVENT_CLOSE: functor(MLX_WINDOW_EVENT, id, 0); break;
						case SDL_WINDOWEVENT_MOVED: functor(MLX_WINDOW_EVENT, id, 1); break;
						case SDL_WINDOWEVENT_MINIMIZED: functor(MLX_WINDOW_EVENT, id, 2); break;
						case SDL_WINDOWEVENT_MAXIMIZED: functor(MLX_WINDOW_EVENT, id, 3); break;
						case SDL_WINDOWEVENT_ENTER: functor(MLX_WINDOW_EVENT, id, 4); break;
						case SDL_WINDOWEVENT_LEAVE: functor(MLX_WINDOW_EVENT, id, 6); break;
						case SDL_WINDOWEVENT_SIZE_CHANGED: functor(MLX_WINDOW_EVENT, id, 8); break;
						case SDL_WINDOWEVENT_RESIZED: functor(MLX_WINDOW_EVENT, id, 9); break;
						case SDL_WINDOWEVENT_RESTORED: functor(MLX_WINDOW_EVENT, id, 11); break;

						case SDL_WINDOWEVENT_FOCUS_GAINED:
						{
							m_active_window_id = id;
							functor(MLX_WINDOW_EVENT, id, 5);
							SendInactiveEvents(functor);
							break;
						}
						case SDL_WINDOWEVENT_FOCUS_LOST:
						{
							if (m_active_window_id == id)
								m_active_window_id = -1;
							functor(MLX_WINDOW_EVENT, id, 7);
							break;
						}

						default : break;
					}
					break;
				}

				case SDL_TEXTINPUT: HandleTextInputEvent(functor, event); break;

				case SDL_CONTROLLERBUTTONUP: functor(MLX_CONTROLLERUP, m_active_window_id, CONTROLLER_CODE(GetControllerIdFromSDL(event.cbutton.which), event.cbutton.button)); break;
				case SDL_CONTROLLERBUTTONDOWN: functor(MLX_CONTROLLERDOWN, m_active_window_id, CONTROLLER_CODE(GetControllerIdFromSDL(event.cbutton.which), event.cbutton.button)); break;

				case SDL_CONTROLLERDEVICEADDED:
				case SDL_CONTROLLERDEVICEREMOVED:
					HandleControllerDeviceEvent(functor, event); break;

				default: break;
			}

			if(m_binding_hook)
				m_binding_hook(&event);
		}
	}

	#undef CONTROLLER_CODE

	#define BYTES_PER_FRAME(spec)	(SDL_AUDIO_BITSIZE(spec.format) / 8 * spec.channels)

	Sound* SDLManager::CreateSoundFromWAV(const char* path, float* duration) noexcept
	{
		SDL_AudioSpec	wav_spec;
		uint8_t*		wav_buffer = nullptr;
		uint32_t		wav_length = 0;

		if (!SDL_LoadWAV(path, &wav_spec, &wav_buffer, &wav_length)) {
			Error("SDL: unable to create a new sound; %", SDL_GetError());
			return nullptr;
		}

		SDL_AudioCVT cvt;
		int result = SDL_BuildAudioCVT(&cvt, wav_spec.format, wav_spec.channels, wav_spec.freq,
			m_audio_device_spec.format, m_audio_device_spec.channels, m_audio_device_spec.freq);
		if (result < 0) {
			SDL_FreeWAV(wav_buffer);
			Error("SDL: unable to create a new sound; %", SDL_GetError());
			return nullptr;
		}

		auto data = std::make_shared<std::vector<uint8_t>>();
		if (result == 0) {
			data->assign(wav_buffer, wav_buffer + wav_length);
		} else {
			std::vector<uint8_t> cvt_buffer(wav_length * cvt.len_mult);
			std::memcpy(cvt_buffer.data(), wav_buffer, wav_length);
			cvt.buf = cvt_buffer.data();
			cvt.len = wav_length;

			if (SDL_ConvertAudio(&cvt) != 0) {
				SDL_FreeWAV(wav_buffer);
				Error("SDL: unable to create a new sound; %", SDL_GetError());
				return nullptr;
			}
			cvt_buffer.resize(static_cast<size_t>(cvt.len_cvt));
			*data = std::move(cvt_buffer);
		}
		SDL_FreeWAV(wav_buffer);

		uint32_t frame_count = data->size() / BYTES_PER_FRAME(m_audio_device_spec);
		if (duration)
			*duration = static_cast<float>(frame_count) / m_audio_device_spec.freq;
		Sound* sound = new Sound(std::move(data), frame_count);
		m_sounds.insert(sound);
		return sound;
	}

	void SDLManager::DestroySound(Sound* sound) noexcept
	{
		if (!SoundExists(sound)) {
			Error("trying to destroy an unregistered sound");
			return;
		}
		m_sounds.erase(sound);
	}

	AudioChannel* SDLManager::CreateAudioChannel() noexcept
	{
		AudioChannel* channel = new AudioChannel;
		AudioDeviceLock lock;

		m_audio_channels.insert(channel);
		return channel;
	}

	void SDLManager::DestroyAudioChannel(AudioChannel* channel) noexcept
	{
		if (!AudioChannelExists(channel)) {
			Error("trying to destroy an unregistered audio channel");
			return;
		}
		AudioDeviceLock lock;
		m_audio_channels.erase(channel);
	}

	void SDLManager::AudioCallback(void* /*userdata*/, Uint8* stream, int len) noexcept
	{
		SDL_AudioSpec spec = s_instance->m_audio_device_spec;
		int out_channels = spec.channels;
		int out_frames = len / BYTES_PER_FRAME(spec);
		int total_samples = out_frames * out_channels;

		static thread_local std::vector<int32_t> accum;
		accum.assign(total_samples, 0);

		for (AudioChannel* ch : s_instance->m_audio_channels)
			ch->MixInto(accum.data(), out_frames, out_channels);

		int16_t* out = reinterpret_cast<int16_t*>(stream);
		for (int i = 0; i < total_samples; ++i)
			out[i] = static_cast<int16_t>(std::clamp(accum[i], -32768, 32767));
	}

	void SDLManager::CheckAudioAllocs() const noexcept
	{
		size_t	audio_allocs = m_sounds.size() + m_audio_channels.size();

		if (audio_allocs > 0)
			Error("Audio Manager: some user-dependant allocations were not freed before ending the application (% active allocations). You may have not destroyed all the MLX resources you've created", audio_allocs);
	}

	SDLManager::~SDLManager()
	{
		if(m_drop_sdl_responsability)
			return;

		RemoveAllControllers();
		CheckAudioAllocs();
		SDL_CloseAudioDevice(m_audio_device);
		SDL_StopTextInput();
		if (m_cursor)
			SDL_FreeCursor(m_cursor);
		SDL_QuitSubSystem(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER | SDL_INIT_AUDIO);
		SDL_Quit();
		s_instance = nullptr;
		DebugLog("SDL Manager uninitialized");
	}
}
