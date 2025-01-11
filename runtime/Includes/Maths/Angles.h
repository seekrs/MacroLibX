#ifndef __MLX_ANGLES__
#define __MLX_ANGLES__

#include <Maths/Enums.h>

namespace mlx
{
	template<typename T> struct EulerAngles;
	template<typename T> struct Quat;

	template<AngleUnit Unit, typename T>
	struct Angle
	{
		T value;

		constexpr Angle() = default;
		constexpr Angle(T angle);
		template<typename U> constexpr explicit Angle(const Angle<Unit, U>& Angle);
		template<AngleUnit FromUnit> constexpr Angle(const Angle<FromUnit, T>& angle);
		constexpr Angle(const Angle&) = default;
		constexpr Angle(Angle&&) noexcept = default;
		~Angle() = default;

		constexpr bool ApproxEqual(const Angle& angle) const;
		constexpr bool ApproxEqual(const Angle& angle, T max_difference) const;

		T GetCos() const;
		T GetSin() const;
		std::pair<T, T> GetSinCos() const;
		T GetTan() const;

		constexpr Angle& Normalize();

		template<AngleUnit ToUnit> T To() const;
		template<AngleUnit ToUnit> Angle<ToUnit, T> ToAngle() const;
		constexpr T ToDegrees() const;
		constexpr Angle<AngleUnit::Degree, T> ToDegreeAngle() const;
		EulerAngles<T> ToEulerAngles() const;
		Quat<T> ToQuat() const;
		constexpr T ToRadians() const;
		constexpr Angle<AngleUnit::Radian, T> ToRadianAngle() const;
		std::string ToString() const;
		constexpr T ToTurns() const;
		constexpr Angle<AngleUnit::Turn, T> ToTurnAngle() const;

		constexpr Angle& operator=(const Angle&) = default;
		constexpr Angle& operator=(Angle&&) noexcept = default;

		constexpr Angle operator+() const;
		constexpr Angle operator-() const;

		constexpr Angle operator+(Angle other) const;
		constexpr Angle operator-(Angle other) const;
		constexpr Angle operator*(T scalar) const;
		constexpr Angle operator/(T divider) const;

		constexpr Angle& operator+=(Angle other);
		constexpr Angle& operator-=(Angle other);
		constexpr Angle& operator*=(T scalar);
		constexpr Angle& operator/=(T divider);

		constexpr bool operator==(Angle other) const;
		constexpr bool operator!=(Angle other) const;
		constexpr bool operator<(Angle other) const;
		constexpr bool operator<=(Angle other) const;
		constexpr bool operator>(Angle other) const;
		constexpr bool operator>=(Angle other) const;

		static constexpr bool ApproxEqual(const Angle& lhs, const Angle& rhs);
		static constexpr bool ApproxEqual(const Angle& lhs, const Angle& rhs, T max_difference);
		static constexpr Angle Clamp(Angle angle, Angle min, Angle max);
		template<AngleUnit FromUnit> static constexpr Angle From(T value);
		static constexpr Angle FromDegrees(T degrees);
		static constexpr Angle FromRadians(T radians);
		static constexpr Angle FromTurns(T turn);
		static constexpr Angle Zero();
	};

	template<typename T>
	using DegreeAngle = Angle<AngleUnit::Degree, T>;

	using DegreeAngled = DegreeAngle<double>;
	using DegreeAnglef = DegreeAngle<float>;

	template<typename T>
	using RadianAngle = Angle<AngleUnit::Radian, T>;

	using RadianAngled = RadianAngle<double>;
	using RadianAnglef = RadianAngle<float>;

	template<typename T>
	using TurnAngle = Angle<AngleUnit::Turn, T>;

	using TurnAngled = TurnAngle<double>;
	using TurnAnglef = TurnAngle<float>;

	template<AngleUnit Unit, typename T> Angle<Unit, T> operator*(T scale, Angle<Unit, T> angle);

	template<AngleUnit Unit, typename T> Angle<Unit, T> operator/(T divider, Angle<Unit, T> angle);

	template<AngleUnit Unit, typename T> std::ostream& operator<<(std::ostream& out, Angle<Unit, T> angle);
}

#include <Maths/Angles.inl>

#endif
