/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inputs.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/05 16:30:19 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/27 15:50:07 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pre_compiled.h>

#include "inputs.h"
#include <mlx.h>
#include <core/profiler.h>

namespace mlx
{
	void Input::update()
	{
		MLX_PROFILE_FUNCTION();
		_xRel = 0;
		_yRel = 0;

		glfwPollEvents();

		static int i = 0;
		i++;
		if(i >= 500)
		{
			auto& hooks = _events_hooks[0];
			auto& win_hook = hooks[MLX_WINDOW_EVENT];
			if(win_hook.hook)
				win_hook.hook(0, win_hook.param);
		}
	}
}
