/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fps.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 14:56:17 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/25 22:59:13 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pre_compiled.h>

#include <core/fps.h>

namespace mlx
{
	void FpsManager::init()
	{
		_timer = static_cast<std::uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count());
		_fps_before = _timer;
		_fps_now = _timer;
	}

	bool FpsManager::update()
	{
		using namespace std::chrono_literals;
		_fps_now = static_cast<std::uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count());

		if(std::chrono::duration<std::uint64_t>{_fps_now - _timer} >= 1s)
			_timer += _fps_now;

		_fps_elapsed_time = _fps_now - _fps_before;
		if(_fps_elapsed_time >= _ns)
		{
			_fps_before += _ns;
			return true;
		}
		std::this_thread::sleep_for(std::chrono::duration<double, std::nano>(_ns - 1));
		return false;
	}
}
