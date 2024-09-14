#include <PreCompiled.h>

#include <mlx.h>
#include <Platform/Inputs.h>
#include <Core/SDLManager.h>

namespace mlx
{
	Inputs::Inputs()
	{
		SDLManager::Get().SetEventCallback([this](mlx_event_type event, int window_id, int code, [[maybe_unused]] void* userdata)
		{
			if(m_windows.find(window_id) == m_windows.end())
				return;
			m_events_hooks[window_id][event].hook(code, m_events_hooks[window_id][event].param);
		}, nullptr);
	}

	std::int32_t Inputs::GetX() const noexcept
	{
		return SDLManager::Get().GetX();
	}

	std::int32_t Inputs::GetY() const noexcept
	{
		return SDLManager::Get().GetY();
	}

	std::int32_t Inputs::GetXRel() const noexcept
	{
		return SDLManager::Get().GetXRel();
	}

	std::int32_t Inputs::GetYRel() const noexcept
	{
		return SDLManager::Get().GetYRel();
	}
}
