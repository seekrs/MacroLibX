#include <PreCompiled.h>

#include <Core/Application.h>
#include <Renderer/Image.h>
#include <Renderer/RenderCore.h>
#include <Core/Memory.h>
#include <Core/EventBus.h>

namespace mlx
{
	Application::Application() : m_render_core(), m_fps(), m_in() 
	{
		EventBus::RegisterListener({[](const EventBase& event)
		{
			if(event.What() == Event::FatalErrorEventCode)
				std::abort();
		}, "__MlxApplication" });

		m_fps.Init();
		SDLManager::Get().Init();
	}

	void Application::Run() noexcept
	{
		m_in.Run();

		while(m_in.IsRunning())
		{
			if(!m_fps.Update())
				continue;

			if(f_loop_hook)
				f_loop_hook(p_param);

			for(auto& gs : m_graphics)
			{
				if(gs)
					gs->Render();
			}
		}
		RenderCore::Get().WaitDeviceIdle();
	}

	void* Application::NewTexture(int w, int h)
	{
		MLX_PROFILE_FUNCTION();
		Texture* texture;
		try { texture = new Texture({}, w, h); }
		catch(...) { return NULL; }
		m_image_registry.RegisterTexture(texture);
		return texture;
	}

	void* Application::NewStbTexture(char* file, int* w, int* h)
	{
		MLX_PROFILE_FUNCTION();
		Texture* texture = StbTextureLoad(file, w, h);
		if(texture == nullptr)
			return NULL; // NULL for C compatibility
		m_image_registry.RegisterTexture(texture);
		return texture;
	}

	void Application::DestroyTexture(void* ptr)
	{
		MLX_PROFILE_FUNCTION();
		RenderCore::Get().WaitDeviceIdle(); // TODO : synchronize with another method than waiting for GPU to be idle
		if(!m_image_registry.IsTextureKnown(static_cast<Texture*>(ptr)))
		{
			Error("invalid image ptr");
			return;
		}

		Texture* texture = static_cast<Texture*>(ptr);
		if(!texture->IsInit())
			Error("trying to destroy a texture that has already been destroyed");
		else
			texture->Destroy();
		for(auto& gs : m_graphics)
		{
			if(gs)
				gs->TryEraseSpritesInScene(texture);
		}
		m_image_registry.UnregisterTexture(texture);
		delete texture;
	}

	Application::~Application()
	{
		SDLManager::Get().Shutdown();
	}
}
