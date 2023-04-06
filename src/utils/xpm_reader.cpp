/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xpm_reader.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/05 13:37:21 by maldavid          #+#    #+#             */
/*   Updated: 2023/04/06 15:20:14 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <utils/xpm_reader.h>
#include <core/errors.h>
#include <cstdio>
#include <sstream>
#include <iostream>

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

		int32_t width = -1;
		int32_t height = -1;
		int32_t ncolors = -1;
		int32_t cpp = -1; // chars per pixels
						  //
		std::stringstream stream;

		stream.str(data[0]);
		stream >> width;
		stream >> height;
		stream >> ncolors;
		stream >> cpp;
		if(width == -1 || height == -1 || ncolors == -1 || cpp == -1 || !stream.eof())
		{
			core::error::report(e_kind::error, "Xpm reader : invalid pixmap description");
			return {};
		}

		*w = width;
		*h = height;

		stream.clear();
		std::vector<std::string> colors;
		colors.reserve(ncolors);
		std::cout << ncolors << std::endl;
		for(int32_t i = 1; i < ncolors; ++i)
		{
			std::cout << (bool)(i < ncolors) << std::endl;
			stream.str(data[i]);
			std::string c;
			stream >> c;
			
			std::cout << ncolors << "	" << i << "	" << c << std::endl;
		}
	}
}
