#include <PreCompiled.h>
#include <Core/EventBus.h>
#include <Core/Logs.h>

namespace mlx
{
	void EventBus::Send(const std::string& listener_name, const EventBase& event)
	{
		for(const EventListener& listener : s_listeners)
		{
			if(listener.GetName() == listener_name)
			{
				listener.Call(event);
				return;
			}
		}
		Warning("Event Bus : listener not found, '%'", listener_name);
	}

	void EventBus::SendBroadcast(const EventBase& event)
	{
		for(const EventListener& listener : s_listeners)
			listener.Call(event);
	}
}
