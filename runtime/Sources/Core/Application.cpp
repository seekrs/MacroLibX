#include <PreCompiled.h>

#include <Core/Application.h>
#include <Renderer/Image.h>
#include <Renderer/RenderCore.h>
#include <Core/Memory.h>
#include <Core/EventBus.h>

namespace mlx
{
	Application::Application() : p_mem_manager(std::make_unique<MemManager>()), p_sdl_manager(std::make_unique<SDLManager>()), m_fps(), m_in() 
	{
		MLX_PROFILE_FUNCTION();
		std::srand(std::time(nullptr));
		EventBus::RegisterListener({ [](const EventBase& event)
		{
			if(event.What() == Event::FatalErrorEventCode)
				std::abort();
		}, "__MlxApplication" });

		#ifdef PROFILER
			p_profiler = std::make_unique<Profiler>();
		#endif

		m_fps.Init();
		p_render_core = std::make_unique<RenderCore>();
	}

	void Application::Run() noexcept
	{
		m_in.Run();

		while(m_in.IsRunning())
		{
			if(!m_fps.Update())
				continue;

			m_in.FetchInputs();

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
		try { texture = new Texture({}, w, h, VK_FORMAT_R8G8B8A8_SRGB, false, "mlx_user_image"); }
		catch(...) { return nullptr; }
		m_image_registry.RegisterTexture(texture);
		return texture;
	}

	void* Application::NewStbTexture(char* file, int* w, int* h)
	{
		MLX_PROFILE_FUNCTION();
		Texture* texture = StbTextureLoad(file, w, h);
		if(texture == nullptr)
			return nullptr;
		m_image_registry.RegisterTexture(texture);
		return texture;
	}

	void Application::DestroyTexture(void* ptr)
	{
		MLX_PROFILE_FUNCTION();
		RenderCore::Get().WaitDeviceIdle();
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
		for(auto& window : m_graphics)
		{
			if(window && window->GetWindow()->GetName() == "让我们在月光下做爱吧")
				window.reset();
		}

		p_render_core.reset();
		p_sdl_manager.reset();
		#ifdef PROFILER
			p_profiler.reset();
		#endif
		p_mem_manager.reset();
	}
}
