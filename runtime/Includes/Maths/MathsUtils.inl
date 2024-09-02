#pragma once
#include <Maths/MathsUtils.h>

#include <Maths/Constants.h>

namespace mlx
{
	template<typename T>
	[[nodiscard]] constexpr T Mod(T x, T y) noexcept
	{
		if constexpr(std::is_floating_point_v<T>)
		{
			if(!std::is_constant_evaluated())
				return x - static_cast<long long>(x / y) * y;
			else
				return std::fmod(x, y);
		}
		return x % y;
	}

	template<std::floating_point T>
	[[nodiscard]] constexpr T DegreeToRadian(T degrees) noexcept
	{
		return degrees * (Pi<T>() / T(180.0));
	}

	template<std::floating_point T>
	[[nodiscard]] constexpr T RadianToDegree(T radians) noexcept
	{
		return radians * (T(180.0) / Pi<T>());
	}

	template<typename T>
	[[nodiscard]] constexpr T Clamp(T value, T min, T max) noexcept
	{
		return std::max(std::min(value, max), min);
	}

	template<typename T, typename T2>
	[[nodiscard]] constexpr T Lerp(const T& from, const T& to, const T2& interpolation) noexcept
	{
		return static_cast<T>(from + interpolation * (to - from));
	}
}
