/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   application.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 22:10:52 by maldavid          #+#    #+#             */
/*   Updated: 2022/12/19 00:40:17 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "application.h"

namespace mlx::core
{
	void Application::run() noexcept
	{
		while(_in.is_running())
		{
			_in.update();
			for(auto win : _wins)
				win->beginFrame();

			if(_loop_hook)
				_loop_hook(_param);
			
			for(auto win : _wins)
				win->endFrame();
		}
	}
}
