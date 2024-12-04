#ifndef __MLX_VEC2__
#define __MLX_VEC2__

namespace mlx
{
	template <typename T> struct Vec3;
	template <typename T> struct Vec4;

	template <typename T>
	struct Vec2
	{
		union { T x, r, s; };
		union { T y, g, t; };

		constexpr Vec2() = default;
		constexpr Vec2(T X, T Y);
		constexpr explicit Vec2(T scale);
		template<typename U> constexpr explicit Vec2(const Vec2<U>& vec);
		constexpr Vec2(const Vec2&) = default;
		constexpr Vec2(Vec2&&) = default;
		constexpr explicit Vec2(const Vec3<T>& vec);
		constexpr explicit Vec2(const Vec4<T>& vec);

		T AbsDotProduct(const Vec2& vec) const;
		constexpr bool ApproxEqual(const Vec2& vec, T max_difference = std::numeric_limits<T>::epsilon()) const;

		template<typename U = T> U Distance(const Vec2& vec) const;
		constexpr T DotProduct(const Vec2& vec) const;

		template<typename U = T> T GetLength() const;
		Vec2 GetNormal(T* length = nullptr) const;
		constexpr T GetSquaredLength() const;

		constexpr Vec2& Maximize(const Vec2& vec);
		constexpr Vec2& Minimize(const Vec2& vec);

		Vec2& Normalize(T* length = nullptr);

		constexpr T SquaredDistance(const Vec2& vec) const;

		std::string ToString() const;

		constexpr T& operator[](std::size_t i);
		constexpr T operator[](std::size_t i) const;

		constexpr const Vec2& operator+() const;
		constexpr Vec2 operator-() const;

		constexpr Vec2 operator+(const Vec2& vec) const;
		constexpr Vec2 operator-(const Vec2& vec) const;
		constexpr Vec2 operator*(const Vec2& vec) const;
		constexpr Vec2 operator*(T scale) const;
		constexpr Vec2 operator/(const Vec2& vec) const;
		constexpr Vec2 operator/(T scale) const;
		constexpr Vec2 operator%(const Vec2& vec) const;
		constexpr Vec2 operator%(T mod) const;

		constexpr Vec2& operator=(const Vec2&) = default;
		constexpr Vec2& operator=(Vec2&&) = default;

		constexpr Vec2& operator+=(const Vec2& vec);
		constexpr Vec2& operator-=(const Vec2& vec);
		constexpr Vec2& operator*=(const Vec2& vec);
		constexpr Vec2& operator*=(T scale);
		constexpr Vec2& operator/=(const Vec2& vec);
		constexpr Vec2& operator/=(T scale);
		constexpr Vec2& operator%=(const Vec2& vec);
		constexpr Vec2& operator%=(T mod);

		constexpr bool operator==(const Vec2& vec) const;
		constexpr bool operator!=(const Vec2& vec) const;
		constexpr bool operator<(const Vec2& vec) const;
		constexpr bool operator<=(const Vec2& vec) const;
		constexpr bool operator>(const Vec2& vec) const;
		constexpr bool operator>=(const Vec2& vec) const;

		static constexpr Vec2 Apply(T(*func)(T), const Vec2& vec);
		static constexpr bool ApproxEqual(const Vec2& lhs, const Vec2& rhs, T max_difference = std::numeric_limits<T>::epsilon());
		template<typename U = T> static U Distance(const Vec2& vec1, const Vec2& vec2);
		static constexpr T DotProduct(const Vec2& vec1, const Vec2& vec2);
		static constexpr Vec2 Lerp(const Vec2& from, const Vec2& to, T interpolation);
		static Vec2 Normalize(const Vec2& vec);
		static constexpr Vec2 Unit();
		static constexpr Vec2 UnitX();
		static constexpr Vec2 UnitY();
		static constexpr Vec2 Zero();

		~Vec2() = default;
	};

	using Vec2d = Vec2<double>;
	using Vec2f = Vec2<float>;
	using Vec2i = Vec2<int>;
	using Vec2ui = Vec2<unsigned int>;
	using Vec2i32 = Vec2<std::int32_t>;
	using Vec2i64 = Vec2<std::int64_t>;
	using Vec2ui32 = Vec2<std::uint32_t>;
	using Vec2ui64 = Vec2<std::uint64_t>;

	template<typename T> std::ostream& operator<<(std::ostream& out, const Vec2<T>& vec);

	template<typename T> constexpr Vec2<T> operator*(T scale, const Vec2<T>& vec);
	template<typename T> constexpr Vec2<T> operator/(T scale, const Vec2<T>& vec);
	template<typename T> constexpr Vec2<T> operator%(T mod, const Vec2<T>& vec);
}

#include <Maths/Vec2.inl>

#endif // __AK_VEC2__
