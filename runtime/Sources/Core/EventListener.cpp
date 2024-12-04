#include <PreCompiled.h>
#include <Core/EventListener.h>

namespace mlx
{
	EventListener::EventListener(func::function<void(const EventBase&)> functor, std::string name)
	: m_listen_functor(std::move(functor)), m_name(std::move(name))
	{}
}
