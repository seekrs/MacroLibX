#pragma once
#include <Core/Application.h>

#define CHECK_WINDOW_PTR(win) \
	if(win == nullptr) \
	{ \
		Error("invalid window ptr (NULL)"); \
		return; \
	} \
	else if(*static_cast<int*>(win) < 0 || *static_cast<int*>(win) > static_cast<int>(m_graphics.size()))\
	{ \
		Error("invalid window ptr"); \
		return; \
	} else {}

#define CHECK_IMAGE_PTR(img, retval) \
	if(img == nullptr) \
	{ \
		Error("invalid image ptr (NULL)"); \
		retval; \
	} \
	else if(m_image_registry.Find(img)) \
	{ \
		Error("invalid image ptr"); \
		retval; \
	} else {}

namespace mlx
{
	void Application::GetMousePos(int* x, int* y) noexcept
	{
		*x = p_in->GetX();
		*y = p_in->GetY();
	}

	void Application::MouseMove(Handle win, int x, int y) noexcept
	{
		CHECK_WINDOW_PTR(win);
		if(!m_graphics[*static_cast<int*>(win)]->HasWindow())
		{
			Warning("trying to move the mouse relative to a window that is targeting an image and not a real window, this is not allowed (move ignored)");
			return;
		}
	}

	void Application::OnEvent(Handle win, int event, int (*funct_ptr)(int, void*), void* param) noexcept
	{
		CHECK_WINDOW_PTR(win);
		if(!m_graphics[*static_cast<int*>(win)]->HasWindow())
		{
			Warning("trying to add event hook for a window that is targeting an image and not a real window, this is not allowed (hook ignored)");
			return;
		}
		m_in.OnEvent(m_graphics[*static_cast<int*>(win)]->GetWindow()->GetID(), event, funct_ptr, param);
	}

	void Application::GetScreenSize(Handle win, int* w, int* h) noexcept
	{
		CHECK_WINDOW_PTR(win);
		*w = 0;
		*h = 0;
	}

	void Application::SetFPSCap(std::uint32_t fps) noexcept
	{
		m_fps.SetMaxFPS(fps);
	}

	void* Application::NewGraphicsSuport(std::size_t w, std::size_t h, const char* title)
	{
		MLX_PROFILE_FUNCTION();
		auto it = std::find_if(m_textures.begin(), m_textures.end(), [=](const Texture& texture)
		{
			return &texture == reinterpret_cast<Texture*>(const_cast<char*>(title));
		});
		if(it != _textures.end())
			m_graphics.emplace_back(std::make_unique<GraphicsSupport>(w, h, reinterpret_cast<Texture*>(const_cast<char*>(title)), m_graphics.size()));
		else
		{
			if(title == NULL)
			{
				FatalError("invalid window title (NULL)");
				return nullptr;
			}
			m_graphics.emplace_back(std::make_unique<GraphicsSupport>(w, h, title, m_graphics.size()));
			m_in.RegisterWindow(m_graphics.back()->GetWindow());
		}
		return static_cast<void*>(&m_graphics.back()->GetID());
	}

	void Application::ClearGraphicsSupport(Handle win)
	{
		MLX_PROFILE_FUNCTION();
		CHECK_WINDOW_PTR(win);
		m_graphics[*static_cast<int*>(win)]->ClearRenderData();
	}

	void Application::DestroyGraphicsSupport(Handle win)
	{
		MLX_PROFILE_FUNCTION();
		CHECK_WINDOW_PTR(win);
		m_graphics[*static_cast<int*>(win)].reset();
	}

	void Application::PixelPut(Handle win, int x, int y, std::uint32_t color) const noexcept
	{
		MLX_PROFILE_FUNCTION();
		CHECK_WINDOW_PTR(win);
		m_graphics[*static_cast<int*>(win)]->PixelPut(x, y, color);
	}

	void Application::StringPut(Handle win, int x, int y, std::uint32_t color, char* str)
	{
		MLX_PROFILE_FUNCTION();
		CHECK_WINDOW_PTR(win);
		if(str == nullptr)
		{
			Error("wrong text (NULL)");
			return;
		}
		if(std::strlen(str) == 0)
		{
			Warning("trying to put an empty text");
			return;
		}
		m_graphics[*static_cast<int*>(win)]->StringPut(x, y, color, str);
	}

	void Application::LoadFont(Handle win, const std::filesystem::path& filepath, float scale)
	{
		MLX_PROFILE_FUNCTION();
		CHECK_WINDOW_PTR(win);
		m_graphics[*static_cast<int*>(win)]->LoadFont(filepath, scale);
	}

	void Application::TexturePut(Handle win, Handle img, int x, int y)
	{
		MLX_PROFILE_FUNCTION();
		CHECK_WINDOW_PTR(win);
		CHECK_IMAGE_PTR(img, return);
		NonOwningPtr<Texture> texture = static_cast<Texture*>(img);
		if(!texture->IsInit())
			Error("trying to put a texture that has been destroyed");
		else
			m_graphics[*static_cast<int*>(win)]->TexturePut(texture, x, y);
	}

	int Application::GetTexturePixel(Handle img, int x, int y)
	{
		MLX_PROFILE_FUNCTION();
		CHECK_IMAGE_PTR(img, return 0);
		NonOwningPtr<Texture> texture = static_cast<Texture*>(img);
		if(!texture->IsInit())
		{
			Error("trying to get a pixel from texture that has been destroyed");
			return 0;
		}
		return texture->GetPixel(x, y);
	}

	void Application::SetTexturePixel(Handle img, int x, int y, std::uint32_t color)
	{
		MLX_PROFILE_FUNCTION();
		CHECK_IMAGE_PTR(img, return);
		NonOwningPtr<Texture> texture = static_cast<Texture*>(img);
		if(!texture->IsInit())
			Error("trying to set a pixel on texture that has been destroyed");
		else
			texture->SetPixel(x, y, color);
	}

	void Application::LoopHook(int (*f)(void*), void* param)
	{
		f_loop_hook = f;
		p_param = param;
	}
	
	void Application::LoopEnd() noexcept
	{
		p_in->Finish();
	}
}
