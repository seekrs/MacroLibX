/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pixel_put.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 15:14:50 by maldavid          #+#    #+#             */
/*   Updated: 2023/04/01 12:57:32 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <renderer/buffers/vk_ibo.h>
#include <renderer/buffers/vk_vbo.h>
#include <renderer/images/vk_image.h>
#include <renderer/pixel_put.h>
#include <cstring>

namespace mlx
{
	struct PixelPutPipeline::_Pimpl
	{
		Image image;
		C_VBO vbo;
		C_IBO ibo;
		Buffer buffer;
		DescriptorSet image_set;
		void* map = nullptr;
		uint32_t width = 0;
		uint32_t height = 0;
	};

	PixelPutPipeline::PixelPutPipeline() : _impl(std::make_unique<_Pimpl>()) {}

	void PixelPutPipeline::init(uint32_t width, uint32_t height, Renderer& renderer) noexcept
	{
		_impl->image.create(width, height, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_TILING_OPTIMAL,
					VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
					VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		_impl->image.createImageView(VK_IMAGE_VIEW_TYPE_2D, VK_IMAGE_ASPECT_COLOR_BIT);
		_impl->image.createSampler();
		_impl->buffer.create(Buffer::kind::dynamic, sizeof(uint32_t) * (width * height), VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
		_impl->image_set = renderer.getFragDescriptorSet().duplicate();
		_impl->width = width;
		_impl->height = height;

		std::vector<Vertex> vertexData = {
			{{0, 0},						{1.f, 1.f, 1.f},	{0.0f, 0.0f}},
			{{_impl->width, 0},				{1.f, 1.f, 1.f},	{1.0f, 0.0f}},
			{{_impl->width, _impl->height},	{1.f, 1.f, 1.f},	{1.0f, 1.0f}},
			{{0, _impl->height},			{1.f, 1.f, 1.0},	{0.0f, 1.0f}}
		};

		std::vector<uint16_t> indexData = { 0, 1, 2, 2, 3, 0 };

		_impl->vbo.create(sizeof(Vertex) * vertexData.size(), vertexData.data());
		_impl->ibo.create(sizeof(uint16_t) * indexData.size(), indexData.data());
	}

	VkDescriptorSet& PixelPutPipeline::getDescriptorSet() noexcept
	{
		return _impl->image_set.get();
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

	void PixelPutPipeline::present() noexcept
	{
		if(_impl->buffer.isMapped())
			_impl->buffer.unmapMem();
		_impl->image.copyBuffer(_impl->buffer);
		_impl->image_set.writeDescriptor(0, _impl->image.getImageView(), _impl->image.getSampler());
	}

	void PixelPutPipeline::render(Renderer& renderer) noexcept
	{
		_impl->vbo.bind(renderer);
		_impl->ibo.bind(renderer);
		vkCmdDrawIndexed(renderer.getActiveCmdBuffer().get(), static_cast<uint32_t>(_impl->ibo.getSize() / sizeof(uint16_t)), 1, 0, 0, 0);
	}
	
	void PixelPutPipeline::destroy() noexcept
	{
		_impl->vbo.destroy();
		_impl->ibo.destroy();
		_impl->buffer.destroy();
		_impl->image.destroy();
	}

	PixelPutPipeline::~PixelPutPipeline() {}
}
