#pragma once
#include <Core/Application.h>
#include <Core/Handles.h>
#include <Embedded/DogicaTTF.h>

#ifndef DISABLE_ALL_SAFETIES
	#define CHECK_WINDOW_PTR(win, retval) \
		if(win == nullptr) \
		{ \
			Error("invalid window handle (NULL)"); \
			return retval; \
		} \
		else if(std::find_if(m_graphics.begin(), m_graphics.end(), [win](const std::unique_ptr<GraphicsSupport>& gs){ return gs && win->id == gs->GetID(); }) == m_graphics.end()) \
		{ \
			Error("invalid window handle"); \
			return retval; \
		} else {}

	#define CHECK_IMAGE_PTR(img, retval) \
		if(img == nullptr) \
		{ \
			Error("invalid image handle (NULL)"); \
			return retval; \
		} \
		else if(!m_image_registry.IsTextureKnown(image->texture)) \
		{ \
			Error("invalid image handle"); \
			return retval; \
		} else {}
#else
	#define CHECK_WINDOW_PTR(win, retval)
	#define CHECK_IMAGE_PTR(img, retval)
#endif

namespace mlx
{
	void Application::GetMousePos(int* x, int* y) noexcept
	{
		*x = m_in.GetX();
		*y = m_in.GetY();
	}

	void Application::OnEvent(mlx_window win, int event, void(*f)(int, void*), void* param) noexcept
	{
		CHECK_WINDOW_PTR(win, );
		if(!m_graphics[win->id]->HasWindow())
			return;
		m_in.OnEvent(m_graphics[win->id]->GetWindow()->GetID(), event, f, param);
	}

	void Application::SetFPSCap(std::uint32_t fps) noexcept
	{
		if(fps == 0)
		{
			SDL_DisplayMode mode;
			if(!SDL_GetCurrentDisplayMode(1, &mode))
				return;
			fps = mode.refresh_rate;
		}
		m_fps.SetMaxFPS(fps);
	}

	mlx_window Application::NewGraphicsSuport(const mlx_window_create_info* info)
	{
		MLX_PROFILE_FUNCTION();
		if(!info)
		{
			Error("invalid window create info (NULL)");
			return nullptr;
		}

		mlx_window window;
		try { window = new mlx_window_handler; }
		catch(...) { return nullptr; }

		m_graphics.emplace_back(std::make_unique<GraphicsSupport>(info, m_graphics.size()));
		if(m_graphics.back()->HasWindow())
			m_in.RegisterWindow(m_graphics.back()->GetWindow());
		m_graphics.back()->GetScene().BindFont(p_last_font_bound);
		window->id = m_graphics.back()->GetID();
		return window;
	}

	NonOwningPtr<GraphicsSupport> Application::GetGraphicsSupport(mlx_window win)
	{
		CHECK_WINDOW_PTR(win, nullptr);
		return m_graphics[win->id].get();
	}

	void Application::DestroyGraphicsSupport(mlx_window win)
	{
		MLX_PROFILE_FUNCTION();
		CHECK_WINDOW_PTR(win, );
		m_graphics[win->id].reset();
		delete win;
	}

	void Application::LoadFont(const std::filesystem::path& filepath, float scale)
	{
		MLX_PROFILE_FUNCTION();
		std::shared_ptr<Font> font = m_font_registry.GetFont(filepath, scale);
		if(!font)
		{
			if(filepath.string() == "default")
				font = std::make_shared<Font>("default", dogica_ttf, scale);
			else
				font = std::make_shared<Font>(filepath, scale);
			font->BuildFont();
			m_font_registry.RegisterFont(font);
		}

		p_last_font_bound = font;

		for(auto& gs : m_graphics)
		{
			if(gs)
				gs->GetScene().BindFont(font);
		}
	}

	NonOwningPtr<Texture> Application::GetTexture(mlx_image image)
	{
		CHECK_IMAGE_PTR(image, nullptr);
		NonOwningPtr<Texture> texture = image->texture;
		if(!texture->IsInit())
		{
			Error("trying to use a texture that has been destroyed");
			return nullptr;
		}
		return texture;
	}

	void Application::AddLoopHook(void(*f)(void*), void* param)
	{
		m_hooks.emplace_back(f, param);
	}
	
	void Application::LoopEnd() noexcept
	{
		m_in.Finish();
	}
}
