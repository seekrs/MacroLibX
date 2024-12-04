#pragma once
#include <Maths/Vec4.h>

namespace mlx
{
	template<typename T>
	constexpr Vec4<T>::Vec4(T X, T Y, T Z, T W) : x(X), y(Y), z(Z), w(W) {}

	template<typename T>
	constexpr Vec4<T>::Vec4(T X, T Y, const Vec2<T>& vec) : x(X), y(Y), z(vec.x), w(vec.y) {}

	template<typename T>
	constexpr Vec4<T>::Vec4(T X, const Vec2<T>& vec, T W) : x(X), y(vec.x), z(vec.y), w(W) {}

	template<typename T>
	constexpr Vec4<T>::Vec4(T X, const Vec3<T>& vec) : x(X), y(vec.x), z(vec.y), w(vec.z) {}

	template<typename T>
	constexpr Vec4<T>::Vec4(T scale) : x(scale), y(scale), z(scale), w(scale) {}

	template<typename T>
	constexpr Vec4<T>::Vec4(const Vec2<T>& vec, T Z, T W) : x(vec.x), y(vec.y), z(Z), w(W) {}

	template<typename T>
	constexpr Vec4<T>::Vec4(const Vec3<T>& vec, T W) : x(vec.x), y(vec.y), z(vec.z), w(W) {}

	template<typename T>
	template<typename U>
	constexpr Vec4<T>::Vec4(const Vec4<U>& vec) : x(static_cast<T>(vec.x)), y(static_cast<T>(vec.y)), z(static_cast<T>(vec.z)), w(static_cast<T>(vec.w)) {}

	template<typename T>
	T Vec4<T>::AbsDotProduct(const Vec4& vec) const
	{
		return std::abs(x * vec.x) + std::abs(y * vec.y) + std::abs(z * vec.z) + std::abs(w * vec.w);
	}

	template<typename T>
	constexpr bool Vec4<T>::ApproxEqual(const Vec4& vec, T maxDifference) const
	{
		return NumberEquals(x, vec.x, maxDifference) && NumberEquals(y, vec.y, maxDifference) && NumberEquals(z, vec.z, maxDifference) && NumberEquals(w, vec.w, maxDifference);
	}

	template<typename T>
	constexpr T Vec4<T>::DotProduct(const Vec4& vec) const
	{
		return x*vec.x + y*vec.y + z*vec.z + w*vec.w;
	}

	template<typename T>
	Vec4<T> Vec4<T>::GetNormal(T* length) const
	{
		Vec4<T> vec(*this);
		vec.Normalize(length);

		return vec;
	}

	template<typename T>
	constexpr Vec4<T>& Vec4<T>::Maximize(const Vec4& vec)
	{
		if (vec.x > x)
			x = vec.x;

		if (vec.y > y)
			y = vec.y;

		if (vec.z > z)
			z = vec.z;

		if (vec.w > w)
			w = vec.w;

		return *this;
	}

	template<typename T>
	constexpr Vec4<T>& Vec4<T>::Minimize(const Vec4& vec)
	{
		if (vec.x < x)
			x = vec.x;

		if (vec.y < y)
			y = vec.y;

		if (vec.z < z)
			z = vec.z;

		if (vec.w < w)
			w = vec.w;

		return *this;
	}

	template<typename T>
	Vec4<T>& Vec4<T>::Normalize(T* length)
	{
		T invLength = T(1.0) / w;
		x *= invLength;
		y *= invLength;
		z *= invLength;

		if (length)
			*length = w;

		w = T(1.0);

		return *this;
	}

	template<typename T>
	std::string Vec4<T>::ToString() const
	{
		std::ostringstream ss;
		ss << *this;

		return ss.str();
	}

	template<typename T>
	constexpr T& Vec4<T>::operator[](std::size_t i)
	{
		mlx::Assert(i < 4, "index out of range");
		return *(&x + i);
	}

	template<typename T>
	constexpr const T& Vec4<T>::operator[](std::size_t i) const
	{
		mlx::Assert(i < 4, "index out of range");
		return *(&x + i);
	}

	template<typename T>
	constexpr const Vec4<T>& Vec4<T>::operator+() const
	{
		return *this;
	}

	template<typename T>
	constexpr Vec4<T> Vec4<T>::operator-() const
	{
		return Vec4(-x, -y, -z, -w);
	}
	template<typename T>
	constexpr Vec4<T> Vec4<T>::operator+(const Vec4& vec) const
	{
		return Vec4(x + vec.x, y + vec.y, z + vec.z, w + vec.w);
	}
	template<typename T>
	constexpr Vec4<T> Vec4<T>::operator-(const Vec4& vec) const
	{
		return Vec4(x - vec.x, y - vec.y, z - vec.z, w - vec.w);
	}

	template<typename T>
	constexpr Vec4<T> Vec4<T>::operator*(const Vec4& vec) const
	{
		return Vec4(x * vec.x, y * vec.y, z * vec.z, w * vec.w);
	}

	template<typename T>
	constexpr Vec4<T> Vec4<T>::operator*(T scale) const
	{
		return Vec4(x * scale, y * scale, z * scale, w * scale);
	}

	template<typename T>
	constexpr Vec4<T> Vec4<T>::operator/(const Vec4& vec) const
	{
		return Vec4(x / vec.x, y / vec.y, z / vec.z, w / vec.w);
	}

	template<typename T>
	constexpr Vec4<T> Vec4<T>::operator/(T scale) const
	{
		return Vec4(x / scale, y / scale, z / scale, w / scale);
	}

	template<typename T>
	constexpr Vec4<T> Vec4<T>::operator%(const Vec4& vec) const
	{
		return Vec4(Mod(x, vec.x), Mod(y, vec.y), Mod(z, vec.z), Mod(w, vec.w));
	}

	template<typename T>
	constexpr Vec4<T> Vec4<T>::operator%(T mod) const
	{
		return Vec4(Mod(x, mod), Mod(y, mod), Mod(z, mod), Mod(z, mod));
	}

	template<typename T>
	constexpr Vec4<T>& Vec4<T>::operator+=(const Vec4& vec)
	{
		x += vec.x;
		y += vec.y;
		z += vec.z;
		w += vec.w;

		return *this;
	}

	template<typename T>
	constexpr Vec4<T>& Vec4<T>::operator-=(const Vec4& vec)
	{
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;
		w -= vec.w;

		return *this;
	}

	template<typename T>
	constexpr Vec4<T>& Vec4<T>::operator*=(const Vec4& vec)
	{
		x *= vec.x;
		y *= vec.y;
		z *= vec.z;
		w *= vec.w;

		return *this;
	}

	template<typename T>
	constexpr Vec4<T>& Vec4<T>::operator*=(T scale)
	{
		x *= scale;
		y *= scale;
		z *= scale;
		w *= scale;

		return *this;
	}

	template<typename T>
	constexpr Vec4<T>& Vec4<T>::operator/=(const Vec4& vec)
	{
		x /= vec.x;
		y /= vec.y;
		z /= vec.z;
		w /= vec.w;

		return *this;
	}

	template<typename T>
	constexpr Vec4<T>& Vec4<T>::operator/=(T scale)
	{
		x /= scale;
		y /= scale;
		z /= scale;
		w /= scale;

		return *this;
	}

	template<typename T>
	constexpr Vec4<T>& Vec4<T>::operator%=(const Vec4& vec)
	{
		x = Mod(x, vec.x);
		y = Mod(y, vec.y);
		z = Mod(z, vec.z);
		w = Mod(w, vec.w);

		return *this;
	}

	template<typename T>
	constexpr Vec4<T>& Vec4<T>::operator%=(T mod)
	{
		x = Mod(x, mod);
		y = Mod(y, mod);
		z = Mod(z, mod);
		w = Mod(w, mod);

		return *this;
	}

	template<typename T>
	constexpr bool Vec4<T>::operator==(const Vec4& vec) const
	{
		return x == vec.x && y == vec.y && z == vec.z && w == vec.w;
	}

	template<typename T>
	constexpr bool Vec4<T>::operator!=(const Vec4& vec) const
	{
		return !operator==(vec);
	}

	template<typename T>
	constexpr bool Vec4<T>::operator<(const Vec4& vec) const
	{
		if (x != vec.x)
			return x < vec.x;

		if (y != vec.y)
			return y < vec.y;

		if (z != vec.z)
			return z < vec.z;

		return w < vec.w;
	}

	template<typename T>
	constexpr bool Vec4<T>::operator<=(const Vec4& vec) const
	{
		if (x != vec.x)
			return x < vec.x;

		if (y != vec.y)
			return y < vec.y;

		if (z != vec.z)
			return z < vec.z;

		return w <= vec.w;
	}

	template<typename T>
	constexpr bool Vec4<T>::operator>(const Vec4& vec) const
	{
		if (x != vec.x)
			return x > vec.x;

		if (y != vec.y)
			return y > vec.y;

		if (z != vec.z)
			return z > vec.z;

		return w > vec.w;
	}

	template<typename T>
	constexpr bool Vec4<T>::operator>=(const Vec4& vec) const
	{
		if (x != vec.x)
			return x > vec.x;

		if (y != vec.y)
			return y > vec.y;

		if (z != vec.z)
			return z > vec.z;

		return w >= vec.w;
	}

	template<typename T>
	constexpr Vec4<T> Vec4<T>::Apply(T(*func)(T), const Vec4& vec)
	{
		return Vec4(func(vec.x), func(vec.y), func(vec.z), func(vec.w));
	}

	template<typename T>
	constexpr bool Vec4<T>::ApproxEqual(const Vec4& lhs, const Vec4& rhs, T maxDifference)
	{
		return lhs.ApproxEqual(rhs, maxDifference);
	}

	template<typename T>
	constexpr T Vec4<T>::DotProduct(const Vec4& vec1, const Vec4& vec2)
	{
		return vec1.DotProduct(vec2);
	}

	template<typename T>
	Vec4<T> Vec4<T>::Normalize(const Vec4& vec)
	{
		return vec.GetNormal();
	}

	template<typename T>
	constexpr Vec4<T> Vec4<T>::UnitX()
	{
		return Vec4(1, 0, 0, 1);
	}

	template<typename T>
	constexpr Vec4<T> Vec4<T>::UnitY()
	{
		return Vec4(0, 1, 0, 1);
	}

	template<typename T>
	constexpr Vec4<T> Vec4<T>::UnitZ()
	{
		return Vec4(0, 0, 1, 1);
	}

	template<typename T>
	constexpr Vec4<T> Vec4<T>::Zero()
	{
		return Vec4(0, 0, 0, 1);
	}

	template<typename T>
	std::ostream& operator<<(std::ostream& out, const Vec4<T>& vec)
	{
		return out << "Vec4(" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ')';
	}

	template<typename T>
	constexpr Vec4<T> operator*(T scale, const Vec4<T>& vec)
	{
		return Vec4<T>(scale * vec.x, scale * vec.y, scale * vec.z, scale * vec.w);
	}

	template<typename T>
	constexpr Vec4<T> operator/(T scale, const Vec4<T>& vec)
	{
		return Vec4<T>(scale / vec.x, scale / vec.y, scale / vec.z, scale / vec.w);
	}

	template<typename T>
	constexpr Vec4<T> operator%(T mod, const Vec4<T>& vec)
	{
		return Vec4<T>(Mod(mod, vec.x), Mod(mod, vec.y), Mod(mod, vec.z), Mod(mod, vec.w));
	}
}

