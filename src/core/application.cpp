/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   application.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 22:10:52 by maldavid          #+#    #+#             */
/*   Updated: 2023/04/02 23:49:03 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "application.h"
#include <renderer/images/texture.h>
#include <renderer/core/render_core.h>
#include <X11/X.h> // for LSBFirst

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

	void* Application::newTexture(int w, int h)
	{
		std::shared_ptr<Texture> texture = std::make_shared<Texture>();
		texture->create(nullptr, w, h, VK_FORMAT_R8G8B8A8_UNORM);
		TextureID id = _texture_lib.addTextureToLibrary(texture);
		_texture_ids.push_back(id);
		return &_texture_ids.back();
	}

	void* Application::newStbTexture(char* file, int* w, int* h)
	{
		std::shared_ptr<Texture> texture = std::make_shared<Texture>(stbTextureLoad(file, w, h));
		TextureID id = _texture_lib.addTextureToLibrary(texture);
		_texture_ids.push_back(id);
		return &_texture_ids.back();
	}

	char* Application::mapTexture(void* img, int* bits_per_pixel, int* size_line, int* endian)
	{
		TextureID id = *static_cast<TextureID*>(img);
		std::shared_ptr<Texture> texture = _texture_lib.getTexture(id);
		char* map = static_cast<char*>(texture->openCPUmap());
		*bits_per_pixel = sizeof(uint32_t) * 8;
		*size_line = texture->getWidth();
		*endian = LSBFirst;
		return map;
	}

	void Application::destroyTexture(void* ptr)
	{
        vkDeviceWaitIdle(Render_Core::get().getDevice().get());
		TextureID id = *static_cast<TextureID*>(ptr);
		_texture_lib.removeTextureFromLibrary(id);
	}
}
