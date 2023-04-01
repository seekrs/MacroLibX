/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pixel_put.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 13:18:50 by maldavid          #+#    #+#             */
/*   Updated: 2023/04/01 17:26:46 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_PIXEL_PUT__
#define __MLX_PIXEL_PUT__

#include <renderer/descriptors/vk_descriptor_set.h>

namespace mlx
{
	class PixelPutPipeline
	{
		public:
			PixelPutPipeline();

			void init(uint32_t width, uint32_t height, class Renderer& renderer) noexcept;

			void setPixel(uint32_t x, uint32_t y, int color) noexcept;
			void present() noexcept;
			void render(class Renderer& renderer) noexcept;
			VkDescriptorSet getDescriptorSet() noexcept;

			void clear();

			void destroy() noexcept;

			~PixelPutPipeline();

		private:
			struct _Pimpl;
			std::unique_ptr<_Pimpl> _impl;
	};
}

#endif
