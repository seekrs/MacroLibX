#pragma once
#include <Maths/Vec3.h>

namespace mlx
{
	template<typename T>
	constexpr Vec3<T>::Vec3(T X, T Y, T Z) : x(X), y(Y), z(Z) {}

	template<typename T>
	constexpr Vec3<T>::Vec3(T X, const Vec2<T>& vec) : x(X), y(vec.x), z(vec.y) {}

	template<typename T>
	constexpr Vec3<T>::Vec3(T scale) : x(scale), y(scale), z(scale) {}

	template<typename T>
	constexpr Vec3<T>::Vec3(const Vec2<T>& vec, T Z) : x(vec.x), y(vec.y), z(Z) {}

	template<typename T>
	template<typename U>
	constexpr Vec3<T>::Vec3(const Vec3<U>& vec) : x(static_cast<T>(vec.x)), y(static_cast<T>(vec.y)), z(static_cast<T>(vec.z)) {}

	template<typename T>
	constexpr Vec3<T>::Vec3(const Vec4<T>& vec) : x(vec.x), y(vec.y), z(vec.z) {}

	template<typename T>
	T Vec3<T>::AbsDotProduct(const Vec3& vec) const
	{
		return std::abs(x * vec.x) + std::abs(y * vec.y) + std::abs(z * vec.z);
	}

	template<typename T>
	constexpr bool Vec3<T>::ApproxEqual(const Vec3& vec, T maxDifference) const
	{
		return NumberEquals(x, vec.x, maxDifference) && NumberEquals(y, vec.y, maxDifference) && NumberEquals(z, vec.z, maxDifference);
	}

	template<typename T>
	constexpr Vec3<T> Vec3<T>::CrossProduct(const Vec3& vec) const
	{
		return Vec3(y * vec.z - z * vec.y, z * vec.x - x * vec.z, x * vec.y - y * vec.x);
	}

	template<typename T>
	template<typename U>
	U Vec3<T>::Distance(const Vec3& vec) const
	{
		return static_cast<U>(std::sqrt(static_cast<U>(SquaredDistance(vec))));
	}

	template<typename T>
	constexpr T Vec3<T>::DotProduct(const Vec3& vec) const
	{
		return x * vec.x + y * vec.y + z * vec.z;
	}

	template<typename T>
	Vec3<T> Vec3<T>::GetAbs() const
	{
		return Vec3(std::abs(x), std::abs(y), std::abs(z));
	}

	template<typename T>
	template<typename U>
	U Vec3<T>::GetLength() const
	{
		return static_cast<U>(std::sqrt(static_cast<U>(GetSquaredLength())));
	}

	template<typename T>
	Vec3<T> Vec3<T>::GetNormal(T* length) const
	{
		Vec3 vec(*this);
		vec.Normalize(length);

		return vec;
	}

	template<typename T>
	constexpr T Vec3<T>::GetSquaredLength() const
	{
		return x*x + y*y + z*z;
	}

	template<typename T>
	constexpr Vec3<T>& Vec3<T>::Maximize(const Vec3& vec)
	{
		if (vec.x > x)
			x = vec.x;

		if (vec.y > y)
			y = vec.y;

		if (vec.z > z)
			z = vec.z;

		return *this;
	}

	template<typename T>
	constexpr Vec3<T>& Vec3<T>::Minimize(const Vec3& vec)
	{
		if (vec.x < x)
			x = vec.x;

		if (vec.y < y)
			y = vec.y;

		if (vec.z < z)
			z = vec.z;

		return *this;
	}

	template<typename T>
	Vec3<T>& Vec3<T>::Normalize(T* length)
	{
		T norm = GetLength();
		if (norm > T(0.0))
		{
			T invNorm = T(1.0) / norm;
			x *= invNorm;
			y *= invNorm;
			z *= invNorm;
		}

		if (length)
			*length = norm;

		return *this;
	}

	template<typename T>
	constexpr T Vec3<T>::SquaredDistance(const Vec3& vec) const
	{
		return (*this - vec).GetSquaredLength();
	}

	template<typename T>
	std::string Vec3<T>::ToString() const
	{
		return "Vec3(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ')';
	}

	template<typename T>
	constexpr T& Vec3<T>::operator[](std::size_t i)
	{
		mlx::Assert(i < 3, "index out of range");
		return *(&x + i);
	}

	template<typename T>
	constexpr const T& Vec3<T>::operator[](std::size_t i) const
	{
		mlx::Assert(i < 3, "index out of range");
		return *(&x + i);
	}

	template<typename T>
	constexpr const Vec3<T>& Vec3<T>::operator+() const
	{
		return *this;
	}

	template<typename T>
	constexpr Vec3<T> Vec3<T>::operator-() const
	{
		return Vec3(-x, -y, -z);
	}
	template<typename T>
	constexpr Vec3<T> Vec3<T>::operator+(const Vec3& vec) const
	{
		return Vec3(x + vec.x, y + vec.y, z + vec.z);
	}

	template<typename T>
	constexpr Vec3<T> Vec3<T>::operator-(const Vec3& vec) const
	{
		return Vec3(x - vec.x, y - vec.y, z - vec.z);
	}

	template<typename T>
	constexpr Vec3<T> Vec3<T>::operator*(const Vec3& vec) const
	{
		return Vec3(x * vec.x, y * vec.y, z * vec.z);
	}

	template<typename T>
	constexpr Vec3<T> Vec3<T>::operator*(T scale) const
	{
		return Vec3(x * scale, y * scale, z * scale);
	}

	template<typename T>
	constexpr Vec3<T> Vec3<T>::operator/(const Vec3& vec) const
	{
		return Vec3(x / vec.x, y / vec.y, z / vec.z);
	}

	template<typename T>
	constexpr Vec3<T> Vec3<T>::operator/(T scale) const
	{
		return Vec3(x / scale, y / scale, z / scale);
	}

	template<typename T>
	constexpr Vec3<T> Vec3<T>::operator%(const Vec3& vec) const
	{
		return Vec3(Mod(x, vec.x), Mod(y, vec.y), Mod(z, vec.z));
	}

	template<typename T>
	constexpr Vec3<T> Vec3<T>::operator%(T mod) const
	{
		return Vec3(Mod(x, mod), Mod(y, mod), Mod(z, mod));
	}

	template<typename T>
	constexpr Vec3<T>& Vec3<T>::operator+=(const Vec3& vec)
	{
		x += vec.x;
		y += vec.y;
		z += vec.z;

		return *this;
	}

	template<typename T>
	constexpr Vec3<T>& Vec3<T>::operator-=(const Vec3& vec)
	{
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;

		return *this;
	}

	template<typename T>
	constexpr Vec3<T>& Vec3<T>::operator*=(const Vec3& vec)
	{
		x *= vec.x;
		y *= vec.y;
		z *= vec.z;

		return *this;
	}

	template<typename T>
	constexpr Vec3<T>& Vec3<T>::operator*=(T scale)
	{
		x *= scale;
		y *= scale;
		z *= scale;

		return *this;
	}

	template<typename T>
	constexpr Vec3<T>& Vec3<T>::operator/=(const Vec3& vec)
	{
		x /= vec.x;
		y /= vec.y;
		z /= vec.z;

		return *this;
	}

	template<typename T>
	constexpr Vec3<T>& Vec3<T>::operator/=(T scale)
	{
		x /= scale;
		y /= scale;
		z /= scale;

		return *this;
	}

	template<typename T>
	constexpr Vec3<T>& Vec3<T>::operator%=(const Vec3& vec)
	{
		x = Mod(x, vec.x);
		y = Mod(y, vec.y);
		z = Mod(z, vec.z);

		return *this;
	}

	template<typename T>
	constexpr Vec3<T>& Vec3<T>::operator%=(T mod)
	{
		x = Mod(x, mod);
		y = Mod(y, mod);
		z = Mod(z, mod);

		return *this;
	}

	template<typename T>
	constexpr bool Vec3<T>::operator==(const Vec3& vec) const
	{
		return x == vec.x && y == vec.y && z == vec.z;
	}

	template<typename T>
	constexpr bool Vec3<T>::operator!=(const Vec3& vec) const
	{
		return !operator==(vec);
	}

	template<typename T>
	constexpr bool Vec3<T>::operator<(const Vec3& vec) const
	{
		if (x != vec.x)
			return x < vec.x;

		if (y != vec.y)
			return y < vec.y;

		return z < vec.z;
	}

	template<typename T>
	constexpr bool Vec3<T>::operator<=(const Vec3& vec) const
	{
		if (x != vec.x)
			return x < vec.x;

		if (y != vec.y)
			return y < vec.y;

		return z <= vec.z;
	}

	template<typename T>
	constexpr bool Vec3<T>::operator>(const Vec3& vec) const
	{
		if (x != vec.x)
			return x > vec.x;

		if (y != vec.y)
			return y > vec.y;

		return z > vec.z;
	}

	template<typename T>
	constexpr bool Vec3<T>::operator>=(const Vec3& vec) const
	{
		if (x != vec.x)
			return x > vec.x;

		if (y != vec.y)
			return y > vec.y;

		return z >= vec.z;
	}

	template<typename T>
	constexpr Vec3<T> Vec3<T>::Apply(T(*func)(T), const Vec3& vec)
	{
		return Vec3(func(vec.x), func(vec.y), func(vec.z));
	}

	template<typename T>
	constexpr bool Vec3<T>::ApproxEqual(const Vec3& lhs, const Vec3& rhs, T maxDifference)
	{
		return lhs.ApproxEqual(rhs, maxDifference);
	}

	template<typename T>
	constexpr Vec3<T> Vec3<T>::CrossProduct(const Vec3& vec1, const Vec3& vec2)
	{
		return vec1.CrossProduct(vec2);
	}

	template<typename T>
	constexpr T Vec3<T>::DotProduct(const Vec3& vec1, const Vec3& vec2)
	{
		return vec1.DotProduct(vec2);
	}

	template<typename T>
	constexpr Vec3<T> Vec3<T>::Backward()
	{
		return Vec3(0, 0, 1);
	}

	template<typename T>
	template<typename U>
	U Vec3<T>::Distance(const Vec3& vec1, const Vec3& vec2)
	{
		return vec1.Distance<U>(vec2);
	}

	template<typename T>
	constexpr Vec3<T> Vec3<T>::Down()
	{
		return Vec3(0, -1, 0);
	}

	template<typename T>
	constexpr Vec3<T> Vec3<T>::Forward()
	{
		return Vec3(0, 0, -1);
	}

	template<typename T>
	constexpr Vec3<T> Vec3<T>::Left()
	{
		return Vec3(-1, 0, 0);
	}

	template<typename T>
	constexpr Vec3<T> Vec3<T>::Max(const Vec3& lhs, const Vec3& rhs)
	{
		Vec3 max = lhs;
		max.Maximize(rhs);

		return max;
	}

	template<typename T>
	constexpr Vec3<T> Vec3<T>::Min(const Vec3& lhs, const Vec3& rhs)
	{
		Vec3 min = lhs;
		min.Minimize(rhs);

		return min;
	}

	template<typename T>
	Vec3<T> Vec3<T>::Normalize(const Vec3& vec)
	{
		return vec.GetNormal();
	}

	template<typename T>
	constexpr Vec3<T> Vec3<T>::Right()
	{
		return Vec3(1, 0, 0);
	}

	template<typename T>
	constexpr T Vec3<T>::SquaredDistance(const Vec3& vec1, const Vec3& vec2)
	{
		return vec1.SquaredDistance(vec2);
	}

	template<typename T>
	constexpr Vec3<T> Vec3<T>::Unit()
	{
		return Vec3(1);
	}

	template<typename T>
	constexpr Vec3<T> Vec3<T>::UnitX()
	{
		return Vec3(1, 0, 0);
	}

	template<typename T>
	constexpr Vec3<T> Vec3<T>::UnitY()
	{
		return Vec3(0, 1, 0);
	}

	template<typename T>
	constexpr Vec3<T> Vec3<T>::UnitZ()
	{
		return Vec3(0, 0, 1);
	}

	template<typename T>
	constexpr Vec3<T> Vec3<T>::Up()
	{
		return Vec3(0, 1, 0);
	}

	template<typename T>
	constexpr Vec3<T> Vec3<T>::Zero()
	{
		return Vec3(0, 0, 0);
	}

	template<typename T>
	std::ostream& operator<<(std::ostream& out, const Vec3<T>& vec)
	{
		return out << "Vec3(" << vec.x << ", " << vec.y << ", " << vec.z << ')';
	}

	template<typename T>
	constexpr Vec3<T> operator*(T scale, const Vec3<T>& vec)
	{
		return Vec3<T>(scale * vec.x, scale * vec.y, scale * vec.z);
	}

	template<typename T>
	constexpr Vec3<T> operator/(T scale, const Vec3<T>& vec)
	{
		return Vec3<T>(scale / vec.x, scale / vec.y, scale / vec.z);
	}

	template<typename T>
	constexpr Vec3<T> operator%(T mod, const Vec3<T>& vec)
	{
		return Vec3<T>(Mod(mod, vec.x), Mod(mod, vec.y), Mod(mod, vec.z));
	}
}

