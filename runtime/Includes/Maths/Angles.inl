#pragma once
#include <Maths/Angles.h>

#include <Maths/Constants.h>
#include <Maths/MathsUtils.h>

namespace mlx
{
	namespace Internal
	{
		template<AngleUnit From, AngleUnit To> struct AngleConversion;

		template<AngleUnit Unit>
		struct AngleConversion<Unit, Unit>
		{
			template<typename T>
			static constexpr T Convert(T angle)
			{
				return angle;
			}
		};

		template<>
		struct AngleConversion<AngleUnit::Degree, AngleUnit::Radian>
		{
			template<typename T>
			static constexpr T Convert(T angle)
			{
				return DegreeToRadian(angle);
			}
		};

		template<>
		struct AngleConversion<AngleUnit::Degree, AngleUnit::Turn>
		{
			template<typename T>
			static constexpr T Convert(T angle)
			{
				return angle / T(360);
			}
		};

		template<>
		struct AngleConversion<AngleUnit::Radian, AngleUnit::Degree>
		{
			template<typename T>
			static constexpr T Convert(T angle)
			{
				return RadianToDegree(angle);
			}
		};

		template<>
		struct AngleConversion<AngleUnit::Radian, AngleUnit::Turn>
		{
			template<typename T>
			static constexpr T Convert(T angle)
			{
				return angle / Tau<T>();
			}
		};

		template<>
		struct AngleConversion<AngleUnit::Turn, AngleUnit::Degree>
		{
			template<typename T>
			static constexpr T Convert(T angle)
			{
				return angle * T(360);
			}
		};

		template<>
		struct AngleConversion<AngleUnit::Turn, AngleUnit::Radian>
		{
			template<typename T>
			static constexpr T Convert(T angle)
			{
				return angle * Tau<T>();
			}
		};

		template<AngleUnit Unit> struct AngleUtils;

		template<>
		struct AngleUtils<AngleUnit::Degree>
		{
			template<typename T>
			static constexpr T GetEpsilon()
			{
				return T(1e-4);
			}

			template<typename T>
			static constexpr T GetLimit()
			{
				return 360;
			}

			template<typename T> static std::ostream& ToString(std::ostream& out, T value)
			{
				return out << "Angle(" << value << "deg)";
			}
		};

		template<>
		struct AngleUtils<AngleUnit::Radian>
		{
			template<typename T>
			static constexpr T GetEpsilon()
			{
				return T(1e-5);
			}

			template<typename T>
			static constexpr T GetLimit()
			{
				return Tau<T>();
			}

			template<typename T>
			static std::ostream& ToString(std::ostream& out, T value)
			{
				return out << "Angle(" << value << "rad)";
			}
		};

		template<>
		struct AngleUtils<AngleUnit::Turn>
		{
			template<typename T>
			static constexpr T GetEpsilon()
			{
				return T(1e-5);
			}

			template<typename T>
			static constexpr T GetLimit()
			{
				return 1;
			}

			template<typename T>
			static std::ostream& ToString(std::ostream& out, T value)
			{
				return out << "Angle(" << value << "turn)";
			}
		};

		#ifdef MLX_PLAT_LINUX
			template<typename T>
			void SinCos(T x, T* sin, T* cos)
			{
				double s, c;
				::sincos(x, &s, &c);
				*sin = static_cast<T>(s);
				*cos = static_cast<T>(c);
			}

			template<>
			inline void SinCos(float x, float* s, float* c)
			{
				::sincosf(x, s, c);
			}

			template<>
			inline void SinCos(long double x, long double* s, long double* c)
			{
				::sincosl(x, s, c);
			}
		#else
			template<typename T>
			void SinCos(T x, T* sin, T* cos)
			{
				*sin = std::sin(x);
				*cos = std::cos(x);
			}
		#endif
	}

	template<AngleUnit Unit, typename T>
	constexpr Angle<Unit, T>::Angle(T angle) :
	value(angle)
	{
	}

	template<AngleUnit Unit, typename T>
	template<typename U>
	constexpr Angle<Unit, T>::Angle(const Angle<Unit, U>& angle) :
	value(static_cast<T>(angle.value))
	{
	}

	template<AngleUnit Unit, typename T>
	template<AngleUnit FromUnit>
	constexpr Angle<Unit, T>::Angle(const Angle<FromUnit, T>& angle) :
	value(Internal::AngleConversion<FromUnit, Unit>::Convert(angle.value))
	{
	}

	template<AngleUnit Unit, typename T>
	constexpr bool Angle<Unit, T>::ApproxEqual(const Angle& angle) const
	{
		return ApproxEqual(angle, Internal::AngleUtils<Unit>::template GetEpsilon<T>());
	}

	template<AngleUnit Unit, typename T>
	constexpr bool Angle<Unit, T>::ApproxEqual(const Angle& angle, T maxDifference) const
	{
		return NumberEquals(value, angle.value, maxDifference);
	}

	template<AngleUnit Unit, typename T>
	T Angle<Unit, T>::GetCos() const
	{
		return std::cos(ToRadians());
	}

	template<AngleUnit Unit, typename T>
	T Angle<Unit, T>::GetSin() const
	{
		return std::sin(ToRadians());
	}

	template<AngleUnit Unit, typename T>
	std::pair<T, T> Angle<Unit, T>::GetSinCos() const
	{
		T sin, cos;
		Internal::SinCos<T>(ToRadians(), &sin, &cos);

		return std::make_pair(sin, cos);
	}

	template<AngleUnit Unit, typename T>
	T Angle<Unit, T>::GetTan() const
	{
		return std::tan(ToRadians());
	}

	template<AngleUnit Unit, typename T>
	constexpr Angle<Unit, T>& Angle<Unit, T>::Normalize()
	{
		constexpr T limit = Internal::AngleUtils<Unit>::template GetLimit<T>();
		constexpr T halfLimit = limit / T(2);

		value = Mod(value + halfLimit, limit);
		if (value < T(0))
			value += limit;

		value -= halfLimit;
		return *this;
	}

	template<AngleUnit Unit, typename T>
	template<AngleUnit ToUnit>
	T Angle<Unit, T>::To() const
	{
		return Internal::AngleConversion<Unit, ToUnit>::Convert(value);
	}

	template<AngleUnit Unit, typename T>
	template<AngleUnit ToUnit>
	Angle<ToUnit, T> Angle<Unit, T>::ToAngle() const
	{
		return Angle<ToUnit, T>(To<ToUnit>());
	}

	template<AngleUnit Unit, typename T>
	constexpr T Angle<Unit, T>::ToDegrees() const
	{
		return To<AngleUnit::Degree>();
	}

	template<AngleUnit Unit, typename T>
	constexpr Angle<AngleUnit::Degree, T> Angle<Unit, T>::ToDegreeAngle() const
	{
		return ToAngle<AngleUnit::Degree>();
	}

	template<AngleUnit Unit, typename T>
	EulerAngles<T> Angle<Unit, T>::ToEulerAngles() const
	{
		return EulerAngles<T>(0, 0, ToDegrees());
	}

	template<AngleUnit Unit, typename T>
	Quat<T> Angle<Unit, T>::ToQuat() const
	{
		auto halfAngle = Angle(*this) / 2.f;
		auto sincos = halfAngle.GetSinCos();
		return Quat<T>(sincos.second, 0, 0, sincos.first);
	}

	template<AngleUnit Unit, typename T>
	constexpr T Angle<Unit, T>::ToRadians() const
	{
		return To<AngleUnit::Radian>();
	}

	template<AngleUnit Unit, typename T>
	constexpr Angle<AngleUnit::Radian, T> Angle<Unit, T>::ToRadianAngle() const
	{
		return ToAngle<AngleUnit::Radian>();
	}

	template<AngleUnit Unit, typename T>
	std::string Angle<Unit, T>::ToString() const
	{
		std::ostringstream oss;
		Internal::AngleUtils<Unit>::ToString(oss, value);

		return oss.str();
	}

	template<AngleUnit Unit, typename T>
	constexpr T Angle<Unit, T>::ToTurns() const
	{
		return To<AngleUnit::Turn>(value);
	}

	template<AngleUnit Unit, typename T>
	constexpr Angle<AngleUnit::Turn, T> Angle<Unit, T>::ToTurnAngle() const
	{
		return ToAngle<AngleUnit::Turn>();
	}

	template<AngleUnit Unit, typename T>
	constexpr Angle<Unit, T> Angle<Unit, T>::operator+() const
	{
		return *this;
	}

	template<AngleUnit Unit, typename T>
	constexpr Angle<Unit, T> Angle<Unit, T>::operator-() const
	{
		return Angle(-value);
	}

	template<AngleUnit Unit, typename T>
	constexpr Angle<Unit, T> Angle<Unit, T>::operator+(Angle other) const
	{
		return Angle(value + other.value);
	}

	template<AngleUnit Unit, typename T>
	constexpr Angle<Unit, T> Angle<Unit, T>::operator-(Angle other) const
	{
		return Angle(value - other.value);
	}

	template<AngleUnit Unit, typename T>
	constexpr Angle<Unit, T> Angle<Unit, T>::operator*(T scalar) const
	{
		return Angle(value * scalar);
	}

	template<AngleUnit Unit, typename T>
	constexpr Angle<Unit, T> Angle<Unit, T>::operator/(T divider) const
	{
		return Angle(value / divider);
	}

	template<AngleUnit Unit, typename T>
	constexpr Angle<Unit, T>& Angle<Unit, T>::operator+=(Angle other)
	{
		value += other.value;
		return *this;
	}

	template<AngleUnit Unit, typename T>
	constexpr Angle<Unit, T>& Angle<Unit, T>::operator-=(Angle other)
	{
		value -= other.value;
		return *this;
	}

	template<AngleUnit Unit, typename T>
	constexpr Angle<Unit, T>& Angle<Unit, T>::operator*=(T scalar)
	{
		value *= scalar;
		return *this;
	}

	template<AngleUnit Unit, typename T>
	constexpr Angle<Unit, T>& Angle<Unit, T>::operator/=(T divider)
	{
		value /= divider;
		return *this;
	}

	template<AngleUnit Unit, typename T>
	constexpr bool Angle<Unit, T>::operator==(Angle other) const
	{
		return value == other.value;
	}

	template<AngleUnit Unit, typename T>
	constexpr bool Angle<Unit, T>::operator!=(Angle other) const
	{
		return value != other.value;
	}

	template<AngleUnit Unit, typename T>
	constexpr bool Angle<Unit, T>::operator<(Angle other) const
	{
		return value < other.value;
	}

	template<AngleUnit Unit, typename T>
	constexpr bool Angle<Unit, T>::operator<=(Angle other) const
	{
		return value <= other.value;
	}

	template<AngleUnit Unit, typename T>
	constexpr bool Angle<Unit, T>::operator>(Angle other) const
	{
		return value > other.value;
	}

	template<AngleUnit Unit, typename T>
	constexpr bool Angle<Unit, T>::operator>=(Angle other) const
	{
		return value >= other.value;
	}

	template<AngleUnit Unit, typename T>
	constexpr bool Angle<Unit, T>::ApproxEqual(const Angle& lhs, const Angle& rhs)
	{
		return lhs.ApproxEqual(rhs);
	}

	template<AngleUnit Unit, typename T>
	constexpr bool Angle<Unit, T>::ApproxEqual(const Angle& lhs, const Angle& rhs, T maxDifference)
	{
		return lhs.ApproxEqual(rhs, maxDifference);
	}

	template<AngleUnit Unit, typename T>
	constexpr Angle<Unit, T> Angle<Unit, T>::Clamp(Angle angle, Angle min, Angle max)
	{
		return Angle(std::clamp(angle.value, min.value, max.value));
	}

	template<AngleUnit Unit, typename T>
	template<AngleUnit FromUnit>
	constexpr Angle<Unit, T> Angle<Unit, T>::From(T value)
	{
		return Angle(Internal::AngleConversion<FromUnit, Unit>::Convert(value));
	}

	template<AngleUnit Unit, typename T>
	constexpr Angle<Unit, T> Angle<Unit, T>::FromDegrees(T degrees)
	{
		return From<AngleUnit::Degree>(degrees);
	}

	template<AngleUnit Unit, typename T>
	constexpr Angle<Unit, T> Angle<Unit, T>::FromRadians(T radians)
	{
		return From<AngleUnit::Radian>(radians);
	}

	template<AngleUnit Unit, typename T>
	constexpr Angle<Unit, T> Angle<Unit, T>::FromTurns(T turns)
	{
		return From<AngleUnit::Turn>(turns);
	}

	template<AngleUnit Unit, typename T>
	constexpr Angle<Unit, T> Angle<Unit, T>::Zero()
	{
		return Angle(0);
	}

	template<AngleUnit Unit, typename T>
	Angle<Unit, T> operator/(T scale, Angle<Unit, T> angle)
	{
		return Angle<Unit, T>(scale / angle.value);
	}

	template<AngleUnit Unit, typename T>
	std::ostream& operator<<(std::ostream& out, Angle<Unit, T> angle)
	{
		return Internal::AngleUtils<Unit>::ToString(out, angle.value);
	}

	template<typename T, AngleUnit Unit>
	constexpr Angle<Unit, T> Clamp(Angle<Unit, T> value, T min, T max)
	{
		return std::max(std::min(value.value, max), min);
	}
}
