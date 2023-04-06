/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xpm_reader.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/05 13:25:55 by maldavid          #+#    #+#             */
/*   Updated: 2023/04/05 13:35:48 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_XPM_READER__
#define __MLX_XPM_READER__

#include <filesystem>
#include <cstdint>
#include <vector>

namespace mlx
{
	std::vector<uint8_t> parseXpmData(char** data, int* w, int* h);
	std::vector<uint8_t> parseXpmFile(std::filesystem::path file, int* w, int* h);
}

#endif
