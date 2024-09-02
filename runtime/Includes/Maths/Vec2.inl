#pragma once
#include <Maths/Vec2.h>

namespace mlx
{
	template<typename T>
	constexpr Vec2<T>::Vec2(T X, T Y) : x(X), y(Y) {}

	template<typename T>
	constexpr Vec2<T>::Vec2(T scale) : x(scale), y(scale) {}

	template<typename T>
	template<typename U>
	constexpr Vec2<T>::Vec2(const Vec2<U>& vec) : x(static_cast<T>(vec.x)), y(static_cast<T>(vec.y)) {}

	template<typename T>
	constexpr Vec2<T>::Vec2(const Vec3<T>& vec) : x(vec.x), y(vec.y) {}

	template<typename T>
	constexpr Vec2<T>::Vec2(const Vec4<T>& vec) : x(vec.x), y(vec.y) {}

	template<typename T>
	T Vec2<T>::AbsDotProduct(const Vec2& vec) const
	{
		return std::abs(x * vec.x) + std::abs(y * vec.y);
	}

	template<typename T>
	constexpr bool Vec2<T>::ApproxEqual(const Vec2& vec, T maxDifference) const
	{
		return NumberEquals(x, vec.x, maxDifference) && NumberEquals(y, vec.y, maxDifference);
	}

	template<typename T>
	template<typename U>
	U Vec2<T>::Distance(const Vec2& vec) const
	{
		return static_cast<U>(std::sqrt(SquaredDistance(vec)));
	}

	template<typename T>
	constexpr T Vec2<T>::DotProduct(const Vec2& vec) const
	{
		return x * vec.x + y * vec.y;
	}

	template<typename T>
	template<typename U>
	T Vec2<T>::GetLength() const
	{
		return static_cast<U>(std::sqrt(static_cast<U>(GetSquaredLength())));
	}

	template<typename T>
	Vec2<T> Vec2<T>::GetNormal(T* length) const
	{
		Vec2 vec(*this);
		vec.Normalize(length);
		return vec;
	}

	template<typename T>
	constexpr T Vec2<T>::GetSquaredLength() const
	{
		return x * x + y * y;
	}

	template<typename T>
	constexpr Vec2<T>& Vec2<T>::Maximize(const Vec2& vec)
	{
		if(vec.x > x)
			x = vec.x;
		if(vec.y > y)
			y = vec.y;
		return *this;
	}

	template<typename T>
	constexpr Vec2<T>& Vec2<T>::Minimize(const Vec2& vec)
	{
		if(vec.x < x)
			x = vec.x;
		if(vec.y < y)
			y = vec.y;
		return *this;
	}

	template<typename T>
	Vec2<T>& Vec2<T>::Normalize(T* length)
	{
		T norm = GetLength();
		if(norm > T(0.0))
		{
			T invNorm = T(1.0) / norm;
			x *= invNorm;
			y *= invNorm;
		}
		if(length)
			*length = norm;
		return *this;
	}

	template<typename T>
	constexpr T Vec2<T>::SquaredDistance(const Vec2& vec) const
	{
		return (*this - vec).GetSquaredLength();
	}

	template<typename T>
	std::string Vec2<T>::ToString() const
	{
		return "Vec2(" + std::to_string(x) + ", " + std::to_string(y) + ')';
	}

	template<typename T>
	constexpr T& Vec2<T>::operator[](std::size_t i)
	{
		mlx::Assert(i < 2, "index out of range");
		return *(&x + i);
	}

	template<typename T>
	constexpr T Vec2<T>::operator[](std::size_t i) const
	{
		mlx::Assert(i < 2, "index out of range");
		return *(&x + i);
	}

	template<typename T>
	constexpr const Vec2<T>& Vec2<T>::operator+() const
	{
		return *this;
	}

	template<typename T>
	constexpr Vec2<T> Vec2<T>::operator-() const
	{
		return Vec2(-x, -y);
	}

	template<typename T>
	constexpr Vec2<T> Vec2<T>::operator+(const Vec2& vec) const
	{
		return Vec2(x + vec.x, y + vec.y);
	}

	template<typename T>
	constexpr Vec2<T> Vec2<T>::operator-(const Vec2& vec) const
	{
		return Vec2(x - vec.x, y - vec.y);
	}

	template<typename T>
	constexpr Vec2<T> Vec2<T>::operator*(const Vec2& vec) const
	{
		return Vec2(x * vec.x, y * vec.y);
	}

	template<typename T>
	constexpr Vec2<T> Vec2<T>::operator*(T scale) const
	{
		return Vec2(x * scale, y * scale);
	}

	template<typename T>
	constexpr Vec2<T> Vec2<T>::operator/(const Vec2& vec) const
	{
		return Vec2(x / vec.x, y / vec.y);
	}

	template<typename T>
	constexpr Vec2<T> Vec2<T>::operator/(T scale) const
	{
		return Vec2(x / scale, y / scale);
	}

	template<typename T>
	constexpr Vec2<T> Vec2<T>::operator%(const Vec2& vec) const
	{
		return Vec2(Mod(x, vec.x), Mod(y, vec.y));
	}

	template<typename T>
	constexpr Vec2<T> Vec2<T>::operator%(T mod) const
	{
		return Vec2(Mod(x, mod), Mod(y, mod));
	}

	template<typename T>
	constexpr Vec2<T>& Vec2<T>::operator+=(const Vec2& vec)
	{
		x += vec.x;
		y += vec.y;

		return *this;
	}

	template<typename T>
	constexpr Vec2<T>& Vec2<T>::operator-=(const Vec2& vec)
	{
		x -= vec.x;
		y -= vec.y;

		return *this;
	}

	template<typename T>
	constexpr Vec2<T>& Vec2<T>::operator*=(const Vec2& vec)
	{
		x *= vec.x;
		y *= vec.y;

		return *this;
	}

	template<typename T>
	constexpr Vec2<T>& Vec2<T>::operator*=(T scale)
	{
		x *= scale;
		y *= scale;

		return *this;
	}

	template<typename T>
	constexpr Vec2<T>& Vec2<T>::operator/=(const Vec2& vec)
	{
		x /= vec.x;
		y /= vec.y;

		return *this;
	}

	template<typename T>
	constexpr Vec2<T>& Vec2<T>::operator/=(T scale)
	{
		x /= scale;
		y /= scale;

		return *this;
	}

	template<typename T>
	constexpr Vec2<T>& Vec2<T>::operator%=(const Vec2& vec)
	{
		x = Mod(x, vec.x);
		y = Mod(y, vec.y);

		return *this;
	}

	template<typename T>
	constexpr Vec2<T>& Vec2<T>::operator%=(T value)
	{
		x = Mod(x, value);
		y = Mod(y, value);

		return *this;
	}

	template<typename T>
	constexpr bool Vec2<T>::operator==(const Vec2& vec) const
	{
		return x == vec.x && y == vec.y;
	}

	template<typename T>
	constexpr bool Vec2<T>::operator!=(const Vec2& vec) const
	{
		return !operator==(vec);
	}

	template<typename T>
	constexpr bool Vec2<T>::operator<(const Vec2& vec) const
	{
		if (x != vec.x)
			return x < vec.x;

		return y < vec.y;
	}

	template<typename T>
	constexpr bool Vec2<T>::operator<=(const Vec2& vec) const
	{
		if (x != vec.x)
			return x < vec.x;

		return y <= vec.y;
	}

	template<typename T>
	constexpr bool Vec2<T>::operator>(const Vec2& vec) const
	{
		if (x != vec.x)
			return x > vec.x;

		return y > vec.y;
	}

	template<typename T>
	constexpr bool Vec2<T>::operator>=(const Vec2& vec) const
	{
		if (x != vec.x)
			return x > vec.x;

		return y >= vec.y;
	}

	template<typename T>
	constexpr Vec2<T> Vec2<T>::Apply(T(*func)(T), const Vec2& vec)
	{
		return Vec2(func(vec.x), func(vec.y));
	}

	template<typename T>
	constexpr bool Vec2<T>::ApproxEqual(const Vec2& lhs, const Vec2& rhs, T maxDifference)
	{
		return lhs.ApproxEqual(rhs, maxDifference);
	}

	template<typename T>
	template<typename U>
	U Vec2<T>::Distance(const Vec2& vec1, const Vec2& vec2)
	{
		return vec1.Distance<U>(vec2);
	}

	template<typename T>
	constexpr T Vec2<T>::DotProduct(const Vec2& vec1, const Vec2& vec2)
	{
		return vec1.DotProduct(vec2);
	}

	template<typename T>
	Vec2<T> Vec2<T>::Normalize(const Vec2& vec)
	{
		return vec.GetNormal();
	}

	template<typename T>
	constexpr Vec2<T> Vec2<T>::Unit()
	{
		return Vec2(1, 1);
	}

	template<typename T>
	constexpr Vec2<T> Vec2<T>::UnitX()
	{
		return Vec2(1, 0);
	}

	template<typename T>
	constexpr Vec2<T> Vec2<T>::UnitY()
	{
		return Vec2(0, 1);
	}

	template<typename T>
	constexpr Vec2<T> Vec2<T>::Zero()
	{
		return Vec2(0, 0);
	}

	template<typename T>
	std::ostream& operator<<(std::ostream& out, const Vec2<T>& vec)
	{
		return out << "Vec2(" << vec.x << ", " << vec.y << ')';
	}

	template<typename T>
	constexpr Vec2<T> operator*(T scale, const Vec2<T>& vec)
	{
		return Vec2<T>(scale * vec.x, scale * vec.y);
	}

	template<typename T>
	constexpr Vec2<T> operator/(T scale, const Vec2<T>& vec)
	{
		return Vec2<T>(scale / vec.x, scale / vec.y);
	}

	template<typename T>
	constexpr Vec2<T> operator%(T mod, const Vec2<T>& vec)
	{
		return Vec2<T>(Mod(mod, vec.x), Mod(mod, vec.y));
	}
}
