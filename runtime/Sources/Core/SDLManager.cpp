#include <PreCompiled.h>
#include <Core/SDLManager.h>
#include <Core/Memory.h>
#include <Embedded/IconMlx.h>

namespace mlx
{
	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		constexpr const std::uint32_t rmask = 0xff000000;
		constexpr const std::uint32_t gmask = 0x00ff0000;
		constexpr const std::uint32_t bmask = 0x0000ff00;
		constexpr const std::uint32_t amask = 0x000000ff;
	#else
		constexpr const std::uint32_t rmask = 0x000000ff;
		constexpr const std::uint32_t gmask = 0x0000ff00;
		constexpr const std::uint32_t bmask = 0x00ff0000;
		constexpr const std::uint32_t amask = 0xff000000;
	#endif

	namespace details
	{
		struct WindowInfos
		{
			SDL_Window* window;
			SDL_Surface* icon;
		};
	}

	void SDLManager::Init() noexcept
	{
		MLX_PROFILE_FUNCTION();
		m_drop_sdl_responsability = SDL_WasInit(SDL_INIT_VIDEO);
		if(m_drop_sdl_responsability) // is case the mlx is running in a sandbox like MacroUnitTester where SDL is already init
			return;
		SDL_SetMemoryFunctions(MemManager::Get().Malloc, MemManager::Get().Calloc, MemManager::Get().Realloc, MemManager::Get().Free);

		#ifdef FORCE_WAYLAND
			SDL_SetHint(SDL_HINT_VIDEODRIVER, "wayland,x11");
		#endif

		if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) != 0)
			FatalError("SDL : unable to init all subsystems; %", SDL_GetError());

		struct WatcherData
		{
			func::function<void(mlx_event_type, int, int, void*)> callback;
			NonOwningPtr<SDLManager> manager;
			void* userdata;
		};

		WatcherData watcher_data;
		watcher_data.callback = f_callback;
		watcher_data.userdata = p_callback_data;

		SDL_AddEventWatch([](void* userdata, SDL_Event* event) -> int
		{
			WatcherData* data = static_cast<WatcherData*>(userdata);

			std::uint32_t id = event->window.windowID;
			switch(event->type) 
			{
				case SDL_KEYUP: data->callback(MLX_KEYUP, id, event->key.keysym.scancode, data->userdata); break;
				case SDL_KEYDOWN: data->callback(MLX_KEYDOWN, id, event->key.keysym.scancode, data->userdata); break;
				case SDL_MOUSEBUTTONUP: data->callback(MLX_MOUSEUP, id, event->button.button, data->userdata); break;
				case SDL_MOUSEBUTTONDOWN: data->callback(MLX_MOUSEDOWN, id, event->button.button, data->userdata); break;
				case SDL_MOUSEWHEEL:
				{
					if(event->wheel.y > 0) // scroll up
						data->callback(MLX_MOUSEWHEEL, id, 1, data->userdata);
					else if(event->wheel.y < 0) // scroll down
						data->callback(MLX_MOUSEWHEEL, id, 2, data->userdata);
					if(event->wheel.x > 0) // scroll right
						data->callback(MLX_MOUSEWHEEL, id, 3, data->userdata);
					else if(event->wheel.x < 0) // scroll left
						data->callback(MLX_MOUSEWHEEL, id, 4, data->userdata);
					break;
				}
				case SDL_WINDOWEVENT:
				{
					switch(event->window.event)
					{
						case SDL_WINDOWEVENT_CLOSE: data->callback(MLX_WINDOW_EVENT, id, 0, data->userdata); break;
						case SDL_WINDOWEVENT_MOVED: data->callback(MLX_WINDOW_EVENT, id, 1, data->userdata); break;
						case SDL_WINDOWEVENT_MINIMIZED: data->callback(MLX_WINDOW_EVENT, id, 2, data->userdata); break;
						case SDL_WINDOWEVENT_MAXIMIZED: data->callback(MLX_WINDOW_EVENT, id, 3, data->userdata); break;
						case SDL_WINDOWEVENT_ENTER: data->callback(MLX_WINDOW_EVENT, id, 4, data->userdata); break;
						case SDL_WINDOWEVENT_FOCUS_GAINED: data->callback(MLX_WINDOW_EVENT, id, 5, data->userdata); break;
						case SDL_WINDOWEVENT_LEAVE: data->callback(MLX_WINDOW_EVENT, id, 6, data->userdata); break;
						case SDL_WINDOWEVENT_FOCUS_LOST: data->callback(MLX_WINDOW_EVENT, id, 7, data->userdata); break;

						default : break;
					}
					break;
				}

				default: break;
			}

			return 0;
		}, &watcher_data);
		DebugLog("SDL Manager initialized");
	}

	void* SDLManager::CreateWindow(const std::string& title, std::size_t w, std::size_t h, bool hidden)
	{
		details::WindowInfos* infos = new details::WindowInfos;
		Verify(infos != nullptr, "SDL : window allocation failed");

		infos->window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_VULKAN | (hidden ? SDL_WINDOW_HIDDEN : SDL_WINDOW_SHOWN));
		if(!infos->window)
			FatalError("SDL : unable to open a new window; %", SDL_GetError());
		infos->icon = SDL_CreateRGBSurfaceFrom(static_cast<void*>(logo_mlx), logo_mlx_width, logo_mlx_height, 32, 4 * logo_mlx_width, rmask, gmask, bmask, amask);
		SDL_SetWindowIcon(infos->window, infos->icon);

		m_windows_registry.insert(infos);

		return infos;
	}

	void SDLManager::DestroyWindow(void* window) noexcept
	{
		Verify(m_windows_registry.find(window) != m_windows_registry.end(), "SDL : cannot destroy window; unknown window pointer");

		details::WindowInfos* infos = static_cast<details::WindowInfos*>(window);
		if(infos->window != nullptr)
			SDL_DestroyWindow(infos->window);
		if(infos->icon != nullptr)
			SDL_FreeSurface(infos->icon);

		m_windows_registry.erase(infos);
		delete infos;
	}

	VkSurfaceKHR SDLManager::CreateVulkanSurface(Handle window, VkInstance instance) const noexcept
	{
		VkSurfaceKHR surface;
		if(!SDL_Vulkan_CreateSurface(static_cast<SDL_Window*>(window), instance, &surface))
			FatalError("SDL : could not create a Vulkan surface; %", SDL_GetError());
		return surface;
	}

	std::vector<const char*> SDLManager::GetRequiredVulkanInstanceExtentions(Handle window) const noexcept
	{
		std::uint32_t count;
		if(!SDL_Vulkan_GetInstanceExtensions(static_cast<SDL_Window*>(window), &count, nullptr))
			FatalError("Vulkan : cannot get instance extentions from window : %",  SDL_GetError());

		std::vector<const char*> extensions(count);

		if(!SDL_Vulkan_GetInstanceExtensions(static_cast<SDL_Window*>(window), &count, extensions.data()))
			FatalError("Vulkan : cannot get instance extentions from window : %", SDL_GetError());
		return extensions;
	}

	Vec2ui SDLManager::GetVulkanDrawableSize(Handle window) const noexcept
	{
		Vec2i extent;
		SDL_Vulkan_GetDrawableSize(static_cast<SDL_Window*>(window), &extent.x, &extent.y);
		return Vec2ui{ extent };
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

	void SDLManager::Shutdown() noexcept
	{
		if(m_drop_sdl_responsability)
			return;
		SDL_QuitSubSystem(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS);
		SDL_Quit();
		DebugLog("SDL Manager uninitialized");
	}
}
