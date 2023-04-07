/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text_pipeline.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 16:41:13 by maldavid          #+#    #+#             */
/*   Updated: 2023/04/07 00:13:50 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <renderer/text_pipeline.h>

#define STB_TRUETYPE_IMPLEMENTATION
#include <std_truetype.h>

namespace mlx
{
	void TextPutPipeline::init(Renderer& renderer) noexcept
	{
		
	}

	void TextPutPipeline::put(int x, int y, int color, std::string str)
	{

	}
}
