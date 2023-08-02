/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   application.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 22:10:52 by maldavid          #+#    #+#             */
/*   Updated: 2023/04/25 15:12:57 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "application.h"
#include <renderer/images/texture.h>
#include <renderer/core/render_core.h>
#include <array>
#include <utils/endian.h>

namespace mlx::core
{
	void Application::run() noexcept
	{
		while(_in->is_running())
		{
			_in->update();
			for(auto& gs : _graphics)
				gs->beginRender();

			if(_loop_hook)
				_loop_hook(_param);
			
			for(auto& gs : _graphics)
				gs->endRender();
		}
	}

	void* Application::newTexture(int w, int h)
	{
		_textures.emplace_front().create(nullptr, w, h, VK_FORMAT_R8G8B8A8_UNORM);
		return &_textures.front();
	}

	void* Application::newStbTexture(char* file, int* w, int* h)
	{
		_textures.emplace_front(stbTextureLoad(file, w, h));
		return &_textures.front();
	}

	void Application::destroyTexture(void* ptr)
	{
        vkDeviceWaitIdle(Render_Core::get().getDevice().get());
		Texture* texture = static_cast<Texture*>(ptr);
		texture->destroy();
	}
}
