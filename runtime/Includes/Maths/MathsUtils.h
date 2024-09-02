#ifndef __MLX_MATHS_UTILS__
#define __MLX_MATHS_UTILS__

namespace mlx
{
	template<typename T>
	[[nodiscard]] constexpr T Mod(T x, T y) noexcept;

	template<std::floating_point T>
	[[nodiscard]] constexpr T DegreeToRadian(T degrees) noexcept;
	
	template<std::floating_point T>
	[[nodiscard]] constexpr T RadianToDegree(T radians) noexcept;

	template<typename T>
	[[nodiscard]] constexpr T Clamp(T value, T min, T max) noexcept;
	
	template<typename T, typename T2>
	[[nodiscard]] constexpr T Lerp(const T& from, const T& to, const T2& interpolation) noexcept;
}

#include <Maths/MathsUtils.inl>

#endif
