#include <PreCompiled.h>

#include <Core/SDLManager.h>
#include <Platform/Window.h>

namespace mlx
{
	Window::Window(std::size_t w, std::size_t h, const std::string& title, bool hidden) : m_width(w), m_height(h)
	{
		static std::uint64_t ids = 0;
		p_window = SDLManager::Get().CreateWindow(title, w, h, hidden);
		m_id = ids++;
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