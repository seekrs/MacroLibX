#ifndef __MLX_QUATERNIONS__
#define __MLX_QUATERNIONS__

#include <Maths/Angles.h>
#include <Maths/Vec3.h>

namespace mlx
{
	template<typename T>
	struct Quat
	{
		T w, x, y, z;

		constexpr Quat() = default;
		constexpr Quat(T W, T X, T Y, T Z);
		template<AngleUnit Unit> Quat(const Angle<Unit, T>& angle);
		Quat(const EulerAngles<T>& angles);
		constexpr Quat(RadianAngle<T> angle, const Vec3<T>& axis);
		constexpr Quat(const T quat[4]);
		template<typename U> constexpr explicit Quat(const Quat<U>& quat);
		constexpr Quat(const Quat&) = default;
		constexpr Quat(Quat&&) = default;

		RadianAngle<T> AngleBetween(const Quat& vec) const;
		constexpr bool ApproxEqual(const Quat& quat, T max_difference = std::numeric_limits<T>::epsilon()) const;

		Quat& ComputeW();
		constexpr Quat& Conjugate();

		constexpr T DotProduct(const Quat& vec) const;

		constexpr Quat GetConjugate() const;
		Quat GetInverse() const;
		Quat GetNormal(T* length = nullptr) const;

		Quat& Inverse();

		T Magnitude() const;

		Quat& Normalize(T* length = nullptr);

		constexpr T SquaredMagnitude() const;

		RadianAngle<T> To2DAngle() const;
		EulerAngles<T> ToEulerAngles() const;
		std::string ToString() const;

		constexpr Quat& operator=(const Quat& quat) = default;
		constexpr Quat& operator=(Quat&&) = default;

		constexpr Quat operator+(const Quat& quat) const;
		constexpr Quat operator*(const Quat& quat) const;
		constexpr Vec3<T> operator*(const Vec3<T>& vec) const;
		constexpr Quat operator*(T scale) const;
		constexpr Quat operator/(const Quat& quat) const;

		constexpr Quat& operator+=(const Quat& quat);
		constexpr Quat& operator*=(const Quat& quat);
		constexpr Quat& operator*=(T scale);
		constexpr Quat& operator/=(const Quat& quat);

		constexpr bool operator==(const Quat& quat) const;
		constexpr bool operator!=(const Quat& quat) const;
		constexpr bool operator<(const Quat& quat) const;
		constexpr bool operator<=(const Quat& quat) const;
		constexpr bool operator>(const Quat& quat) const;
		constexpr bool operator>=(const Quat& quat) const;

		static RadianAngle<T> AngleBetween(const Quat& lhs, const Quat& rhs);
		static constexpr bool ApproxEqual(const Quat& lhs, const Quat& rhs, T max_difference = std::numeric_limits<T>::epsilon());
		static constexpr Quat Identity();
		static constexpr Quat Lerp(const Quat& from, const Quat& to, T interpolation);
		static Quat LookAt(const Vec3<T>& forward, const Vec3<T>& up);
		static Quat Normalize(const Quat& quat, T* length = nullptr);
		static Quat RotationBetween(const Vec3<T>& from, const Vec3<T>& to);
		static Quat RotateTowards(const Quat& from, const Quat& to, RadianAngle<T> max_rotation);
		static Quat Mirror(Quat quat, const Vec3<T>& axis);
		static Quat Slerp(const Quat& from, const Quat& to, T interpolation);
		static constexpr Quat Zero();

		~Quat() = default;
	};

	using Quatd = Quat<double>;
	using Quatf = Quat<float>;

	template<typename T> std::ostream& operator<<(std::ostream& out, const Quat<T>& quat);
}

#include <Maths/Quaternions.inl>

#endif
