/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PixelPut.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 15:14:50 by maldavid          #+#    #+#             */
/*   Updated: 2024/04/24 01:46:06 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <PreCompiled.h>

#include <Renderer/PixelPut.h>

namespace mlx
{
	void PixelPutPipeline::Init(std::uint32_t width, std::uint32_t height, Renderer& renderer) noexcept
	{
		MLX_PROFILE_FUNCTION();
		m_texture.Create(nullptr, width, height, VK_FORMAT_R8G8B8A8_UNORM, "__mlx_pixel_put_pipeline_texture", true);
	 	m_texture.SetDescriptor(renderer.GetFragDescriptorSet().Duplicate());

		m_buffer.Create(BufferType::HighDynamic, sizeof(std::uint32_t) * (width * height), VK_BUFFER_USAGE_TRANSFER_SRC_BIT, "__mlx_pixel_put_pipeline_texture");
		m_buffer.MapMem(&_buffer_map);
		m_cpu_map = std::vector<std::uint32_t>(height * width + 1, 0);
		m_width = width;
		m_height = height;
	}

	void PixelPutPipeline::SetPixel(int x, int y, std::uint32_t color) noexcept
	{
		MLX_PROFILE_FUNCTION();
		if(x < 0 || y < 0 || x > static_cast<int>(m_width) || y > static_cast<int>(m_height))
			return;
		m_cpu_map[(y * m_width) + x] = color;
		m_has_been_modified = true;
	}

	void PixelPutPipeline::Clear()
	{
		MLX_PROFILE_FUNCTION();
		m_cpu_map.assign(m_width * m_height, 0);
		m_has_been_modified = true;
	}

	void PixelPutPipeline::Render(Renderer& renderer) noexcept
	{
		MLX_PROFILE_FUNCTION();
		if(m_has_been_modified)
		{
			std::memcpy(m_buffer_map, m_cpu_map.data(), sizeof(std::uint32_t) * m_cpu_map.size());
			m_texture.CopyFromBuffer(m_buffer);
			m_has_been_modified = false;
		}
		m_texture.UpdateSet(0);
		m_texture.Render(renderer, 0, 0);
	}

	void PixelPutPipeline::Destroy() noexcept
	{
		MLX_PROFILE_FUNCTION();
		m_buffer.Destroy();
		m_texture.Destroy();
	}
}
