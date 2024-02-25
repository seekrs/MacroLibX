/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pixel_put.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 13:18:50 by maldavid          #+#    #+#             */
/*   Updated: 2024/01/11 00:06:05 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_PIXEL_PUT__
#define __MLX_PIXEL_PUT__

#include <mlx_profile.h>
#include <renderer/images/texture.h>
#include <renderer/descriptors/vk_descriptor_set.h>

namespace mlx
{
	class PixelPutPipeline
	{
		public:
			PixelPutPipeline() = default;

			void init(std::uint32_t width, std::uint32_t height, class Renderer& renderer) noexcept;

			void setPixel(int x, int y, std::uint32_t color) noexcept;
			void render(std::array<VkDescriptorSet, 2>& sets, class Renderer& renderer) noexcept;

			void clear();
			void destroy() noexcept;

			~PixelPutPipeline();

		private:
			Texture _texture;
			Buffer _buffer;
			// using vector as CPU map and not directly writting to mapped buffer to improve performances
			std::vector<std::uint32_t> _cpu_map;
			void* _buffer_map = nullptr;
			std::uint32_t _width = 0;
			std::uint32_t _height = 0;
			bool _has_been_modified = true;
	};
}

#endif
