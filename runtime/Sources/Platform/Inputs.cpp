#include "mlx.h"
#include "mlx_keycodes.h"
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
		SDLManager::Get().InputsFetcher([this](mlx_event_type event, int window_id, int code)
		{
			if(!m_windows.contains(window_id))
				return;
			if(!m_events_hooks.contains(window_id) || m_events_hooks[window_id][event].empty())
				return;
			if(event == MLX_WINDOW_EVENT && code == MLX_WINDOW_SIZE_CHANGED)
				EventBus::SendBroadcast(Internal::SwapchainResizeEventBroadcast{});

			for(const auto& hook : m_events_hooks[window_id][event])
			{
				if(hook.fn)
					hook.fn(code, hook.param);
			}
		});
	}

	int Inputs::GetDefaultControllerId() noexcept
	{
		return SDLManager::Get().GetFirstConnectedController();
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

	void Inputs::RumbleController(int controller_id, float low_freq, float high_freq, float duration) const noexcept
	{
		return SDLManager::Get().RumbleController(controller_id, high_freq, low_freq, duration);
	}
}
