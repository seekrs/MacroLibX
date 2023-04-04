/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   application.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 22:10:52 by maldavid          #+#    #+#             */
/*   Updated: 2023/04/05 01:11:36 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "application.h"
#include <renderer/images/texture.h>
#include <renderer/core/render_core.h>
#include <X11/X.h> // for LSBFirst
#include <array>
#include <cstring>

#ifndef MLX_NO_XPM
	#include <X11/xpm.h>
#endif

#include <iostream>

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

#ifndef MLX_NO_XPM
	struct SafeXpmImage: public XpmImage // included in <X11/xpm.h>
	{
		~SafeXpmImage() { XpmFreeXpmImage(this); }
	};

	void* Application::newXpmTexture(char** data, int* w, int* h)
	{
		SafeXpmImage xpm_image;
		if(XpmCreateXpmImageFromData(data, &xpm_image, nullptr) != XpmSuccess)
		{
			error::report(e_kind::error, "XPM reader : invalid xpm file");
			return nullptr;
		}

		*w = xpm_image.width;
		*h = xpm_image.height;

		std::vector<uint8_t> formatted_data;
		formatted_data.reserve((*w) * (*h) * 4);
		uint32_t pixel_color;
		for(int i = 0; i < *h; i++)
		{
			for(int j = 0, k = 0; k < *w; j += 4, k++)
			{
				pixel_color = -1;
				for(int l = 0; l < xpm_image.ncolors; l++)
				{
					if(std::memcmp(xpm_image.colorTable[l].c_color, &xpm_image.data[(i * (*w)) + k], 1) == 0)
					{
						pixel_color = l;
						break;
					}
				}

				if(pixel_color < xpm_image.ncolors)
				{
					formatted_data[(i * (*w)) + j + 0] = xpm_image.colorTable[pixel_color].m_color[0];
					formatted_data[(i * (*w)) + j + 1] = xpm_image.colorTable[pixel_color].m_color[1];
					formatted_data[(i * (*w)) + j + 2] = xpm_image.colorTable[pixel_color].m_color[1];
					formatted_data[(i * (*w)) + j + 3] = 0xFF;
				}
				else
				{
					formatted_data[(i * (*w)) + j + 0] = 0xFF;
					formatted_data[(i * (*w)) + j + 1] = 0xFF;
					formatted_data[(i * (*w)) + j + 2] = 0xFF;
					formatted_data[(i * (*w)) + j + 3] = 0xFF;
				}
			}
		}

		std::shared_ptr<Texture> texture = std::make_shared<Texture>();
		texture->create(formatted_data.data(), *w, *h, VK_FORMAT_R8G8B8A8_UNORM);
		TextureID id = _texture_lib.addTextureToLibrary(texture);
		_texture_ids.push_back(id);
		return &_texture_ids.back();
	}

	void* Application::newXpmTexture(std::string filename, int* w, int* h)
	{
		SafeXpmImage xpm_image;
		if(XpmReadFileToXpmImage(filename.c_str(), &xpm_image, nullptr) != XpmSuccess)
		{
			error::report(e_kind::error, "XPM reader : invalid xpm file");
			return nullptr;
		}

		*w = xpm_image.width;
		*h = xpm_image.height;

		std::shared_ptr<Texture> texture = std::make_shared<Texture>();
		texture->create(reinterpret_cast<uint8_t*>(xpm_image.data), *w, *h, VK_FORMAT_R8G8B8A8_UNORM);
		TextureID id = _texture_lib.addTextureToLibrary(texture);
		_texture_ids.push_back(id);
		return &_texture_ids.back();
	}
#endif

	void Application::destroyTexture(void* ptr)
	{
        vkDeviceWaitIdle(Render_Core::get().getDevice().get());
		TextureID id = *static_cast<TextureID*>(ptr);
		_texture_lib.removeTextureFromLibrary(id);
	}
}
