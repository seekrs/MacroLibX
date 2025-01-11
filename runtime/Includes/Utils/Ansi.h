#ifndef __MLX_ANSI__
#define __MLX_ANSI__

#include <ostream>
#include <cstdint>

namespace mlx
{
	enum class Ansi : std::uint32_t
	{
		red = 31,
		green = 32,
		blue = 34,
		def = 0,
		black = 30,
		yellow = 33,
		magenta = 35,
		cyan = 36,
		white = 37,
		bg_red = 41,
		bg_green = 42,
		bg_blue = 44,
		bg_def = 0,
		bg_black = 40,
		bg_yellow = 43,
		bg_magenta = 45,
		bg_cyan = 46,
		bg_white = 47,
		reset = 0,
		bold = 1,
		underline = 4,
		inverse = 7,
		bold_off = 21,
		underline_off = 24,
		inverse_off = 27
	};

	inline std::ostream& operator<<(std::ostream& os, Ansi ansi)
	{
		return os << "\033[1;" << std::to_string(static_cast<std::uint32_t>(ansi)) << "m";
	}
}

#endif
