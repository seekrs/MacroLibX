/* **************************************************************************** */
/*                                                                              */
/*                                                         :::      ::::::::    */
/*    DriverLoader.inl                                   :+:      :+:    :+:    */
/*                                                     +:+ +:+         +:+      */
/*    By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+         */
/*                                                 +#+#+#+#+#+   +#+            */
/*    Created: 2024/04/03 14:55:01 by maldavid          #+#    #+#              */
/*    Updated: 2024/04/03 14:55:01 by maldavid         ###   ########.fr        */
/*                                                                              */
/* **************************************************************************** */

#pragma once
#include <Core/DriverLoader.h>

namespace mlx
{
	template <typename T>
	bool DriverLoader::LoadDriver()
	{
		m_instances.emplace_back(new T)->InitDriver();
	}

	void DriverLoader::ShutdownAllDrivers()
	{
		for(auto& driver : m_instances)
			driver->ShutdownDriver();
		m_instances.clear();
	}
}
