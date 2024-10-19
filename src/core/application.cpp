/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   application.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 22:10:52 by maldavid          #+#    #+#             */
/*   Updated: 2024/10/19 10:49:21 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "application.h"
#include <renderer/texts/text_library.h>
#include <renderer/texts/font_library.h>
#include <SDL2/SDL.h>
#include <renderer/images/texture.h>
#include <renderer/core/render_core.h>
#include <core/errors.h>
#include <mlx_profile.h>
#include <core/memory.h>

namespace mlx::core
{
	static bool __drop_sdl_responsability = false;
	Application::Application() : _fps(), _in(std::make_unique<Input>()) 
	{
		_fps.init();
		__drop_sdl_responsability = SDL_WasInit(SDL_INIT_VIDEO);
		if(__drop_sdl_responsability) // is case the mlx is running in a sandbox like MacroUnitTester where SDL is already init
			return;
		SDL_SetMemoryFunctions(MemManager::malloc, MemManager::calloc, MemManager::realloc, MemManager::free);

		/* Remove this comment if you want to prioritise Wayland over X11/XWayland, at your own risks */
		//SDL_SetHint(SDL_HINT_VIDEODRIVER, "wayland,x11");

		if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) != 0)
			error::report(e_kind::fatal_error, "SDL error : unable to init all subsystems : %s", SDL_GetError());
	}

	void Application::run() noexcept
	{
		_in->run();
		while(_in->isRunning())
		{
			if(!_fps.update())
				continue;
			_in->update();

			if(_loop_hook)
				_loop_hook(_param);

			for(auto& gs : _graphics)
			{
				if(gs)
					gs->render();
			}
		}

		Render_Core::get().getSingleTimeCmdManager().updateSingleTimesCmdBuffersSubmitState();

		for(auto& gs : _graphics)
		{
			if(!gs)
				continue;
			for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
				gs->getRenderer().getCmdBuffer(i).waitForExecution();
		}
	}

	void* Application::newTexture(int w, int h)
	{
		MLX_PROFILE_FUNCTION();
		#ifdef DEBUG
			_textures.emplace_front().create(nullptr, w, h, VK_FORMAT_R8G8B8A8_UNORM, "__mlx_unamed_user_texture");
		#else
			_textures.emplace_front().create(nullptr, w, h, VK_FORMAT_R8G8B8A8_UNORM, nullptr);
		#endif
		return &_textures.front();
	}

	void* Application::newStbTexture(char* file, int* w, int* h)
	{
		MLX_PROFILE_FUNCTION();
		_textures.emplace_front(stbTextureLoad(file, w, h));
		return &_textures.front();
	}

	void Application::destroyTexture(void* ptr)
	{
		MLX_PROFILE_FUNCTION();
		vkDeviceWaitIdle(Render_Core::get().getDevice().get()); // TODO : synchronize with another method than waiting for GPU to be idle
		if(ptr == nullptr)
		{
			core::error::report(e_kind::error, "invalid image ptr (NULL)");
			return;
		}

		auto it = std::find_if(_textures.begin(), _textures.end(), [=](const Texture& texture) { return &texture == ptr; });
		if(it == _textures.end())
		{
			core::error::report(e_kind::error, "invalid image ptr");
			return;
		}
		Texture* texture = static_cast<Texture*>(ptr);
		if(!texture->isInit())
			core::error::report(e_kind::error, "trying to destroy a texture that has already been destroyed");
		else
			texture->destroy();
		for(auto& gs : _graphics)
		{
			if(gs)
				gs->tryEraseTextureFromManager(texture);
		}
		_textures.erase(it);
	}

	Application::~Application()
	{
		TextLibrary::get().clearLibrary();
		TextLibrary::get().reset();
		FontLibrary::get().clearLibrary();
		FontLibrary::get().reset();
		if(__drop_sdl_responsability)
			return;
		SDL_QuitSubSystem(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS);
		SDL_Quit();
	}
}
