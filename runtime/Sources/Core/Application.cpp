/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Application.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 22:10:52 by maldavid          #+#    #+#             */
/*   Updated: 2024/04/02 17:06:34 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <PreCompiled.h>

#include <Core/Application.h>
#include <renderer/texts/text_library.h>
#include <renderer/texts/font_library.h>
#include <renderer/images/texture.h>
#include <renderer/core/render_core.h>
#include <Core/memory.h>
#include <Core/EventBus.h>

namespace mlx::core
{
	Application::Application() : _fps(), _in(std::make_unique<Input>()) 
	{
		EventBus::RegisterListener({[](const EventBase& event)
		{

		}, "__internal_application" });

		_fps.init();
	}

	void Application::run() noexcept
	{
		while(_in->isRunning())
		{
			if(!_fps.update())
				continue;
			_in->update();

			if(_loop_hook)
				_loop_hook(_param);

			for(auto& gs : _graphics)
				gs->render();
		}

		Render_Core::get().getSingleTimeCmdManager().updateSingleTimesCmdBuffersSubmitState();

		for(auto& gs : _graphics)
		{
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
			gs->tryEraseTextureFromManager(texture);
		_textures.erase(it);
	}

	Application::~Application()
	{
		TextLibrary::get().clearLibrary();
		FontLibrary::get().clearLibrary();
	}
}
