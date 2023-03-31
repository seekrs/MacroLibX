/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pixel_put.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 13:18:50 by maldavid          #+#    #+#             */
/*   Updated: 2023/03/31 20:30:25 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_PIXEL_PUT__
#define __MLX_PIXEL_PUT__

#include <renderer/buffers/vk_buffer.h>
#include <renderer/images/vk_image.h>
#include <renderer/descriptors/vk_descriptor_set.h>

namespace mlx
{
	class PixelPutPipeline
	{
		public:
			PixelPutPipeline() = default;

			void init(uint32_t width, uint32_t height, class Renderer& renderer) noexcept;

			void setPixel(uint32_t x, uint32_t y, int color) noexcept;
			void present() noexcept;
			inline VkDescriptorSet& getDescriptorSet() noexcept { return _image_set.get(); }

			void destroy() noexcept;

			~PixelPutPipeline() = default;

		private:
			Image _image;
			Buffer _buffer;
			DescriptorSet _image_set;
			void* _map = nullptr;
			uint32_t _width = 0;
			uint32_t _height = 0;
	};
}

#endif
