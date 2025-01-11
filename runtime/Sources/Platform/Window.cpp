#include <PreCompiled.h>

#include <Core/SDLManager.h>
#include <Platform/Window.h>

namespace mlx
{
	Window::Window(const mlx_window_create_info* info, bool hidden) : m_name(info->title), m_width(info->width), m_height(info->height)
	{
		p_window = SDLManager::Get().CreateWindow(info, m_id, hidden);
	}

	void Window::Destroy() noexcept
	{
		if(p_window != nullptr)
		{
			SDLManager::Get().DestroyWindow(p_window);
			p_window = nullptr;
		}
	}
}
