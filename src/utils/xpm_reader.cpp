/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xpm_reader.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/05 13:37:21 by maldavid          #+#    #+#             */
/*   Updated: 2023/04/05 13:53:30 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <utils/xpm_reader.h>
#include <core/errors.h>
#include <cstdio>

namespace mlx
{
	std::vector<uint8_t> parseXpmFile(std::filesystem::path file, int* w, int* h)
	{
		return {};
	}

	std::vector<uint8_t> parseXpmData(char** data, int* w, int* h)
	{
		if(data == nullptr)
		{
			core::error::report(e_kind::error, "Xpm reader : null data");
			return {};
		}

		uint32_t width;
		uint32_t height;
		uint32_t ncolors;
		uint32_t cpp; // chars per pixels

		if(std::sscanf(data[0], "%d %d %d %d", &width, &height, &ncolors, &cpp) != 4)
		{
			core::error::report(e_kind::error, "Xpm reader : invalid pixmap description");
			return {};
		}

		*w = width;
		*h = height;
	}
}
