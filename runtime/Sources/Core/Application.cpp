/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Application.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 22:10:52 by maldavid          #+#    #+#             */
/*   Updated: 2024/05/25 16:06:57 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <PreCompiled.h>

#include <Core/Application.h>
#include <Renderer/Texts/TextLibrary.h>
#include <Renderer/Texts/FontLibrary.h>
#include <Renderer/Images/Texture.h>
#include <Renderer/Core/RenderCore.h>
#include <Core/Memory.h>
#include <Core/EventBus.h>
#include <Core/SDLManager.h>

namespace mlx
{
	Application::Application() : m_fps(), m_in() 
	{
		EventBus::RegisterListener({[](const EventBase& event)
		{
		}, "__internal_application" });

		m_fps.init();
		SDLManager::Get().Init();
	}

	void Application::Run() noexcept
	{
		m_in.Run();

		while(m_in.IsRunning())
		{
			if(!m_fps.Update())
				continue;
			m_in.Update();

			if(f_loop_hook)
				f_loop_hook(p_param);

			for(auto& gs : m_graphics)
			{
				if(gs)
					gs->Render();
			}
		}

		RenderCore::Get().GetSingleTimeCmdManager().UpdateSingleTimesCmdBuffersSubmitState();

		for(auto& gs : m_graphics)
		{
			if(!gs)
				continue;
			for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
				gs->GetRenderer().GetCmdBuffer(i).WaitForExecution();
		}
	}

	void* Application::NewTexture(int w, int h)
	{
		MLX_PROFILE_FUNCTION();
		Texture* texture = new Texture;
		#ifdef DEBUG
			texture->Create(nullptr, w, h, VK_FORMAT_R8G8B8A8_UNORM, "__mlx_unamed_user_texture");
		#else
			texture->Create(nullptr, w, h, VK_FORMAT_R8G8B8A8_UNORM, nullptr);
		#endif
		m_image_registry.RegisterTexture(texture);
		return texture;
	}

	void* Application::NewStbTexture(char* file, int* w, int* h)
	{
		MLX_PROFILE_FUNCTION();
		Texture* texture = StbTextureLoad(file, w, h);
		m_image_registry.RegisterTexture(texture);
		return texture;
	}

	void Application::DestroyTexture(void* ptr)
	{
		MLX_PROFILE_FUNCTION();
		vkDeviceWaitIdle(RenderCore::Get().GetDevice().Get()); // TODO : synchronize with another method than waiting for GPU to be idle
		if(!m_image_registry.Find(ptr))
		{
			Error("invalid image ptr");
			return;
		}

		Texture* texture = static_cast<Texture*>(ptr);
		if(!texture->IsInit())
			Error("trying to destroy a texture that has already been destroyed");
		else
			texture->Destroy();
		for(auto& gs : _graphics)
		{
			if(gs)
				gs->TryEraseTextureFromManager(texture);
		}
		m_image_registry.UnregisterTexture(texture);
		delete texture;
	}

	Application::~Application()
	{
		TextLibrary::Get().ClearLibrary();
		FontLibrary::Get().ClearLibrary();
		SDLManager::Get().Shutdown();
	}
}
