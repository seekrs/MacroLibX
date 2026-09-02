#include <PreCompiled.h>

#include <Platform/Inputs.h>
#include <Core/SDLManager.h>
#include <Core/EventBus.h>

namespace mlx
{
	namespace Internal
	{
		struct SwapchainResizeEventBroadcast : public EventBase
		{
			Event What() const override { return Event::SwapchainResizeEventCode; }
		};
	}

	void Inputs::FetchInputs()
	{
		SDLManager::Get().InputsFetcher([this](mlx_event_type event, int window_id, int device_id, int code)
		{
			if(!m_windows.contains(window_id))
				return;
			if(!m_events_hooks.contains(window_id) || m_events_hooks[window_id][event].empty())
				return;
			if(event == MLX_WINDOW_EVENT && code == 8)
				EventBus::SendBroadcast(Internal::SwapchainResizeEventBroadcast{});

			m_event_device_id = device_id;
			for(const auto& hook : m_events_hooks[window_id][event])
			{
				if(hook.fn)
					hook.fn(code, hook.param);
			}
			m_event_device_id = -1;
		});
	}

	int Inputs::GetDefaultControllerId() noexcept
	{
		if (!SDLManager::Get().IsValidController(m_default_controller_id))
			m_default_controller_id += 1;

		return m_default_controller_id;
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

	float Inputs::GetControllerAxis(int controller_id, int axis_kind) const noexcept
	{
		return SDLManager::Get().GetControllerAxis(controller_id, axis_kind);
	}
}
