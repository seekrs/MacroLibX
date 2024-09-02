#ifndef __MLX_VEC3__
#define __MLX_VEC3__

namespace mlx
{
	template<typename T> struct Vec2;
	template<typename T> struct Vec4;

	template<typename T>
	struct Vec3
	{
		union { T x, r, s; };
		union { T y, g, t; };
		union { T z, b, p; };

		constexpr Vec3() = default;
		constexpr Vec3(T X, T Y, T Z);
		constexpr Vec3(T X, const Vec2<T>& vec);
		constexpr explicit Vec3(T scale);
		constexpr Vec3(const Vec2<T>& vec, T Z = 0.0);
		template<typename U> constexpr explicit Vec3(const Vec3<U>& vec);
		constexpr Vec3(const Vec3&) = default;
		constexpr Vec3(Vec3&&) = default;
		constexpr explicit Vec3(const Vec4<T>& vec);

		T AbsDotProduct(const Vec3& vec) const;
		constexpr bool ApproxEqual(const Vec3& vec, T max_difference = std::numeric_limits<T>::epsilon()) const;

		constexpr Vec3 CrossProduct(const Vec3& vec) const;

		template<typename U = T> U Distance(const Vec3& vec) const;
		constexpr T DotProduct(const Vec3& vec) const;

		Vec3 GetAbs() const;
		template<typename U = T> U GetLength() const;
		Vec3 GetNormal(T* length = nullptr) const;
		constexpr T GetSquaredLength() const;

		constexpr Vec3& Maximize(const Vec3& vec);
		constexpr Vec3& Minimize(const Vec3& vec);

		Vec3& Normalize(T* length = nullptr);

		constexpr T SquaredDistance(const Vec3& vec) const;

		std::string ToString() const;

		constexpr T& operator[](std::size_t i);
		constexpr const T& operator[](std::size_t i) const;

		constexpr const Vec3& operator+() const;
		constexpr Vec3 operator-() const;

		constexpr Vec3 operator+(const Vec3& vec) const;
		constexpr Vec3 operator-(const Vec3& vec) const;
		constexpr Vec3 operator*(const Vec3& vec) const;
		constexpr Vec3 operator*(T scale) const;
		constexpr Vec3 operator/(const Vec3& vec) const;
		constexpr Vec3 operator/(T scale) const;
		constexpr Vec3 operator%(const Vec3& vec) const;
		constexpr Vec3 operator%(T mod) const;

		constexpr Vec3& operator=(const Vec3&) = default;
		constexpr Vec3& operator=(Vec3&&) = default;

		constexpr Vec3& operator+=(const Vec3& vec);
		constexpr Vec3& operator-=(const Vec3& vec);
		constexpr Vec3& operator*=(const Vec3& vec);
		constexpr Vec3& operator*=(T scale);
		constexpr Vec3& operator/=(const Vec3& vec);
		constexpr Vec3& operator/=(T scale);
		constexpr Vec3& operator%=(const Vec3& vec);
		constexpr Vec3& operator%=(T mod);

		constexpr bool operator==(const Vec3& vec) const;
		constexpr bool operator!=(const Vec3& vec) const;
		constexpr bool operator<(const Vec3& vec) const;
		constexpr bool operator<=(const Vec3& vec) const;
		constexpr bool operator>(const Vec3& vec) const;
		constexpr bool operator>=(const Vec3& vec) const;

		static constexpr Vec3 Apply(T(*func)(T), const Vec3& vec);
		static constexpr bool ApproxEqual(const Vec3& lhs, const Vec3& rhs, T max_difference = std::numeric_limits<T>::epsilon());
		static constexpr Vec3 Backward();
		static constexpr Vec3 Clamp(const Vec3& vec, const Vec3& min, const Vec3& max);
		static constexpr Vec3 CrossProduct(const Vec3& vec1, const Vec3& vec2);
		template<typename U = T> static U Distance(const Vec3& vec1, const Vec3& vec2);
		static constexpr T DotProduct(const Vec3& vec1, const Vec3& vec2);
		static constexpr Vec3 Down();
		static constexpr Vec3 Forward();
		static constexpr Vec3 Left();
		static constexpr Vec3 Max(const Vec3& lhs, const Vec3& rhs);
		static constexpr Vec3 Min(const Vec3& lhs, const Vec3& rhs);
		static Vec3 Normalize(const Vec3& vec);
		static constexpr Vec3 Right();
		static constexpr T SquaredDistance(const Vec3& vec1, const Vec3& vec2);
		static constexpr Vec3 Unit();
		static constexpr Vec3 UnitX();
		static constexpr Vec3 UnitY();
		static constexpr Vec3 UnitZ();
		static constexpr Vec3 Up();
		static constexpr Vec3 Zero();

		~Vec3() = default;
	};

	using Vec3b = Vec3<std::uint8_t>;
	using Vec3d = Vec3<double>;
	using Vec3f = Vec3<float>;
	using Vec3i = Vec3<int>;
	using Vec3ui = Vec3<unsigned int>;
	using Vec3i32 = Vec3<std::int32_t>;
	using Vec3i64 = Vec3<std::int64_t>;
	using Vec3ui32 = Vec3<std::uint32_t>;
	using Vec3ui64 = Vec3<std::uint64_t>;

	template<typename T> std::ostream& operator<<(std::ostream& out, const Vec3<T>& vec);

	template<typename T> constexpr Vec3<T> operator*(T scale, const Vec3<T>& vec);
	template<typename T> constexpr Vec3<T> operator/(T scale, const Vec3<T>& vec);
	template<typename T> constexpr Vec3<T> operator%(T scale, const Vec3<T>& vec);
}

#include <Maths/Vec3.inl>

#endif // __AK_VEC3__
