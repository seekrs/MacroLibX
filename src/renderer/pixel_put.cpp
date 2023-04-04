/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pixel_put.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 15:14:50 by maldavid          #+#    #+#             */
/*   Updated: 2023/04/03 14:22:27 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <renderer/pixel_put.h>
#include <cstring>

namespace mlx
{
	void PixelPutPipeline::init(uint32_t width, uint32_t height, Renderer& renderer) noexcept
	{
		_texture.create(nullptr, width, height, VK_FORMAT_R8G8B8A8_UNORM);
		_texture.setDescriptor(renderer.getFragDescriptorSet().duplicate());

		_buffer.create(Buffer::kind::dynamic, sizeof(uint32_t) * (width * height), VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
		_width = width;
		_height = height;
	}

	VkDescriptorSet PixelPutPipeline::getDescriptorSet() noexcept
	{
		return _texture.getSet();
	}

	void PixelPutPipeline::setPixel(uint32_t x, uint32_t y, int color) noexcept
	{
		if(x < 0 || y < 0 || x > _width || y > _height)
			return;
		if(!_buffer.isMapped())
			_buffer.mapMem(&_map);
		unsigned char* mem = static_cast<unsigned char*>(_map) + (y * _width * sizeof(uint32_t)) + (x * sizeof(uint32_t));
		int new_color = color & 0xFFFFFF00;
		new_color >>= 8;
		new_color |= (color << 24) & 0xFF000000;
		*reinterpret_cast<uint32_t*>(mem) = new_color;
		_has_been_modified = true;
	}

	void PixelPutPipeline::clear()
	{
		if(!_buffer.isMapped())
			_buffer.mapMem(&_map);
		unsigned char* mem = static_cast<unsigned char*>(_map);
		std::memset(mem, 0, sizeof(uint32_t) * (_width * _height));
		_has_been_modified = true;
	}

	void PixelPutPipeline::present() noexcept
	{
		if(_has_been_modified)
		{
			_texture.copyFromBuffer(_buffer);
			_has_been_modified = false;
		}
		_texture.updateSet(0);
	}

	void PixelPutPipeline::render(Renderer& renderer) noexcept
	{
		_texture.render(renderer, 0, 0);
	}
	
	void PixelPutPipeline::destroy() noexcept
	{
		_buffer.destroy();
		_texture.destroy();
	}

	PixelPutPipeline::~PixelPutPipeline() {}
}
