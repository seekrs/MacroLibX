/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pixel_put.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 13:18:50 by maldavid          #+#    #+#             */
/*   Updated: 2023/04/19 11:32:34 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_PIXEL_PUT__
#define __MLX_PIXEL_PUT__

#include <renderer/images/texture.h>
#include <renderer/descriptors/vk_descriptor_set.h>

namespace mlx
{
	class PixelPutPipeline
	{
		public:
			PixelPutPipeline() = default;

			void init(uint32_t width, uint32_t height, class Renderer& renderer) noexcept;

			void setPixel(uint32_t x, uint32_t y, uint32_t color) noexcept;
			void present() noexcept;
			void render(class Renderer& renderer) noexcept;
			VkDescriptorSet getDescriptorSet() noexcept;

			void clear();

			void destroy() noexcept;

			~PixelPutPipeline();

		private:
			Texture _texture;
			Buffer _buffer;
			void* _map = nullptr;
			uint32_t _width = 0;
			uint32_t _height = 0;
			bool _has_been_modified = true;
	};
}

#endif
