#ifndef __MLX_EVENT_LISTENER__
#define __MLX_EVENT_LISTENER__

#include <Core/EventBase.h>

namespace mlx
{
	class EventListener
	{
		public:
			EventListener() = delete;
			EventListener(func::function<void(const EventBase&)> functor, std::string name);

			inline const std::string& GetName() const { return m_name; }
			inline void Call(const EventBase& event) const noexcept { m_listen_functor(event); }

			~EventListener() = default;

		private:
			func::function<void(const EventBase&)> m_listen_functor;
			std::string m_name;
	};
}

#endif
