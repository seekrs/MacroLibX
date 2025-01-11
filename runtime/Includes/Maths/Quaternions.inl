#pragma once
#include <Maths/Quaternions.h>

namespace mlx
{
	template<typename T>
	constexpr Quat<T>::Quat(T W, T X, T Y, T Z) : w(W), x(X), y(Y), z(Z)
	{}

	template<typename T>
	template<AngleUnit Unit>
	Quat<T>::Quat(const Angle<Unit, T>& angle) : Quat(angle.ToQuat())
	{}

	template<typename T>
	Quat<T>::Quat(const EulerAngles<T>& angles) : Quat(angles.ToQuat())
	{}

	template<typename T>
	constexpr Quat<T>::Quat(RadianAngle<T> angle, const Vec3<T>& axis)
	{
		angle /= T(2.0);

		Vec3<T> normalizedAxis = axis.GetNormal();

		auto sincos = angle.GetSinCos();

		w = sincos.second;
		x = normalizedAxis.x * sincos.first;
		y = normalizedAxis.y * sincos.first;
		z = normalizedAxis.z * sincos.first;

		Normalize();
	}

	template<typename T>
	constexpr Quat<T>::Quat(const T quat[4]) : w(quat[0]), x(quat[1]), y(quat[2]), z(quat[3])
	{}

	template<typename T>
	template<typename U>
	constexpr Quat<T>::Quat(const Quat<U>& quat) : w(static_cast<T>(quat.w)), x(static_cast<T>(quat.x)), y(static_cast<T>(quat.y)), z(static_cast<T>(quat.z))
	{}

	template<typename T>
	RadianAngle<T> Quat<T>::AngleBetween(const Quat& quat) const
	{
		T alpha = Vec3<T>::DotProduct(Vec3<T>(x, y, z), Vec3<T>(quat.x, quat.y, quat.z));
		return std::acos(mlx::Clamp(alpha, T(-1.0), T(1.0)));
	}

	template<typename T>
	constexpr bool Quat<T>::ApproxEqual(const Quat& quat, T maxDifference) const
	{
		return NumberEquals(w, quat.w, maxDifference) &&
				NumberEquals(x, quat.x, maxDifference) &&
				NumberEquals(y, quat.y, maxDifference) &&
				NumberEquals(z, quat.z, maxDifference);
	}

	template<typename T>
	Quat<T>& Quat<T>::ComputeW()
	{
		T t = T(1.0) - SquaredMagnitude();

		if(t < T(0.0))
			w = T(0.0);
		else
			w = -std::sqrt(t);

		return *this;
	}

	template<typename T>
	constexpr Quat<T>& Quat<T>::Conjugate()
	{
		x = -x;
		y = -y;
		z = -z;
		return *this;
	}

	template<typename T>
	constexpr T Quat<T>::DotProduct(const Quat& quat) const
	{
		return w * quat.w + x * quat.x + y * quat.y + z * quat.z;
	}

	template<typename T>
	constexpr Quat<T> Quat<T>::GetConjugate() const
	{
		Quat<T> quat(*this);
		quat.Conjugate();
		return quat;
	}

	template<typename T>
	Quat<T> Quat<T>::GetInverse() const
	{
		Quat<T> quat(*this);
		quat.Inverse();
		return quat;
	}

	template<typename T>
	Quat<T> Quat<T>::GetNormal(T* length) const
	{
		Quat<T> quat(*this);
		quat.Normalize(length);
		return quat;
	}

	template<typename T>
	Quat<T>& Quat<T>::Inverse()
	{
		T norm = SquaredMagnitude();
		if(norm > T(0.0))
		{
			T invNorm = T(1.0) / std::sqrt(norm);

			w *= invNorm;
			x *= -invNorm;
			y *= -invNorm;
			z *= -invNorm;
		}

		return *this;
	}

	template<typename T>
	T Quat<T>::Magnitude() const
	{
		return std::sqrt(SquaredMagnitude());
	}

	template<typename T>
	Quat<T>& Quat<T>::Normalize(T* length)
	{
		T norm = std::sqrt(SquaredMagnitude());
		if(norm > T(0.0))
		{
			T invNorm = T(1.0) / norm;
			w *= invNorm;
			x *= invNorm;
			y *= invNorm;
			z *= invNorm;
		}

		if(length)
			*length = norm;

		return *this;
	}

	template<typename T>
	constexpr T Quat<T>::SquaredMagnitude() const
	{
		return w * w + x * x + y * y + z * z;
	}

	template<typename T>
	RadianAngle<T> Quat<T>::To2DAngle() const
	{
		T siny_cosp = T(2.0) * (w * z + x * y);
		T cosy_cosp = T(1.0) - T(2.0) * (y * y + z * z);

		return std::atan2(siny_cosp, cosy_cosp);
	}

	template<typename T>
	EulerAngles<T> Quat<T>::ToEulerAngles() const
	{
		T test = x * y + z * w;
		if(test > T(0.499))
			// singularity at north pole
			return EulerAngles<T>(DegreeAngle<T>(T(0.0)), RadianAngle<T>(T(2.0) * std::atan2(x, w)), DegreeAngle<T>(T(90.0)));

		if(test < T(-0.499))
			// singularity at south pole
			return EulerAngles<T>(DegreeAngle<T>(T(0.0)), RadianAngle<T>(T(-2.0) * std::atan2(x, w)), DegreeAngle<T>(T(-90.0)));

		return EulerAngles<T>(RadianAngle<T>(std::atan2(T(2.0) * x * w - T(2.0) * y * z, T(1.0) - T(2.0) * x * x - T(2.0) * z * z)),
								RadianAngle<T>(std::atan2(T(2.0) * y * w - T(2.0) * x * z, T(1.0) - T(2.0) * y * y - T(2.0) * z * z)),
								RadianAngle<T>(std::asin(T(2.0) * test)));
	}

	template<typename T>
	std::string Quat<T>::ToString() const
	{
		std::ostringstream ss;
		ss << *this;

		return ss.str();
	}

	template<typename T>
	constexpr Quat<T> Quat<T>::operator+(const Quat& quat) const
	{
		Quat result;
		result.w = w + quat.w;
		result.x = x + quat.x;
		result.y = y + quat.y;
		result.z = z + quat.z;

		return result;
	}

	template<typename T>
	constexpr Quat<T> Quat<T>::operator*(const Quat& quat) const
	{
		Quat result;
		result.w = w * quat.w - x * quat.x - y * quat.y - z * quat.z;
		result.x = w * quat.x + x * quat.w + y * quat.z - z * quat.y;
		result.y = w * quat.y + y * quat.w + z * quat.x - x * quat.z;
		result.z = w * quat.z + z * quat.w + x * quat.y - y * quat.x;

		return result;
	}

	template<typename T>
	constexpr Vec3<T> Quat<T>::operator*(const Vec3<T>& vec) const
	{
		Vec3<T> quatVec(x, y, z);
		Vec3<T> uv = quatVec.CrossProduct(vec);
		Vec3<T> uuv = quatVec.CrossProduct(uv);
		uv *= T(2.0) * w;
		uuv *= T(2.0);

		return vec + uv + uuv;
	}

	template<typename T>
	constexpr Quat<T> Quat<T>::operator*(T scale) const
	{
		return Quat(w * scale,
					x * scale,
					y * scale,
					z * scale);
	}

	template<typename T>
	constexpr Quat<T> Quat<T>::operator/(const Quat& quat) const
	{
		return quat.GetConjugate() * (*this);
	}

	template<typename T>
	constexpr Quat<T>& Quat<T>::operator+=(const Quat& quat)
	{
		return operator=(operator+(quat));
	}

	template<typename T>
	constexpr Quat<T>& Quat<T>::operator*=(const Quat& quat)
	{
		return operator=(operator*(quat));
	}

	template<typename T>
	constexpr Quat<T>& Quat<T>::operator*=(T scale)
	{
		return operator=(operator*(scale));
	}

	template<typename T>
	constexpr Quat<T>& Quat<T>::operator/=(const Quat& quat)
	{
		return operator=(operator/(quat));
	}
	
	template<typename T>
	constexpr bool Quat<T>::operator==(const Quat& quat) const
	{
		return w == quat.w && x == quat.x && y == quat.y && z == quat.z;
	}

	template<typename T>
	constexpr bool Quat<T>::operator!=(const Quat& quat) const
	{
		return !operator==(quat);
	}

	template<typename T>
	constexpr bool Quat<T>::operator<(const Quat& quat) const
	{
		if(w != quat.w)
			return w < quat.w;

		if(x != quat.x)
			return x < quat.x;

		if(y != quat.y)
			return y < quat.y;

		if(z != quat.z)
			return z < quat.z;
	}

	template<typename T>
	constexpr bool Quat<T>::operator<=(const Quat& quat) const
	{
		if(w != quat.w)
			return w < quat.w;

		if(x != quat.x)
			return x < quat.x;

		if(y != quat.y)
			return y < quat.y;

		if(z != quat.z)
			return z <= quat.z;
	}

	template<typename T>
	constexpr bool Quat<T>::operator>(const Quat& quat) const
	{
		if(w != quat.w)
			return w > quat.w;

		if(x != quat.x)
			return x > quat.x;

		if(y != quat.y)
			return y > quat.y;

		if(z != quat.z)
			return z > quat.z;
	}

	template<typename T>
	constexpr bool Quat<T>::operator>=(const Quat& quat) const
	{
		if(w != quat.w)
			return w > quat.w;

		if(x != quat.x)
			return x > quat.x;

		if(y != quat.y)
			return y > quat.y;

		if(z != quat.z)
			return z >= quat.z;
	}

	template<typename T>
	RadianAngle<T> Quat<T>::AngleBetween(const Quat& lhs, const Quat& rhs)
	{
		return lhs.AngleBetween(rhs);
	}

	template<typename T>
	constexpr bool Quat<T>::ApproxEqual(const Quat& lhs, const Quat& rhs, T maxDifference)
	{
		return lhs.ApproxEqual(rhs, maxDifference);
	}

	template<typename T>
	constexpr Quat<T> Quat<T>::Identity()
	{
		return Quat(1, 0, 0, 0);
	}

	template<typename T>
	constexpr Quat<T> Quat<T>::Lerp(const Quat& from, const Quat& to, T interpolation)
	{
		Quat interpolated;
		interpolated.w = mlx::Lerp(from.w, to.w, interpolation);
		interpolated.x = mlx::Lerp(from.x, to.x, interpolation);
		interpolated.y = mlx::Lerp(from.y, to.y, interpolation);
		interpolated.z = mlx::Lerp(from.z, to.z, interpolation);

		return interpolated;
	}

	template<typename T>
	Quat<T> Quat<T>::LookAt(const Vec3<T>& forward, const Vec3<T>& up)
	{
		// From https://gamedev.stackexchange.com/questions/53129/quaternion-look-at-with-up-vector
		Vec3<T> forward_w = Vec3<T>::Forward();
		Vec3<T> axis = Vec3<T>::CrossProduct(forward, forward_w);
		RadianAngle<T> angle = std::acos(Vec3<T>::DotProduct(forward, forward_w));

		Vec3<T> third = Vec3<T>::CrossProduct(axis, forward_w);
		if(Vec3<T>::DotProduct(third, forward) < 0)
			angle = -angle;

		Quat<T> q1 = Quat(angle, axis);

		Vec3<T> up_l = q1 * up;
		Vec3<T> right = Vec3<T>::Normalize(Vec3<T>::CrossProduct(forward, up));
		Vec3<T> up_w = Vec3<T>::Normalize(Vec3<T>::CrossProduct(right, forward));

		Vec3<T> axis2 = Vec3<T>::CrossProduct(up_l, up_w);
		RadianAngle<T> angle2 = std::acos(Vec3<T>::DotProduct(forward, forward_w));

		Quat<T> q2 = Quat(angle2, axis2);

		return q2 * q1;
	}

	template<typename T>
	Quat<T> Quat<T>::Normalize(const Quat& quat, T* length)
	{
		return quat.GetNormal(length);
	}

	template<typename T>
	Quat<T> Quat<T>::RotationBetween(const Vec3<T>& from, const Vec3<T>& to)
	{
		T dot = from.DotProduct(to);
		if(dot < T(-0.999999))
		{
			Vec3<T> crossProduct;
			if(from.DotProduct(Vec3<T>::UnitX()) < T(0.999999))
				crossProduct = Vec3<T>::UnitX().CrossProduct(from);
			else
				crossProduct = Vec3<T>::UnitY().CrossProduct(from);

			crossProduct.Normalize();
			return Quat(Pi<T>(), crossProduct);
		}
		else if(dot > T(0.999999))
			return Quat::Identity();
		else
		{
			T norm = std::sqrt(from.GetSquaredLength() * to.GetSquaredLength());
			Vec3<T> crossProduct = from.CrossProduct(to);

			return Quat(norm + dot, crossProduct.x, crossProduct.y, crossProduct.z).GetNormal();
		}
	}

	template<typename T>
	Quat<T> Quat<T>::RotateTowards(const Quat& from, const Quat& to, RadianAngle<T> maxRotation)
	{
		RadianAngle<T> rotationBetween = AngleBetween(from, to);
		if(rotationBetween < maxRotation)
			return to;

		return Slerp(from, to, std::min(maxRotation.value / rotationBetween.value), 1.f);
	}

	template<typename T>
	Quat<T> Quat<T>::Mirror(Quat quat, const Vec3<T>& axis)
	{
		T x = std::copysign(T(1.0), axis.x);
		T y = std::copysign(T(1.0), axis.y);
		T z = std::copysign(T(1.0), axis.z);

		quat.x = y * z * quat.x;
		quat.y = x * z * quat.y;
		quat.z = x * y * quat.z;

		return quat;
	}

	template<typename T>
	Quat<T> Quat<T>::Slerp(const Quat& from, const Quat& to, T interpolation)
	{
		Quat q;

		T cosOmega = from.DotProduct(to);
		if(cosOmega < T(0.0))
		{
			// We invert everything
			q = Quat(-to.w, -to.x, -to.y, -to.z);
			cosOmega = -cosOmega;
		}
		else
			q = Quat(to);

		T k0, k1;
		if(cosOmega > T(0.9999))
		{
			// Linear interpolation to avoid division by zero
			k0 = T(1.0) - interpolation;
			k1 = interpolation;
		}
		else
		{
			T sinOmega = std::sqrt(T(1.0) - cosOmega*cosOmega);
			T omega = std::atan2(sinOmega, cosOmega);

			// To avoid two divisions
			sinOmega = T(1.0)/sinOmega;

			k0 = std::sin((T(1.0) - interpolation) * omega) * sinOmega;
			k1 = std::sin(interpolation*omega) * sinOmega;
		}

		Quat result(k0 * from.w, k0 * from.x, k0 * from.y, k0 * from.z);
		return result += q * k1;
	}

	template<typename T>
	constexpr Quat<T> Quat<T>::Zero()
	{
		return Quat(0, 0, 0, 0);
	}

	template<typename T>
	std::ostream& operator<<(std::ostream& out, const Quat<T>& quat)
	{
		return out << "Quat(" << quat.w << " | " << quat.x << ", " << quat.y << ", " << quat.z << ')';
	}
}
