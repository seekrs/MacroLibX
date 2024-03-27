/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventBus.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 17:36:05 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/27 17:37:01 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
