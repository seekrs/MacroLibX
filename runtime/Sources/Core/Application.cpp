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
		LoadFont("default", 6.0f);
	}

	void Application::Run() noexcept
	{
		m_in.Run();

		while(m_in.IsRunning())
		{
			if(!m_fps.Update())
				continue;

			m_in.FetchInputs();

			for(const auto& hook : m_hooks)
			{
				if(hook.fn)
					hook.fn(hook.param);
			}

			for(auto& gs : m_graphics)
			{
				if(gs)
					gs->Render();
			}
		}
		RenderCore::Get().WaitDeviceIdle();
	}

	mlx_image Application::NewTexture(int w, int h)
	{
		MLX_PROFILE_FUNCTION();

		mlx_image image;
		try { image = new mlx_image_handler; }
		catch(...) { return nullptr; }

		Texture* texture;
		try { texture = new Texture({}, w, h, VK_FORMAT_R8G8B8A8_SRGB, false, "mlx_user_image"); }
		catch(...) { return nullptr; }
		m_image_registry.RegisterTexture(texture);
		image->texture = texture;
		texture->Clear(VK_NULL_HANDLE, Vec4f{ 0.0f });
		return image;
	}

	mlx_image Application::NewStbTexture(char* file, int* w, int* h)
	{
		MLX_PROFILE_FUNCTION();

		mlx_image image;
		try { image = new mlx_image_handler; }
		catch(...) { return nullptr; }

		Texture* texture = StbTextureLoad(file, w, h);
		if(texture == nullptr)
			return nullptr;
		m_image_registry.RegisterTexture(texture);
		image->texture = texture;
		return image;
	}

	void Application::DestroyTexture(mlx_image image)
	{
		MLX_PROFILE_FUNCTION();
		RenderCore::Get().WaitDeviceIdle();
		if(!m_image_registry.IsTextureKnown(image->texture))
		{
			Error("invalid image handle");
			return;
		}

		Texture* texture = image->texture.Get();
		if(!texture->IsInit())
			Error("trying to destroy a texture that has already been destroyed");
		else
			texture->Destroy();

		for(auto& gs : m_graphics)
		{
			if(gs)
				gs->TryEraseSpritesInScene(texture);
		}
		delete texture;
		delete image;
	}

	Application::~Application()
	{
		m_mesh_registry.Reset();
		m_font_registry.Reset();
		p_render_core.reset();
		p_sdl_manager.reset();
		#ifdef PROFILER
			p_profiler.reset();
		#endif
		p_mem_manager.reset();
	}
}
