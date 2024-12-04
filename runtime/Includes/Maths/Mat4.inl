#pragma once
#include <Maths/Mat4.h>

#include <Maths/EulerAngles.h>
#include <Maths/Quaternions.h>
#include <Maths/Vec2.h>
#include <Maths/Vec3.h>
#include <Maths/Vec4.h>
#include <Maths/MathsUtils.h>

namespace mlx
{
	template<typename T>
	constexpr Mat4<T>::Mat4(T r11, T r12, T r13, T r14,
							T r21, T r22, T r23, T r24,
							T r31, T r32, T r33, T r34,
							T r41, T r42, T r43, T r44) :
		m11(r11), m12(r12), m13(r13), m14(r14),
		m21(r21), m22(r22), m23(r23), m24(r24),
		m31(r31), m32(r32), m33(r33), m34(r34),
		m41(r41), m42(r42), m43(r43), m44(r44)
	{}

	template<typename T>
	constexpr Mat4<T>::Mat4(const T matrix[16]) :
		Mat4(matrix[ 0], matrix[ 1], matrix[ 2], matrix[ 3],
			matrix[ 4], matrix[ 5], matrix[ 6], matrix[ 7],
			matrix[ 8], matrix[ 9], matrix[10], matrix[11],
			matrix[12], matrix[13], matrix[14], matrix[15])
	{}

	template<typename T>
	constexpr Mat4<T>& Mat4<T>::ApplyRotation(const Quat<T>& rotation)
	{
		return Concatenate(Mat4<T>::Rotate(rotation));
	}

	template<typename T>
	constexpr Mat4<T>& Mat4<T>::ApplyScale(const Vec3<T>& scale)
	{
		m11 *= scale.x;
		m12 *= scale.x;
		m13 *= scale.x;

		m21 *= scale.y;
		m22 *= scale.y;
		m23 *= scale.y;

		m31 *= scale.z;
		m32 *= scale.z;
		m33 *= scale.z;

		return *this;
	}

	template<typename T>
	constexpr Mat4<T>& Mat4<T>::ApplyTranslation(const Vec3<T>& translation)
	{
		m41 += translation.x;
		m42 += translation.y;
		m43 += translation.z;

		return *this;
	}

	template<typename T>
	constexpr bool Mat4<T>::ApproxEqual(const Mat4& mat, T maxDifference) const
	{
		for(unsigned int i = 0; i < 16; ++i)
			if(!NumberEquals((&m11)[i], (&mat.m11)[i], maxDifference))
				return false;

		return true;
	}

	template<typename T>
	constexpr Mat4<T>& Mat4<T>::Concatenate(const Mat4& matrix)
	{
		return operator=(Mat4(
			m11 * matrix.m11 + m12 * matrix.m21 + m13 * matrix.m31 + m14 * matrix.m41,
			m11 * matrix.m12 + m12 * matrix.m22 + m13 * matrix.m32 + m14 * matrix.m42,
			m11 * matrix.m13 + m12 * matrix.m23 + m13 * matrix.m33 + m14 * matrix.m43,
			m11 * matrix.m14 + m12 * matrix.m24 + m13 * matrix.m34 + m14 * matrix.m44,

			m21 * matrix.m11 + m22 * matrix.m21 + m23 * matrix.m31 + m24 * matrix.m41,
			m21 * matrix.m12 + m22 * matrix.m22 + m23 * matrix.m32 + m24 * matrix.m42,
			m21 * matrix.m13 + m22 * matrix.m23 + m23 * matrix.m33 + m24 * matrix.m43,
			m21 * matrix.m14 + m22 * matrix.m24 + m23 * matrix.m34 + m24 * matrix.m44,

			m31 * matrix.m11 + m32 * matrix.m21 + m33 * matrix.m31 + m34 * matrix.m41,
			m31 * matrix.m12 + m32 * matrix.m22 + m33 * matrix.m32 + m34 * matrix.m42,
			m31 * matrix.m13 + m32 * matrix.m23 + m33 * matrix.m33 + m34 * matrix.m43,
			m31 * matrix.m14 + m32 * matrix.m24 + m33 * matrix.m34 + m34 * matrix.m44,

			m41 * matrix.m11 + m42 * matrix.m21 + m43 * matrix.m31 + m44 * matrix.m41,
			m41 * matrix.m12 + m42 * matrix.m22 + m43 * matrix.m32 + m44 * matrix.m42,
			m41 * matrix.m13 + m42 * matrix.m23 + m43 * matrix.m33 + m44 * matrix.m43,
			m41 * matrix.m14 + m42 * matrix.m24 + m43 * matrix.m34 + m44 * matrix.m44
		));
	}

	template<typename T>
	constexpr Mat4<T>& Mat4<T>::ConcatenateTransform(const Mat4& matrix)
	{
		return operator=(Mat4(
			m11*matrix.m11 + m12*matrix.m21 + m13*matrix.m31,
			m11*matrix.m12 + m12*matrix.m22 + m13*matrix.m32,
			m11*matrix.m13 + m12*matrix.m23 + m13*matrix.m33,
			T(0.0),

			m21*matrix.m11 + m22*matrix.m21 + m23*matrix.m31,
			m21*matrix.m12 + m22*matrix.m22 + m23*matrix.m32,
			m21*matrix.m13 + m22*matrix.m23 + m23*matrix.m33,
			T(0.0),

			m31*matrix.m11 + m32*matrix.m21 + m33*matrix.m31,
			m31*matrix.m12 + m32*matrix.m22 + m33*matrix.m32,
			m31*matrix.m13 + m32*matrix.m23 + m33*matrix.m33,
			T(0.0),

			m41*matrix.m11 + m42*matrix.m21 + m43*matrix.m31 + matrix.m41,
			m41*matrix.m12 + m42*matrix.m22 + m43*matrix.m32 + matrix.m42,
			m41*matrix.m13 + m42*matrix.m23 + m43*matrix.m33 + matrix.m43,
			T(1.0)
		));
	}

	template<typename T>
	constexpr Vec4<T> Mat4<T>::GetColumn(std::size_t column) const
	{
		Assert(column < 4, "column index out of range");
		const T* ptr = &m11 + column * 4;
		return Vec4<T>(ptr[0], ptr[1], ptr[2], ptr[3]);
	}

	template<typename T>
	constexpr T Mat4<T>::GetDeterminant() const
	{
		T A = m22*(m33*m44 - m43*m34) - m32*(m23*m44 - m43*m24) + m42*(m23*m34 - m33*m24);
		T B = m12*(m33*m44 - m43*m34) - m32*(m13*m44 - m43*m14) + m42*(m13*m34 - m33*m14);
		T C = m12*(m23*m44 - m43*m24) - m22*(m13*m44 - m43*m14) + m42*(m13*m24 - m23*m14);
		T D = m12*(m23*m34 - m33*m24) - m22*(m13*m34 - m33*m14) + m32*(m13*m24 - m23*m14);

		return m11*A - m21*B + m31*C - m41*D;
	}

	template<typename T>
	constexpr T Mat4<T>::GetDeterminantTransform() const
	{
		T A = m22*m33 - m32*m23;
		T B = m12*m33 - m32*m13;
		T C = m12*m23 - m22*m13;

		return m11*A - m21*B + m31*C;
	}

	template<typename T>
	constexpr bool Mat4<T>::GetInverse(Mat4* dest) const
	{
		Assert(dest, "destination matrix must be valid");

		T det = GetDeterminant();
		if(det == T(0.0))
			return false;

		// http://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix
		T inv[16];
		inv[0] = m22 * m33 * m44 -
				 m22 * m34 * m43 -
				 m32 * m23 * m44 +
				 m32 * m24 * m43 +
				 m42 * m23 * m34 -
				 m42 * m24 * m33;

		inv[1] = -m12 * m33 * m44 +
				  m12 * m34 * m43 +
				  m32 * m13 * m44 -
				  m32 * m14 * m43 -
				  m42 * m13 * m34 +
				  m42 * m14 * m33;

		inv[2] = m12 * m23 * m44 -
				 m12 * m24 * m43 -
				 m22 * m13 * m44 +
				 m22 * m14 * m43 +
				 m42 * m13 * m24 -
				 m42 * m14 * m23;

		inv[3] = -m12 * m23 * m34 +
				  m12 * m24 * m33 +
				  m22 * m13 * m34 -
				  m22 * m14 * m33 -
				  m32 * m13 * m24 +
				  m32 * m14 * m23;

		inv[4] = -m21 * m33 * m44 +
				  m21 * m34 * m43 +
				  m31 * m23 * m44 -
				  m31 * m24 * m43 -
				  m41 * m23 * m34 +
				  m41 * m24 * m33;

		inv[5] = m11 * m33 * m44 -
				 m11 * m34 * m43 -
				 m31 * m13 * m44 +
				 m31 * m14 * m43 +
				 m41 * m13 * m34 -
				 m41 * m14 * m33;

		inv[6] = -m11 * m23 * m44 +
				  m11 * m24 * m43 +
				  m21 * m13 * m44 -
				  m21 * m14 * m43 -
				  m41 * m13 * m24 +
				  m41 * m14 * m23;

		inv[7] = m11 * m23 * m34 -
				 m11 * m24 * m33 -
				 m21 * m13 * m34 +
				 m21 * m14 * m33 +
				 m31 * m13 * m24 -
				 m31 * m14 * m23;

		inv[8] = m21 * m32 * m44 -
				 m21 * m34 * m42 -
				 m31 * m22 * m44 +
				 m31 * m24 * m42 +
				 m41 * m22 * m34 -
				 m41 * m24 * m32;

		inv[9] = -m11 * m32 * m44 +
				  m11 * m34 * m42 +
				  m31 * m12 * m44 -
				  m31 * m14 * m42 -
				  m41 * m12 * m34 +
				  m41 * m14 * m32;

		inv[10] = m11 * m22 * m44 -
				  m11 * m24 * m42 -
				  m21 * m12 * m44 +
				  m21 * m14 * m42 +
				  m41 * m12 * m24 -
				  m41 * m14 * m22;

		inv[11] = -m11 * m22 * m34 +
				   m11 * m24 * m32 +
				   m21 * m12 * m34 -
				   m21 * m14 * m32 -
				   m31 * m12 * m24 +
				   m31 * m14 * m22;

		inv[12] = -m21 * m32 * m43 +
				   m21 * m33 * m42 +
				   m31 * m22 * m43 -
				   m31 * m23 * m42 -
				   m41 * m22 * m33 +
				   m41 * m23 * m32;

		inv[13] = m11 * m32 * m43 -
				  m11 * m33 * m42 -
				  m31 * m12 * m43 +
				  m31 * m13 * m42 +
				  m41 * m12 * m33 -
				  m41 * m13 * m32;

		inv[14] = -m11 * m22 * m43 +
				   m11 * m23 * m42 +
				   m21 * m12 * m43 -
				   m21 * m13 * m42 -
				   m41 * m12 * m23 +
				   m41 * m13 * m22;

		inv[15] = m11 * m22 * m33 -
				  m11 * m23 * m32 -
				  m21 * m12 * m33 +
				  m21 * m13 * m32 +
				  m31 * m12 * m23 -
				  m31 * m13 * m22;

		T invDet = T(1.0) / det;
		for(unsigned int i = 0; i < 16; ++i)
			inv[i] *= invDet;

		*dest = inv;
		return true;
	}

	template<typename T>
	constexpr bool Mat4<T>::GetInverseTransform(Mat4* dest) const
	{
		Assert(dest, "destination matrix must be valid");

		T det = GetDeterminantTransform();
		if(det == T(0.0))
			return false;


		// http://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix
		T inv[16];
		inv[0] = m22 * m33 - 
				 m32 * m23;

		inv[1] = -m12 * m33 +
				  m32 * m13;

		inv[2] = m12 * m23 -
				m22 * m13;

		inv[3] = T(0.0);

		inv[4] = -m21 * m33 +
				  m31 * m23;

		inv[5] = m11 * m33 -
				 m31 * m13;

		inv[6] = -m11 * m23 +
				  m21 * m13;

		inv[7] = T(0.0);

		inv[8] = m21 * m32 -
				m31 * m22;

		inv[9] = -m11 * m32 +
				  m31 * m12;

		inv[10] = m11 * m22 -
				  m21 * m12;

		inv[11] = T(0.0);

		inv[12] = -m21 * m32 * m43 +
				   m21 * m33 * m42 +
				   m31 * m22 * m43 -
				   m31 * m23 * m42 -
				   m41 * m22 * m33 +
				   m41 * m23 * m32;

		inv[13] = m11 * m32 * m43 -
				  m11 * m33 * m42 -
				  m31 * m12 * m43 +
				  m31 * m13 * m42 +
				  m41 * m12 * m33 -
				  m41 * m13 * m32;

		inv[14] = -m11 * m22 * m43 +
				   m11 * m23 * m42 +
				   m21 * m12 * m43 -
				   m21 * m13 * m42 -
				   m41 * m12 * m23 +
				   m41 * m13 * m22;

		T invDet = T(1.0) / det;
		for(unsigned int i = 0; i < 16; ++i)
			inv[i] *= invDet;

		inv[15] = T(1.0);

		*dest = inv;
		return true;
	}

	template<typename T>
	Quat<T> Mat4<T>::GetRotation() const
	{
		// http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuat/
		Quat<T> quat;

		T trace = m11 + m22 + m33;
		if(trace > T(0.0))
		{
			T s = T(0.5) / std::sqrt(trace + T(1.0));
			quat.w = T(0.25) / s;
			quat.x = (m23 - m32) * s;
			quat.y = (m31 - m13) * s;
			quat.z = (m12 - m21) * s;
		}
		else
		{
			if(m11 > m22 && m11 > m33)
			{
				T s = T(2.0) * std::sqrt(T(1.0) + m11 - m22 - m33);

				quat.w = (m23 - m32) / s;
				quat.x = T(0.25) * s;
				quat.y = (m21 + m12) / s;
				quat.z = (m31 + m13) / s;
			}
			else if(m22 > m33)
			{
				T s = T(2.0) * std::sqrt(T(1.0) + m22 - m11 - m33);

				quat.w = (m31 - m13) / s;
				quat.x = (m21 + m12) / s;
				quat.y = T(0.25) * s;
				quat.z = (m32 + m23) / s;
			}
			else
			{
				T s = T(2.0) * std::sqrt(T(1.0) + m33 - m11 - m22);

				quat.w = (m12 - m21) / s;
				quat.x = (m31 + m13) / s;
				quat.y = (m32 + m23) / s;
				quat.z = T(0.25) * s;
			}
		}

		return quat;
	}

	template<typename T>
	constexpr Vec4<T> Mat4<T>::GetRow(std::size_t row) const
	{
		Assert(row < 4, "row index out of range");

		const T* ptr = &m11;
		return Vec4<T>(ptr[row], ptr[row+4], ptr[row+8], ptr[row+12]);
	}

	template<typename T>
	constexpr Vec3<T> Mat4<T>::GetScale() const
	{
		Vec3<T> squaredScale = GetSquaredScale();
		return Vec3<T>(std::sqrt(squaredScale.x), std::sqrt(squaredScale.y), std::sqrt(squaredScale.z));
	}

	template<typename T>
	constexpr Vec3<T> Mat4<T>::GetSquaredScale() const
	{
		return Vec3<T>(m11 * m11 + m12 * m12 + m13 * m13,
						m21 * m21 + m22 * m22 + m23 * m23,
						m31 * m31 + m32 * m32 + m33 * m33);
	}

	template<typename T>
	constexpr Vec3<T> Mat4<T>::GetTranslation() const
	{
		return Vec3<T>(m41, m42, m43);
	}

	template<typename T>
	constexpr void Mat4<T>::GetTransposed(Mat4* dest) const
	{
		(*dest) = Mat4f(
			m11, m21, m31, m41,
			m12, m22, m32, m42,
			m13, m23, m33, m43,
			m14, m24, m34, m44
		);
	}

	template<typename T>
	constexpr bool Mat4<T>::HasNegativeScale() const
	{
		return GetDeterminant() < T(0.0);
	}

	template<typename T>
	constexpr bool Mat4<T>::HasScale() const
	{
		T t = m11*m11 + m21*m21 + m31*m31;
		if(!NumberEquals(t, T(1.0)))
			return true;

		t = m12*m12 + m22*m22 + m32*m32;
		if(!NumberEquals(t, T(1.0)))
			return true;

		t = m13*m13 + m23*m23 + m33*m33;
		if(!NumberEquals(t, T(1.0)))
			return true;

		return false;
	}

	template<typename T>
	constexpr Mat4<T>& Mat4<T>::Inverse(bool* succeeded)
	{
		bool result = GetInverse(this);
		if(succeeded)
			*succeeded = result;

		return *this;
	}

	template<typename T>
	constexpr Mat4<T>& Mat4<T>::InverseTransform(bool* succeeded)
	{
		bool result = GetInverseTransform(this);
		if(succeeded)
			*succeeded = result;

		return *this;
	}

	template<typename T>
	constexpr bool Mat4<T>::IsTransformMatrix() const
	{
		return NumberEquals(m14, T(0.0)) && NumberEquals(m24, T(0.0)) && NumberEquals(m34, T(0.0)) && NumberEquals(m44, T(1.0));
	}

	template<typename T>
	constexpr bool Mat4<T>::IsIdentity() const
	{
		return (NumberEquals(m11, T(1.0)) && NumberEquals(m12, T(0.0)) && NumberEquals(m13, T(0.0)) && NumberEquals(m14, T(0.0)) &&
				NumberEquals(m21, T(0.0)) && NumberEquals(m22, T(1.0)) && NumberEquals(m23, T(0.0)) && NumberEquals(m24, T(0.0)) &&
				NumberEquals(m31, T(0.0)) && NumberEquals(m32, T(0.0)) && NumberEquals(m33, T(1.0)) && NumberEquals(m34, T(0.0)) &&
				NumberEquals(m41, T(0.0)) && NumberEquals(m42, T(0.0)) && NumberEquals(m43, T(0.0)) && NumberEquals(m44, T(1.0)));
	}

	template<typename T>
	constexpr Mat4<T>& Mat4<T>::SetRotation(const Quat<T>& rotation)
	{
		T qw = rotation.w;
		T qx = rotation.x;
		T qy = rotation.y;
		T qz = rotation.z;

		T qx2 = qx * qx;
		T qy2 = qy * qy;
		T qz2 = qz * qz;

		m11 = T(1.0) - T(2.0) * qy2 - T(2.0) * qz2;
		m21 = T(2.0) * qx * qy - T(2.0) * qz * qw;
		m31 = T(2.0) * qx * qz + T(2.0) * qy * qw;

		m12 = T(2.0) * qx * qy + T(2.0) * qz * qw;
		m22 = T(1.0) - T(2.0) * qx2 - T(2.0) * qz2;
		m32 = T(2.0) * qy * qz - T(2.0) * qx * qw;

		m13 = T(2.0) * qx * qz - T(2.0) * qy * qw;
		m23 = T(2.0) * qy * qz + T(2.0) * qx * qw;
		m33 = T(1.0) - T(2.0) * qx2 - T(2.0) * qy2;

		return *this;
	}

	template<typename T>
	constexpr Mat4<T>& Mat4<T>::SetScale(const Vec3<T>& scale)
	{
		m11 = scale.x;
		m22 = scale.y;
		m33 = scale.z;

		return *this;
	}

	template<typename T>
	constexpr Mat4<T>& Mat4<T>::SetTranslation(const Vec3<T>& translation)
	{
		m41 = translation.x;
		m42 = translation.y;
		m43 = translation.z;

		return *this;
	}

	template<typename T>
	std::string Mat4<T>::ToString() const
	{
		std::ostringstream ss;
		ss << *this;

		return ss.str();
	}

	template<typename T>
	constexpr Vec2<T> Mat4<T>::Transform(const Vec2<T>& vector, T z, T w) const
	{
		return Vec2<T>(m11 * vector.x + m21 * vector.y + m31 * z + m41 * w,
						m12 * vector.x + m22 * vector.y + m32 * z + m42 * w);
	}

	template<typename T>
	constexpr Vec3<T> Mat4<T>::Transform(const Vec3<T>& vector, T w) const
	{
		return Vec3<T>(m11 * vector.x + m21 * vector.y + m31 * vector.z + m41 * w,
						m12 * vector.x + m22 * vector.y + m32 * vector.z + m42 * w,
						m13 * vector.x + m23 * vector.y + m33 * vector.z + m43 * w);
	}

	template<typename T>
	constexpr Vec4<T> Mat4<T>::Transform(const Vec4<T>& vector) const
	{
		return Vec4<T>(m11 * vector.x + m21 * vector.y + m31 * vector.z + m41 * vector.w,
						m12 * vector.x + m22 * vector.y + m32 * vector.z + m42 * vector.w,
						m13 * vector.x + m23 * vector.y + m33 * vector.z + m43 * vector.w,
						m14 * vector.x + m24 * vector.y + m34 * vector.z + m44 * vector.w);
	}

	template<typename T>
	constexpr Mat4<T>& Mat4<T>::Transpose()
	{
		std::swap(m12, m21);
		std::swap(m13, m31);
		std::swap(m14, m41);
		std::swap(m23, m32);
		std::swap(m24, m42);
		std::swap(m34, m43);

		return *this;
	}

	template<typename T>
	constexpr T& Mat4<T>::operator()(std::size_t x, std::size_t y)
	{
		Assert(x <= 3, "index out of range");
		Assert(y <= 3, "index out of range");

		return (&m11)[y*4 + x];
	}

	template<typename T>
	constexpr const T& Mat4<T>::operator()(std::size_t x, std::size_t y) const
	{
		Assert(x <= 3, "index out of range");
		Assert(y <= 3, "index out of range");

		return (&m11)[y*4+x];
	}

	template<typename T>
	constexpr T& Mat4<T>::operator[](std::size_t i)
	{
		Assert(i <= 16, "index out of range");

		return (&m11)[i];
	}

	template<typename T>
	constexpr const T& Mat4<T>::operator[](std::size_t i) const
	{
		Assert(i <= 16, "index out of range");

		return (&m11)[i];
	}

	template<typename T>
	constexpr Mat4<T> Mat4<T>::operator*(const Mat4& matrix) const
	{
		Mat4 result(*this);
		return result.Concatenate(matrix);
	}

	template<typename T>
	constexpr Vec2<T> Mat4<T>::operator*(const Vec2<T>& vector) const
	{
		return Transform(vector);
	}

	template<typename T>
	constexpr Vec3<T> Mat4<T>::operator*(const Vec3<T>& vector) const
	{
		return Transform(vector);
	}

	template<typename T>
	constexpr Vec4<T> Mat4<T>::operator*(const Vec4<T>& vector) const
	{
		return Transform(vector);
	}

	template<typename T>
	constexpr Mat4<T> Mat4<T>::operator*(T scalar) const
	{
		Mat4 mat;
		for(unsigned int i = 0; i < 16; ++i)
			mat[i] = (&m11)[i] * scalar;

		return mat;
	}

	template<typename T>
	constexpr Mat4<T>& Mat4<T>::operator*=(const Mat4& matrix)
	{
		Concatenate(matrix);

		return *this;
	}

	template<typename T>
	constexpr Mat4<T>& Mat4<T>::operator*=(T scalar)
	{
		for(unsigned int i = 0; i < 16; ++i)
			(&m11)[i] *= scalar;

		return *this;
	}

	template<typename T>
	constexpr bool Mat4<T>::operator==(const Mat4& mat) const
	{
		for(unsigned int i = 0; i < 16; ++i)
			if((&m11)[i] != (&mat.m11)[i])
				return false;

		return true;
	}

	template<typename T>
	constexpr bool Mat4<T>::operator!=(const Mat4& mat) const
	{
		return !operator==(mat);
	}

	template<typename T>
	constexpr bool Mat4<T>::ApproxEqual(const Mat4& lhs, const Mat4& rhs, T maxDifference)
	{
		return lhs.ApproxEqual(rhs, maxDifference);
	}

	template<typename T>
	constexpr Mat4<T> Mat4<T>::Concatenate(const Mat4& left, const Mat4& right)
	{
		Mat4 matrix(left); // Copy of left-hand side matrix
		matrix.Concatenate(right); // Concatenation with right-hand side

		return matrix;
	}

	template<typename T>
	constexpr Mat4<T> Mat4<T>::ConcatenateTransform(const Mat4& left, const Mat4& right)
	{
		Mat4 matrix(left); // Copy of left-hand side matrix
		matrix.ConcatenateTransform(right); // Affine concatenation with right-hand side

		return matrix;
	}

	template<typename T>
	constexpr Mat4<T> Mat4<T>::Identity()
	{
		return Mat4(
			T(1.0), T(0.0), T(0.0), T(0.0),
			T(0.0), T(1.0), T(0.0), T(0.0),
			T(0.0), T(0.0), T(1.0), T(0.0),
			T(0.0), T(0.0), T(0.0), T(1.0)
		);
	}

	template<typename T>
	constexpr Mat4<T> Mat4<T>::LookAt(const Vec3<T>& eye, const Vec3<T>& target, const Vec3<T>& up)
	{
		Vec3<T> f = Vec3<T>::Normalize(target - eye);
		Vec3<T> s = Vec3<T>::Normalize(f.CrossProduct(up));
		Vec3<T> u = s.CrossProduct(f);

		return Mat4(
			s.x, u.x, -f.x, T(0.0),
			s.y, u.y, -f.y, T(0.0),
			s.z, u.z, -f.z, T(0.0),
			-s.DotProduct(eye), -u.DotProduct(eye), f.DotProduct(eye), T(1.0)
		);
	}

	template<typename T>
	constexpr Mat4<T> Mat4<T>::Ortho(T left, T right, T top, T bottom, T zNear, T zFar)
	{
		// http://msdn.microsoft.com/en-us/library/windows/desktop/bb204942(v=vs.85).aspx
		return Mat4(
			T(2.0) / (right - left),         T(0.0),                          T(0.0),                  T(0.0),
			T(0.0),                          T(2.0) / (top - bottom),         T(0.0),                  T(0.0),
			T(0.0),                          T(0.0),                          T(1.0) / (zNear - zFar), T(0.0),
			(left + right) / (left - right), (top + bottom) / (bottom - top), zNear / (zNear - zFar),  T(1.0)
		);
	}

	template<typename T>
	Mat4<T> Mat4<T>::Perspective(RadianAngle<T> angle, T ratio, T zNear, T zFar)
	{
		angle /= T(2.0);

		T yScale = angle.GetTan();

		return Mat4(
			T(1.0) / (ratio * yScale), T(0.0),             T(0.0),                          T(0.0),
			T(0.0),                    T(-1.0) / (yScale), T(0.0),                          T(0.0),
			T(0.0),                    T(0.0),             zFar / (zNear - zFar),           T(-1.0),
			T(0.0),                    T(0.0),            -(zNear * zFar) / (zFar - zNear), T(0.0)
		);
	}

	template<typename T>
	constexpr Mat4<T> Mat4<T>::Rotate(const Quat<T>& rotation)
	{
		Mat4 matrix = Mat4::Identity();
		matrix.SetRotation(rotation);

		return matrix;
	}

	template<typename T>
	constexpr Mat4<T> Mat4<T>::Scale(const Vec3<T>& scale)
	{
		return Mat4(
			scale.x, T(0.0),  T(0.0),  T(0.0),
			T(0.0),  scale.y, T(0.0),  T(0.0),
			T(0.0),  T(0.0),  scale.z, T(0.0),
			T(0.0),  T(0.0),  T(0.0),  T(1.0)
		);
	}

	template<typename T>
	constexpr Mat4<T> Mat4<T>::Translate(const Vec3<T>& translation)
	{
		return Mat4(
			T(1.0), T(0.0), T(0.0), T(0.0),
			T(0.0), T(1.0), T(0.0), T(0.0),
			T(0.0), T(0.0), T(1.0), T(0.0),
			translation.x, translation.y, translation.z, T(1.0)
		);
	}

	template<typename T>
	constexpr Mat4<T> Mat4<T>::Transform(const Vec3<T>& translation, const Quat<T>& rotation)
	{
		Mat4 mat = Mat4f::Identity();
		mat.SetRotation(rotation);
		mat.SetTranslation(translation);

		return mat;
	}

	template<typename T>
	constexpr Mat4<T> Mat4<T>::Transform(const Vec3<T>& translation, const Quat<T>& rotation, const Vec3<T>& scale)
	{
		Mat4 mat = Transform(translation, rotation);
		mat.ApplyScale(scale);

		return mat;
	}

	template<typename T>
	constexpr Mat4<T> Mat4<T>::TransformInverse(const Vec3<T>& translation, const Quat<T>& rotation)
	{
		// A view matrix must apply an inverse transformation of the 'world' matrix
		Quat<T> invRot = rotation.GetConjugate(); // Inverse of the rotation

		return Transform(-(invRot * translation), invRot);
	}

	template<typename T>
	constexpr Mat4<T> Mat4<T>::TransformInverse(const Vec3<T>& translation, const Quat<T>& rotation, const Vec3<T>& scale)
	{
		return TransformInverse(translation, rotation).ApplyScale(T(1.0) / scale);
	}

	template<typename T>
	constexpr Mat4<T> Mat4<T>::Zero()
	{
		return Mat4(
			T(0.0), T(0.0), T(0.0), T(0.0),
			T(0.0), T(0.0), T(0.0), T(0.0),
			T(0.0), T(0.0), T(0.0), T(0.0),
			T(0.0), T(0.0), T(0.0), T(0.0)
		);
	}

	template<typename T>
	std::ostream& operator<<(std::ostream& out, const Mat4<T>& matrix)
	{
		return out << "Mat4(" << matrix.m11 << ", " << matrix.m12 << ", " << matrix.m13 << ", " << matrix.m14 << ",\n"
			<< "        " <<        matrix.m21 << ", " << matrix.m22 << ", " << matrix.m23 << ", " << matrix.m24 << ",\n"
			<< "        " <<        matrix.m31 << ", " << matrix.m32 << ", " << matrix.m33 << ", " << matrix.m34 << ",\n"
			<< "        " <<        matrix.m41 << ", " << matrix.m42 << ", " << matrix.m43 << ", " << matrix.m44 << ')';
	}

	template<typename T>
	constexpr Mat4<T> operator*(T scale, const Mat4<T>& matrix)
	{
		return matrix * scale;
	}
}
