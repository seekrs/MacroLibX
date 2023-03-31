/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pixel_put.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 15:14:50 by maldavid          #+#    #+#             */
/*   Updated: 2023/03/31 20:30:38 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <renderer/pixel_put.h>
#include <renderer/renderer.h>
#include <cstring>

namespace mlx
{
	void PixelPutPipeline::init(uint32_t width, uint32_t height, Renderer& renderer) noexcept
	{
		_image.create(width, height, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_TILING_OPTIMAL,
					VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
					VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		_image.createImageView(VK_IMAGE_VIEW_TYPE_2D, VK_IMAGE_ASPECT_COLOR_BIT);
		_image.createSampler();
		_buffer.create(Buffer::kind::dynamic, sizeof(uint32_t) * (width * height), VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
		_image_set = renderer.getFragDescriptorSet().duplicate();
		_width = width;
		_height = height;
	}

	void PixelPutPipeline::setPixel(uint32_t x, uint32_t y, int color) noexcept
	{
		if(x < 0 || y < 0 || x > _width || y > _height)
			return;
		if(!_buffer.isMapped())
			_buffer.mapMem(&_map);
		
		unsigned char* mem = static_cast<unsigned char*>(_map) + y * _width + x * sizeof(uint32_t);
		*reinterpret_cast<uint32_t*>(mem) = color;
	}

	void PixelPutPipeline::present() noexcept
	{
		if(_buffer.isMapped())
			_buffer.unmapMem();
		_image.copyBuffer(_buffer);
		_image_set.writeDescriptor(0, _image.getImageView(), _image.getSampler());
	}
	
	void PixelPutPipeline::destroy() noexcept
	{
		_buffer.destroy();
		_image.destroy();
	}
}
