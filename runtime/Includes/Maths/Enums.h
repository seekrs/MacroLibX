#ifndef __SCOPE_MATHS_ENUMS__
#define __SCOPE_MATHS_ENUMS__

#include <cstddef>

namespace Scop
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
