#ifndef __MLX_VEC4__
#define __MLX_VEC4__

namespace mlx
{
	template<typename T> struct Vec2;
	template<typename T> struct Vec3;

	template<typename T>
	struct Vec4
	{
		union { T x, r, s; };
		union { T y, g, t; };
		union { T z, b, p; };
		union { T w, a, q; };

		constexpr Vec4() = default;
		constexpr Vec4(T X, T Y, T Z, T W = 1.0);
		constexpr Vec4(T X, T Y, const Vec2<T>& vec);
		constexpr Vec4(T X, const Vec2<T>& vec, T W);
		constexpr Vec4(T X, const Vec3<T>& vec);
		constexpr explicit Vec4(T scale);
		constexpr Vec4(const Vec2<T>& vec, T Z = 0.0, T W = 1.0);
		constexpr Vec4(const Vec3<T>& vec, T W = 1.0);
		template<typename U> constexpr explicit Vec4(const Vec4<U>& vec);
		constexpr Vec4(const Vec4&) = default;
		constexpr Vec4(Vec4&&) = default;

		T AbsDotProduct(const Vec4& vec) const;
		constexpr bool ApproxEqual(const Vec4& vec, T max_difference = std::numeric_limits<T>::epsilon()) const;

		constexpr T DotProduct(const Vec4& vec) const;

		Vec4 GetNormal(T* length = nullptr) const;

		constexpr Vec4& Maximize(const Vec4& vec);
		constexpr Vec4& Minimize(const Vec4& vec);

		Vec4& Normalize(T* length = nullptr);

		std::string ToString() const;

		constexpr Vec4& operator=(const Vec4&) = default;
		constexpr Vec4& operator=(Vec4&&) = default;

		constexpr T& operator[](std::size_t i);
		constexpr const T& operator[](std::size_t i) const;

		constexpr const Vec4& operator+() const;
		constexpr Vec4 operator-() const;

		constexpr Vec4 operator+(const Vec4& vec) const;
		constexpr Vec4 operator-(const Vec4& vec) const;
		constexpr Vec4 operator*(const Vec4& vec) const;
		constexpr Vec4 operator*(T scale) const;
		constexpr Vec4 operator/(const Vec4& vec) const;
		constexpr Vec4 operator/(T scale) const;
		constexpr Vec4 operator%(const Vec4& vec) const;
		constexpr Vec4 operator%(T mod) const;

		constexpr Vec4& operator+=(const Vec4& vec);
		constexpr Vec4& operator-=(const Vec4& vec);
		constexpr Vec4& operator*=(const Vec4& vec);
		constexpr Vec4& operator*=(T scale);
		constexpr Vec4& operator/=(const Vec4& vec);
		constexpr Vec4& operator/=(T scale);
		constexpr Vec4& operator%=(const Vec4& vec);
		constexpr Vec4& operator%=(T mod);

		constexpr bool operator==(const Vec4& vec) const;
		constexpr bool operator!=(const Vec4& vec) const;
		constexpr bool operator<(const Vec4& vec) const;
		constexpr bool operator<=(const Vec4& vec) const;
		constexpr bool operator>(const Vec4& vec) const;
		constexpr bool operator>=(const Vec4& vec) const;

		static constexpr Vec4 Apply(T(*func)(T), const Vec4& vec);
		static constexpr bool ApproxEqual(const Vec4& lhs, const Vec4& rhs, T max_difference = std::numeric_limits<T>::epsilon());
		static constexpr T DotProduct(const Vec4& vec1, const Vec4& vec2);
		static Vec4 Normalize(const Vec4& vec);
		static constexpr Vec4 UnitX();
		static constexpr Vec4 UnitY();
		static constexpr Vec4 UnitZ();
		static constexpr Vec4 Zero();

		~Vec4() = default;
	};

	using Vec4d = Vec4<double>;
	using Vec4f = Vec4<float>;
	using Vec4i = Vec4<int>;
	using Vec4ui = Vec4<unsigned int>;
	using Vec4i32 = Vec4<std::int32_t>;
	using Vec4i64 = Vec4<std::int64_t>;
	using Vec4ui32 = Vec4<std::uint32_t>;
	using Vec4ui64 = Vec4<std::uint64_t>;

	template<typename T> std::ostream& operator<<(std::ostream& out, const Vec4<T>& vec);

	template<typename T> constexpr Vec4<T> operator*(T scale, const Vec4<T>& vec);
	template<typename T> constexpr Vec4<T> operator/(T scale, const Vec4<T>& vec);
	template<typename T> constexpr Vec4<T> operator%(T mod, const Vec4<T>& vec);
}

#include <Maths/Vec4.inl>

#endif // __AK_VEC4__

