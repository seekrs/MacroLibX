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

		m_drop_sdl_responsability = SDL_WasInit(SDL_INIT_VIDEO);
		if(m_drop_sdl_responsability) // is case the mlx is running in a sandbox like MacroUnitTester where SDL is already init
			return;
		SDL_SetMemoryFunctions(MemManager::Get().Malloc, MemManager::Get().Calloc, MemManager::Get().Realloc, MemManager::Get().Free);

		#ifdef FORCE_WAYLAND
			SDL_SetHint(SDL_HINT_VIDEODRIVER, "wayland,x11");
		#endif

		if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) != 0)
			FatalError("SDL : unable to init all subsystems; %", SDL_GetError());
		DebugLog("SDL Manager initialized");
	}

	Handle SDLManager::CreateWindow(const std::string& title, std::size_t w, std::size_t h, bool hidden, std::int32_t& id)
	{
		Internal::WindowInfos* infos = new Internal::WindowInfos;
		Verify(infos != nullptr, "SDL : window allocation failed");

		infos->window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_VULKAN | (hidden ? SDL_WINDOW_HIDDEN : SDL_WINDOW_SHOWN));
		if(!infos->window)
			FatalError("SDL : unable to open a new window; %", SDL_GetError());
		infos->icon = SDL_CreateRGBSurfaceFrom(static_cast<void*>(logo_mlx), logo_mlx_width, logo_mlx_height, 32, 4 * logo_mlx_width, rmask, gmask, bmask, amask);
		SDL_SetWindowIcon(infos->window, infos->icon);

		m_windows_registry.insert(infos);

		id = SDL_GetWindowID(infos->window);

		return infos;
	}

	void SDLManager::DestroyWindow(Handle window) noexcept
	{
		Verify(m_windows_registry.find(window) != m_windows_registry.end(), "SDL : cannot destroy window; unknown window pointer");

		Internal::WindowInfos* infos = static_cast<Internal::WindowInfos*>(window);
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
		if(!SDL_Vulkan_CreateSurface(static_cast<Internal::WindowInfos*>(window)->window, instance, &surface))
			FatalError("SDL : could not create a Vulkan surface; %", SDL_GetError());
		return surface;
	}

	std::vector<const char*> SDLManager::GetRequiredVulkanInstanceExtentions() const noexcept
	{
		std::vector<const char*> extensions;
		extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);

		#ifdef VK_USE_PLATFORM_XCB_KHR
			extensions.push_back(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
		#endif

		#ifdef VK_USE_PLATFORM_XLIB_KHR
			extensions.push_back(VK_KHR_XLIB_SURFACE_EXTENSION_NAME);
		#endif

		#ifdef VK_USE_PLATFORM_WAYLAND_KHR
	//		extensions.push_back(VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME);
		#endif

		#ifdef VK_USE_PLATFORM_WIN32_KHR
			extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
		#endif

		#ifdef VK_USE_PLATFORM_METAL_EXT
			extensions.push_back(VK_EXT_METAL_SURFACE_EXTENSION_NAME);
		#endif
		return extensions;
	}

	Vec2ui SDLManager::GetVulkanDrawableSize(Handle window) const noexcept
	{
		Vec2i extent;
		SDL_Vulkan_GetDrawableSize(static_cast<Internal::WindowInfos*>(window)->window, &extent.x, &extent.y);
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

	void SDLManager::InputsFetcher(func::function<void(mlx_event_type, int, int)> functor)
	{
		SDL_Event event;
		while(SDL_PollEvent(&event))
		{
			std::uint32_t id = event.window.windowID;
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
						case SDL_WINDOWEVENT_FOCUS_GAINED: functor(MLX_WINDOW_EVENT, id, 5); break;
						case SDL_WINDOWEVENT_LEAVE: functor(MLX_WINDOW_EVENT, id, 6); break;
						case SDL_WINDOWEVENT_FOCUS_LOST: functor(MLX_WINDOW_EVENT, id, 7); break;

						default : break;
					}
					break;
				}

				default: break;
			}
		}
	}

	SDLManager::~SDLManager()
	{
		if(m_drop_sdl_responsability)
			return;
		SDL_QuitSubSystem(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS);
		SDL_Quit();
		s_instance = nullptr;
		DebugLog("SDL Manager uninitialized");
	}
}