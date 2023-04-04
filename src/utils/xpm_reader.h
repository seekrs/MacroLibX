/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xpm_reader.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 14:55:09 by maldavid          #+#    #+#             */
/*   Updated: 2023/04/04 15:39:16 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * https://github.com/clckwrkbdgr/libchthon
 * This xpm reader is a modified version of libchthon one
 * because xmp readers are as rare as the sun in Scotland.
 */

#ifndef __MLX_XPM_READER__
#define __MLX_XPM_READER__

#include <string>
#include <vector>
#include <cstddef>
#include <map>

namespace mlx
{
	struct XPMData
	{
		XPMData();
	};

	typedef uint32_t Color;
	typedef uint8_t ColorComponent;
	
	Color from_rgb(ColorComponent r, ColorComponent g, ColorComponent b);
	uint8_t get_red(Color color);
	uint8_t get_green(Color color);
	uint8_t get_blue(Color color);
	bool is_transparent(Color color);
	Color rgb_to_argb(Color color);
	
	class Pixmap
	{
		public:
			Pixmap(uint32_t w = 1, uint32_t h = 1, uint32_t palette_size = 1);

			void load(const std::vector<std::string>& xpm_lines);

			Map<uint32_t> pixels;
			std::vector<Color> palette;

		private:
			uint32_t _color_count;
			uint32_t _row_count;
			std::vector<std::string> _interspaces;
			std::vector<std::string> _colors;
			std::vector<std::string> _values_interspaces;
			std::vector<std::pair<std::string, std::pair<std::string, std::string>>> _colors_interspaces;
	};
}

#endif
