/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventBus.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 17:30:36 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/27 17:31:41 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_EVENT_BUS__
#define __MLX_EVENT_BUS__

#include <Core/EventBase.h>
#include <Core/EventListener.h>

namespace mlx
{
	class EventBus
	{
		public:
			EventBus() = delete;
			static void Send(const std::string& listener_name, const EventBase& event);
			static void SendBroadcast(const EventBase& event);
			inline static void RegisterListener(const EventListener& listener) { s_listeners.push_back(listener); }
			~EventBus() = delete;

		private:
			inline static std::vector<EventListener> s_listeners;
	};
}

#endif
