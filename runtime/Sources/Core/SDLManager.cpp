#include <PreCompiled.h>
#include <Core/SDLManager.h>
#include <Core/Memory.h>
#include <Embedded/IconMlx.h>
#include <Utils/Bits.h>

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

		m_drop_sdl_responsability = SDL_WasInit(SDL_INIT_VIDEO);
		if(m_drop_sdl_responsability) // is case the mlx is running in a sandbox like MacroUnitTester where SDL is already init
			return;
		SDL_SetMemoryFunctions(MemManager::Get().Malloc, MemManager::Get().Calloc, MemManager::Get().Realloc, MemManager::Get().Free);

		#ifdef FORCE_WAYLAND
			SDL_SetHint(SDL_HINT_VIDEODRIVER, "wayland,x11");
		#endif

		if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) != 0)
			FatalError("SDL: unable to init all subsystems; %", SDL_GetError());
		DebugLog("SDL Manager initialized");
	}

	Handle SDLManager::CreateWindow(const mlx_window_create_info* info, std::int32_t& id, bool hidden)
	{
		Internal::WindowInfos* infos = new Internal::WindowInfos;
		Verify(infos != nullptr, "SDL: window allocation failed");

		std::uint32_t flags = SDL_WINDOW_VULKAN;
		if(hidden)
			flags |= SDL_WINDOW_HIDDEN;
		else
			flags |= SDL_WINDOW_SHOWN;
		if(info->is_resizable)
			flags |= SDL_WINDOW_RESIZABLE;
		if(info->is_fullscreen)
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

		infos->window = SDL_CreateWindow(info->title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, info->width, info->height, flags);
		if(!infos->window)
			FatalError("SDL: unable to open a new window; %", SDL_GetError());
		infos->icon = SDL_CreateRGBSurfaceFrom(static_cast<void*>(logo_mlx), logo_mlx_width, logo_mlx_height, 32, 4 * logo_mlx_width, Rmask(), Gmask(), Bmask(), Amask());
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
			SDL_FreeSurface(infos->icon);

		m_windows_registry.erase(infos);
		delete infos;
	}

	VkSurfaceKHR SDLManager::CreateVulkanSurface(Handle window, VkInstance instance) const noexcept
	{
		VkSurfaceKHR surface;
		if(!SDL_Vulkan_CreateSurface(static_cast<Internal::WindowInfos*>(window)->window, instance, &surface))
			FatalError("SDL: could not create a Vulkan surface; %", SDL_GetError());
		return surface;
	}

	std::vector<const char*> SDLManager::GetRequiredVulkanInstanceExtentions(Handle window) const noexcept
	{
		std::uint32_t count;
		if(!SDL_Vulkan_GetInstanceExtensions(static_cast<Internal::WindowInfos*>(window)->window, &count, nullptr))
			FatalError("SDL Manager: could not retrieve Vulkan instance extensions");
		std::vector<const char*> extensions(count);
		if(!SDL_Vulkan_GetInstanceExtensions(static_cast<Internal::WindowInfos*>(window)->window, &count, extensions.data()))
			FatalError("SDL Manager: could not retrieve Vulkan instance extensions");
		extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
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

	void SDLManager::SetWindowSize(Handle window, int x, int y) const noexcept
	{
		SDL_SetWindowSize(static_cast<Internal::WindowInfos*>(window)->window, x, y);
	}

	void SDLManager::SetWindowTitle(Handle window, std::string_view title) const noexcept
	{
		SDL_SetWindowTitle(static_cast<Internal::WindowInfos*>(window)->window, title.data());
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

	void SDLManager::InputsFetcher(std::function<void(mlx_event_type, int, int)> functor)
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
