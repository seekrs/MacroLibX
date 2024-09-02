#ifndef __MLX_MATHS_ENUMS__
#define __MLX_MATHS_ENUMS__

namespace mlx
{
	enum class AngleUnit
	{
		Degree = 0,
		Radian,
		Turn,

		EndEnum
	};

	constexpr std::size_t AngleUnitCount = static_cast<std::size_t>(AngleUnit::EndEnum);
}

#endif
