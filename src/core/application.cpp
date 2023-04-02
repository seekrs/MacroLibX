/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   application.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 22:10:52 by maldavid          #+#    #+#             */
/*   Updated: 2023/04/02 15:11:53 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "application.h"
#include <renderer/images/texture.h>
#include <renderer/core/render_core.h>

namespace mlx::core
{
	void Application::run() noexcept
	{
		while(_in.is_running())
		{
			_in.update();
			for(auto& gs : _graphics)
				gs->beginRender();

			if(_loop_hook)
				_loop_hook(_param);
			
			for(auto& gs : _graphics)
				gs->endRender();
		}
	}

	void* Application::newStbTexture(char* file, int* w, int* h)
	{
		std::shared_ptr<Texture> texture = std::make_shared<Texture>(stbTextureLoad(file, w, h));
		TextureID id = _texture_lib.addTextureToLibrary(texture);
		_texture_ids.push_back(id);
		return &_texture_ids.back();
	}

	void Application::destroyTexture(void* ptr)
	{
        vkDeviceWaitIdle(Render_Core::get().getDevice().get());
		TextureID id = *static_cast<TextureID*>(ptr);
		_texture_lib.removeTextureFromLibrary(id);
	}
}
