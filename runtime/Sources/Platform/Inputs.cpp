#include <PreCompiled.h>

#include <mlx.h>
#include <Platform/Inputs.h>
#include <Core/SDLManager.h>

namespace mlx
{
	void Inputs::FetchInputs()
	{
		SDLManager::Get().InputsFetcher([this](mlx_event_type event, int window_id, int code)
		{
			if(!m_windows.contains(window_id))
				return;
			if(!m_events_hooks.contains(window_id) || m_events_hooks[window_id][event].empty())
				return;
			for(const auto& hook : m_events_hooks[window_id][event])
			{
				if(hook.fn)
					hook.fn(code, hook.param);
			}
		});
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
