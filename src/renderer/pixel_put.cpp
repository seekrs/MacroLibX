/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pixel_put.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 15:14:50 by maldavid          #+#    #+#             */
/*   Updated: 2023/08/02 05:28:49 by maldavid         ###   ########.fr       */
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
		_buffer.mapMem(&_buffer_map);
		_cpu_map = std::vector<uint32_t>(height * width, 0);
		_width = width;
		_height = height;
	}

	void PixelPutPipeline::setPixel(uint32_t x, uint32_t y, uint32_t color) noexcept
	{
		if(x < 0 || y < 0 || x > _width || y > _height)
			return;
		_cpu_map[(y * _width) + x] = color;
		_has_been_modified = true;
	}

	void PixelPutPipeline::clear()
	{
		_cpu_map.assign(_width * _height, 0);
		_has_been_modified = true;
	}

	void PixelPutPipeline::present() noexcept
	{
		if(_has_been_modified)
		{
			std::memcpy(_buffer_map, _cpu_map.data(), sizeof(uint32_t) * _cpu_map.size());
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
