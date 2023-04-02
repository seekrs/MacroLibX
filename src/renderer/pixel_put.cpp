/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pixel_put.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 15:14:50 by maldavid          #+#    #+#             */
/*   Updated: 2023/04/02 20:02:30 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <renderer/images/texture.h>
#include <renderer/pixel_put.h>
#include <cstring>

namespace mlx
{
	struct PixelPutPipeline::_Pimpl
	{
		Texture texture;
		Buffer buffer;
		void* map = nullptr;
		uint32_t width = 0;
		uint32_t height = 0;
	};

	PixelPutPipeline::PixelPutPipeline() : _impl(std::make_unique<_Pimpl>()) {}

	void PixelPutPipeline::init(uint32_t width, uint32_t height, Renderer& renderer) noexcept
	{
		_impl->texture.create(nullptr, width, height, VK_FORMAT_R8G8B8A8_UNORM);
		_impl->texture.setDescriptor(renderer.getFragDescriptorSet().duplicate());

		_impl->buffer.create(Buffer::kind::dynamic, sizeof(uint32_t) * (width * height), VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
		_impl->width = width;
		_impl->height = height;
	}

	VkDescriptorSet PixelPutPipeline::getDescriptorSet() noexcept
	{
		return _impl->texture.getSet();
	}

	void PixelPutPipeline::setPixel(uint32_t x, uint32_t y, int color) noexcept
	{
		if(x < 0 || y < 0 || x > _impl->width || y > _impl->height)
			return;
		if(!_impl->buffer.isMapped())
			_impl->buffer.mapMem(&_impl->map);
		
		unsigned char* mem = static_cast<unsigned char*>(_impl->map) + (y * _impl->width * sizeof(uint32_t)) + (x * sizeof(uint32_t));
		*reinterpret_cast<uint32_t*>(mem) = color;
	}

	void PixelPutPipeline::clear()
	{
		if(!_impl->buffer.isMapped())
			_impl->buffer.mapMem(&_impl->map);
		unsigned char* mem = static_cast<unsigned char*>(_impl->map);
		std::memset(mem, 0, sizeof(uint32_t) * (_impl->width * _impl->height));
	}

	void PixelPutPipeline::present() noexcept
	{
		if(_impl->buffer.isMapped())
			_impl->buffer.unmapMem();
		_impl->texture.copyFromBuffer(_impl->buffer);
		_impl->texture.updateSet(0);
	}

	void PixelPutPipeline::render(Renderer& renderer) noexcept
	{
		_impl->texture.render(renderer, 0, 0);
	}
	
	void PixelPutPipeline::destroy() noexcept
	{
		_impl->buffer.destroy();
		_impl->texture.destroy();
	}

	PixelPutPipeline::~PixelPutPipeline() {}
}
