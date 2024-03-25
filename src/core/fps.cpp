/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fps.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 14:56:17 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/25 16:44:15 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <core/fps.h>
#include <chrono>
#include <SDL2/SDL.h>
#include <thread>

namespace mlx
{
	void FpsManager::init()
	{
		_timer = SDL_GetTicks64();
		_fps_before = static_cast<std::uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count());
		_fps_now = static_cast<std::uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count());
	}

	bool FpsManager::update()
	{
		using namespace std::chrono_literals;
		_fps_now = static_cast<std::uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count());

		if(SDL_GetTicks64() - _timer > 1000)
			_timer += 1000;

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
