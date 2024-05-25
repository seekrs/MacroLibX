/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SDLManager.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 15:44:03 by maldavid          #+#    #+#             */
/*   Updated: 2024/05/25 16:46:48 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <PreCompiled.h>
#include <Core/SDLManager.h>
#include <Core/Memory.h>
#include <Utils/IconMlx.h>

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
		SDL_SetMemoryFunctions(MemManager::malloc, MemManager::calloc, MemManager::realloc, MemManager::free);

		#ifdef FORCE_WAYLAND
			SDL_SetHint(SDL_HINT_VIDEODRIVER, "wayland,x11");
		#endif

		if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) != 0)
			FatalError("SDL : unable to init all subsystems; %", SDL_GetError());
	}

	void* SDLManager::CreateWindow(const std::string& title, std::size_t w, std::size_t h)
	{
		details::WindowInfos* infos = new details::WindowInfos;
		Verify(infos != nullptr, "SDL : window allocation failed");

		infos->window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_VULKAN | SDL_WINDOW_SHOWN);
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

	void SDLManager::Shutdown() noexcept
	{
		if(m_drop_sdl_responsability)
			return;
		SDL_QuitSubSystem(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS);
		SDL_Quit();
	}
}
