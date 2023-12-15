/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   application.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 22:10:52 by maldavid          #+#    #+#             */
/*   Updated: 2023/12/15 20:51:41 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "application.h"
#include <renderer/images/texture.h>
#include <renderer/core/render_core.h>
#include <array>
#include <core/errors.h>
#include <mlx_profile.h>
#include <core/memory.h>

namespace mlx::core
{
	Application::Application() : _in(std::make_unique<Input>())
	{
		SDL_SetMemoryFunctions(MemManager::malloc, MemManager::calloc, MemManager::realloc, MemManager::free);
		if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) != 0)
			error::report(e_kind::fatal_error, "SDL error : unable to init all subsystems : %s", SDL_GetError());
	}

	void Application::run() noexcept
	{
		while(_in->is_running())
		{
			_in->update();
			for(auto& gs : _graphics)
				gs->beginRender();

			if(_loop_hook)
				_loop_hook(_param);

			for(auto& gs : _graphics)
				gs->endRender();
		}
	}

	void* Application::newTexture(int w, int h)
	{
		#ifdef DEBUG
			_textures.emplace_front().create(nullptr, w, h, VK_FORMAT_R8G8B8A8_UNORM, "__mlx_unamed_user_texture");
		#else
			_textures.emplace_front().create(nullptr, w, h, VK_FORMAT_R8G8B8A8_UNORM, nullptr);
		#endif
		return &_textures.front();
	}

	void* Application::newStbTexture(char* file, int* w, int* h)
	{
		_textures.emplace_front(stbTextureLoad(file, w, h));
		return &_textures.front();
	}

	void Application::destroyTexture(void* ptr)
	{
		vkDeviceWaitIdle(Render_Core::get().getDevice().get()); // TODO : synchronize with another method than stopping all the GPU porcess
		Texture* texture = static_cast<Texture*>(ptr);
		texture->destroy();
	}

	Application::~Application()
	{
		SDL_QuitSubSystem(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS);
		SDL_Quit();
	}
}
