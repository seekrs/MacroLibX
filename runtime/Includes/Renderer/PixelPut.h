/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PixelPut.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 13:18:50 by maldavid          #+#    #+#             */
/*   Updated: 2024/04/24 01:46:11 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_PIXEL_PUT__
#define __MLX_PIXEL_PUT__

#include <Renderer/Images/Texture.h>
#include <Renderer/Descriptors/DescriptorSet.h>

namespace mlx
{
	class PixelPutPipeline
	{
		public:
			PixelPutPipeline() = default;

			void Init(std::uint32_t width, std::uint32_t height, class Renderer& renderer) noexcept;

			void SetPixel(int x, int y, std::uint32_t color) noexcept;
			void Render(class Renderer& renderer) noexcept;

			void Clear();
			void Destroy() noexcept;

			~PixelPutPipeline() = default;

		private:
			Texture m_texture;
			Buffer m_buffer;
			// using vector as CPU map and not directly writting to mapped buffer to improve performances
			std::vector<std::uint32_t> m_cpu_map;
			void* m_buffer_map = nullptr;
			std::uint32_t m_width = 0;
			std::uint32_t m_height = 0;
			bool m_has_been_modified = true;
	};
}

#endif
