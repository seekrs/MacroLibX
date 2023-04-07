/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text_pipeline.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 16:41:13 by maldavid          #+#    #+#             */
/*   Updated: 2023/04/07 17:24:33 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <renderer/text_pipeline.h>
#include <fstream>

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

namespace mlx
{
	static uint8_t ttf_buffer[1 << 20];
	static uint8_t tmp_bitmap[512 * 512];

	void TextPutPipeline::init(Renderer* renderer) noexcept
	{
		_renderer = renderer;
	}

	void TextPutPipeline::put(int x, int y, int color, std::string str)
	{
		
	}

	void TextPutPipeline::destroy() noexcept
	{
		_atlas.destroy();
	}
}
