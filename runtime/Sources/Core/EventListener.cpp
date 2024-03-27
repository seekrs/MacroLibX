/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventListener.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 17:37:09 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/27 17:37:38 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <PreCompiled.h>
#include <Core/EventListener.h>

namespace mlx
{
	EventListener::EventListener(func::function<void(const EventBase&)> functor, std::string name)
	: m_listen_functor(std::move(functor)), m_name(std::move(name))
	{}
}
