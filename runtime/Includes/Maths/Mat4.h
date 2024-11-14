#ifndef __MLX_MAT4__
#define __MLX_MAT4__

#include <Maths/Angles.h>

namespace mlx
{
	template<typename T> struct Vec2;
	template<typename T> struct Vec3;
	template<typename T> struct Vec4;
	template<typename T> struct Quat;

	template<typename T>
	struct Mat4
	{
		T m11, m12, m13, m14;
		T m21, m22, m23, m24;
		T m31, m32, m33, m34;
		T m41, m42, m43, m44;

		constexpr Mat4() = default;
		constexpr Mat4(T r11, T r12, T r13, T r14,
						T r21, T r22, T r23, T r24,
						T r31, T r32, T r33, T r34,
						T r41, T r42, T r43, T r44);
		constexpr Mat4(const T matrix[16]);
		constexpr Mat4(const Mat4&) = default;
		constexpr Mat4(Mat4&&) = default;

		constexpr Mat4& ApplyRotation(const Quat<T>& rotation);
		constexpr Mat4& ApplyScale(const Vec3<T>& scale);
		constexpr Mat4& ApplyTranslation(const Vec3<T>& translation);

		constexpr bool ApproxEqual(const Mat4& vec, T max_difference = std::numeric_limits<T>::epsilon()) const;

		constexpr Mat4& Concatenate(const Mat4& matrix);
		constexpr Mat4& ConcatenateTransform(const Mat4& matrix);

		constexpr Vec4<T> GetColumn(std::size_t column) const;
		constexpr T GetDeterminant() const;
		constexpr T GetDeterminantTransform() const;
		constexpr bool GetInverse(Mat4* dest) const;
		constexpr bool GetInverseTransform(Mat4* dest) const;
		Quat<T> GetRotation() const;
		constexpr Vec4<T> GetRow(std::size_t row) const;
		constexpr Vec3<T> GetScale() const;
		constexpr Vec3<T> GetSquaredScale() const;
		constexpr Vec3<T> GetTranslation() const;
		constexpr void GetTransposed(Mat4* dest) const;

		constexpr bool HasNegativeScale() const;
		constexpr bool HasScale() const;

		constexpr Mat4& Inverse(bool* succeeded = nullptr);
		constexpr Mat4& InverseTransform(bool* succeeded = nullptr);

		constexpr bool IsTransformMatrix() const;
		constexpr bool IsIdentity() const;

		constexpr Mat4& SetRotation(const Quat<T>& rotation);
		constexpr Mat4& SetScale(const Vec3<T>& scale);
		constexpr Mat4& SetTranslation(const Vec3<T>& translation);

		std::string ToString() const;

		constexpr Vec2<T> Transform(const Vec2<T>& vector, T z = 0.0, T w = 1.0) const;
		constexpr Vec3<T> Transform(const Vec3<T>& vector, T w = 1.0) const;
		constexpr Vec4<T> Transform(const Vec4<T>& vector) const;

		constexpr Mat4& Transpose();

		constexpr T& operator()(std::size_t x, std::size_t y);
		constexpr const T& operator()(std::size_t x, std::size_t y) const;

		constexpr T& operator[](std::size_t i);
		constexpr const T& operator[](std::size_t i) const;

		constexpr Mat4& operator=(const Mat4&) = default;
		constexpr Mat4& operator=(Mat4&&) = default;

		constexpr Mat4 operator*(const Mat4& matrix) const;
		constexpr Vec2<T> operator*(const Vec2<T>& vector) const;
		constexpr Vec3<T> operator*(const Vec3<T>& vector) const;
		constexpr Vec4<T> operator*(const Vec4<T>& vector) const;
		constexpr Mat4 operator*(T scalar) const;

		constexpr Mat4& operator*=(const Mat4& matrix);
		constexpr Mat4& operator*=(T scalar);

		constexpr bool operator==(const Mat4& mat) const;
		constexpr bool operator!=(const Mat4& mat) const;

		static constexpr bool ApproxEqual(const Mat4& lhs, const Mat4& rhs, T max_difference = std::numeric_limits<T>::epsilon());
		static constexpr Mat4 Concatenate(const Mat4& left, const Mat4& right);
		static constexpr Mat4 ConcatenateTransform(const Mat4& left, const Mat4& right);
		static constexpr Mat4 Identity();
		static constexpr Mat4 LookAt(const Vec3<T>& eye, const Vec3<T>& target, const Vec3<T>& up = Vec3<T>::Up());
		static constexpr Mat4 Ortho(T left, T right, T top, T bottom, T z_near = -1.0, T zFar = 1.0);
		static Mat4 Perspective(RadianAngle<T> angle, T ratio, T z_near, T z_far);
		static constexpr Mat4 Rotate(const Quat<T>& rotation);
		static constexpr Mat4 Scale(const Vec3<T>& scale);
		static constexpr Mat4 Translate(const Vec3<T>& translation);
		static constexpr Mat4 Transform(const Vec3<T>& translation, const Quat<T>& rotation);
		static constexpr Mat4 Transform(const Vec3<T>& translation, const Quat<T>& rotation, const Vec3<T>& scale);
		static constexpr Mat4 TransformInverse(const Vec3<T>& translation, const Quat<T>& rotation);
		static constexpr Mat4 TransformInverse(const Vec3<T>& translation, const Quat<T>& rotation, const Vec3<T>& scale);
		static constexpr Mat4 Zero();

		~Mat4() = default;
	};

	using Mat4d = Mat4<double>;
	using Mat4f = Mat4<float>;
}

#include <Maths/Mat4.inl>

#endif
