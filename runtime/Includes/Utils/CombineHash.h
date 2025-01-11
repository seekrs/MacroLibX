#ifndef __MLX_HASH__
#define __MLX_HASH__

namespace mlx
{
	inline void HashCombine([[maybe_unused]] std::size_t& seed) noexcept {}

	template <typename T, typename... Rest>
	inline void HashCombine(std::size_t& seed, const T& v, Rest... rest)
	{
		std::hash<T> hasher;
		seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		HashCombine(seed, rest...);
	}
}

#endif
