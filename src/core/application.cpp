/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   application.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 22:10:52 by maldavid          #+#    #+#             */
/*   Updated: 2023/04/01 16:03:45 by maldavid         ###   ########.fr       */
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
			for(auto win : _wins)
				win->beginFrame();

			if(_loop_hook)
				_loop_hook(_param);
			
			for(auto win : _wins)
				win->endFrame();
		}
	}

	void* Application::new_stb_texture(char* file, int* w, int* h)
	{
		std::shared_ptr<Texture> texture = std::make_shared<Texture>(stb_texture_load(file, w, h));
		TextureID id = _texture_lib.addTextureToLibrary(texture);
		_texture_ids.push_back(id);
		return &_texture_ids.back();
	}

	void Application::texture_put(void* win, void* img, int x, int y)
	{
		std::shared_ptr<Texture> texture = _texture_lib.getTexture(*static_cast<TextureID*>(img));
		_wins[*static_cast<int*>(win)]->texture_put(texture, x, y);
	}

	void Application::destroy_texture(void* ptr)
	{
        vkDeviceWaitIdle(Render_Core::get().getDevice().get());
		TextureID id = *static_cast<TextureID*>(ptr);
		_texture_lib.removeTextureFromLibrary(id);
	}
}
