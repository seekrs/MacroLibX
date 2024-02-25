/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pixel_put.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 15:14:50 by maldavid          #+#    #+#             */
/*   Updated: 2024/01/11 00:06:01 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <renderer/pixel_put.h>
#include <cstring>
#include <core/profiler.h>

namespace mlx
{
	void PixelPutPipeline::init(std::uint32_t width, std::uint32_t height, Renderer& renderer) noexcept
	{
		MLX_PROFILE_FUNCTION();
		_texture.create(nullptr, width, height, VK_FORMAT_R8G8B8A8_UNORM, "__mlx_pixel_put_pipeline_texture", true);
		_texture.setDescriptor(renderer.getFragDescriptorSet().duplicate());

		_buffer.create(Buffer::kind::dynamic, sizeof(std::uint32_t) * (width * height), VK_BUFFER_USAGE_TRANSFER_SRC_BIT, "__mlx_pixel_put_pipeline_texture");
		_buffer.mapMem(&_buffer_map);
		_cpu_map = std::vector<std::uint32_t>(height * width + 1, 0);
		_width = width;
		_height = height;
	}

	void PixelPutPipeline::setPixel(int x, int y, std::uint32_t color) noexcept
	{
		MLX_PROFILE_FUNCTION();
		if(x < 0 || y < 0 || x > static_cast<int>(_width) || y > static_cast<int>(_height))
			return;
		_cpu_map[(y * _width) + x] = color;
		_has_been_modified = true;
	}

	void PixelPutPipeline::clear()
	{
		MLX_PROFILE_FUNCTION();
		_cpu_map.assign(_width * _height, 0);
		_has_been_modified = true;
	}

	void PixelPutPipeline::render(std::array<VkDescriptorSet, 2>& sets, Renderer& renderer) noexcept
	{
		MLX_PROFILE_FUNCTION();
		if(_has_been_modified)
		{
			std::memcpy(_buffer_map, _cpu_map.data(), sizeof(std::uint32_t) * _cpu_map.size());
			_texture.copyFromBuffer(_buffer);
			_has_been_modified = false;
		}
		_texture.updateSet(0);
		_texture.render(sets, renderer, 0, 0);
	}

	void PixelPutPipeline::destroy() noexcept
	{
		MLX_PROFILE_FUNCTION();
		_buffer.destroy();
		_texture.destroy();
	}

	PixelPutPipeline::~PixelPutPipeline() {}
}
