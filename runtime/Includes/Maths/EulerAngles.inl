#pragma once
#include <Maths/EulerAngles.h>

namespace mlx
{
	template<typename T>
	constexpr EulerAngles<T>::EulerAngles(DegreeAngle<T> P, DegreeAngle<T> Y, DegreeAngle<T> R) :
		pitch(P), yaw(Y), roll(R)
	{}

	template<typename T>
	constexpr EulerAngles<T>::EulerAngles(const DegreeAngle<T> angles[3]) :
		EulerAngles(angles[0], angles[1], angles[2])
	{}

	template<typename T>
	template<AngleUnit Unit>
	constexpr EulerAngles<T>::EulerAngles(const Angle<Unit, T>& angle) :
		EulerAngles(angle.ToEulerAngles())
	{}

	template<typename T>
	constexpr EulerAngles<T>::EulerAngles(const Quat<T>& quat) :
		EulerAngles(quat.ToEulerAngles())
	{}

	template<typename T>
	template<typename U>
	constexpr EulerAngles<T>::EulerAngles(const EulerAngles<U>& angles) :
		pitch(DegreeAngle<T>(angles.pitch)), yaw(DegreeAngle<T>(angles.yaw)), roll(DegreeAngle<T>(angles.roll))
	{}

	template<typename T>
	constexpr bool EulerAngles<T>::ApproxEqual(const EulerAngles& angles, T maxDifference) const
	{
		return pitch.ApproxEqual(angles.pitch, maxDifference) && yaw.ApproxEqual(angles.yaw, maxDifference) && roll.ApproxEqual(angles.roll, maxDifference);
	}

	template<typename T>
	constexpr EulerAngles<T>& EulerAngles<T>::Normalize()
	{
		pitch.Normalize();
		yaw.Normalize();
		roll.Normalize();
		return *this;
	}

	template<typename T>
	Quat<T> EulerAngles<T>::ToQuat() const
	{
		// XYZ
		auto [s1, c1] = (yaw / T(2.0)).GetSinCos();
		auto [s2, c2] = (roll / T(2.0)).GetSinCos();
		auto [s3, c3] = (pitch / T(2.0)).GetSinCos();

		return Quat<T>(c1 * c2 * c3 - s1 * s2 * s3,
						s1 * s2 * c3 + c1 * c2 * s3,
						s1 * c2 * c3 + c1 * s2 * s3,
						c1 * s2 * c3 - s1 * c2 * s3);
	}

	template<typename T>
	std::string EulerAngles<T>::ToString() const
	{
		std::ostringstream ss;
		ss << *this;
		return ss.str();
	}

	template<typename T>
	constexpr EulerAngles<T> EulerAngles<T>::operator+(const EulerAngles& angles) const
	{
		return EulerAngles(pitch + angles.pitch, yaw + angles.yaw, roll + angles.roll);
	}

	template<typename T>
	constexpr EulerAngles<T> EulerAngles<T>::operator-(const EulerAngles& angles) const
	{
		return EulerAngles(pitch - angles.pitch, yaw - angles.yaw, roll - angles.roll);
	}

	template<typename T>
	constexpr EulerAngles<T>& EulerAngles<T>::operator+=(const EulerAngles& angles)
	{
		pitch += angles.pitch;
		yaw += angles.yaw;
		roll += angles.roll;
		return *this;
	}

	template<typename T>
	constexpr EulerAngles<T>& EulerAngles<T>::operator-=(const EulerAngles& angles)
	{
		pitch -= angles.pitch;
		yaw -= angles.yaw;
		roll -= angles.roll;
		return *this;
	}

	template<typename T>
	constexpr bool EulerAngles<T>::operator==(const EulerAngles& angles) const
	{
		return pitch == angles.pitch && yaw == angles.yaw && roll == angles.roll;
	}

	template<typename T>
	constexpr bool EulerAngles<T>::operator!=(const EulerAngles& angles) const
	{
		return !operator==(angles);
	}

	template<typename T>
	constexpr bool EulerAngles<T>::operator<(const EulerAngles& angles) const
	{
		if (pitch != angles.pitch)
			return pitch < angles.pitch;
		if (yaw != angles.yaw)
			return yaw < angles.yaw;
		return roll < angles.roll;
	}

	template<typename T>
	constexpr bool EulerAngles<T>::operator<=(const EulerAngles& angles) const
	{
		if (pitch != angles.pitch)
			return pitch < angles.pitch;
		if (yaw != angles.yaw)
			return yaw < angles.yaw;
		return roll <= angles.roll;
	}

	template<typename T>
	constexpr bool EulerAngles<T>::operator>(const EulerAngles& angles) const
	{
		if (pitch != angles.pitch)
			return pitch > angles.pitch;
		if (yaw != angles.yaw)
			return yaw > angles.yaw;
		return roll > angles.roll;
	}

	template<typename T>
	constexpr bool EulerAngles<T>::operator>=(const EulerAngles& angles) const
	{
		if (pitch != angles.pitch)
			return pitch > angles.pitch;
		if (yaw != angles.yaw)
			return yaw > angles.yaw;
		return roll >= angles.roll;
	}

	template<typename T>
	constexpr bool EulerAngles<T>::ApproxEqual(const EulerAngles& lhs, const EulerAngles& rhs, T maxDifference)
	{
		return lhs.ApproxEqual(rhs, maxDifference);
	}

	template<typename T>
	constexpr EulerAngles<T> EulerAngles<T>::Zero()
	{
		return EulerAngles(0, 0, 0);
	}

	template<typename T>
	std::ostream& operator<<(std::ostream& out, const EulerAngles<T>& angles)
	{
		return out << "EulerAngles(" << angles.pitch << ", " << angles.yaw << ", " << angles.roll << ')';
	}
}
