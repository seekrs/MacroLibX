#ifndef __MLX_BITS__
#define __MLX_BITS__

namespace mlx
{
	template<std::integral T>
	constexpr T ByteSwap(T value) noexcept
	{
		static_assert(std::has_unique_object_representations_v<T>, "T may not have padding bits");
		auto value_representation = std::bit_cast<std::array<std::byte, sizeof(T)>>(value);
		std::ranges::reverse(value_representation);
		return std::bit_cast<T>(value_representation);
	}

	constexpr std::uint32_t Rmask() noexcept
	{
		if constexpr(std::endian::native == std::endian::big)
			return 0xFF000000;
		else
			return 0x000000FF;
	}

	constexpr std::uint32_t Gmask() noexcept
	{
		if constexpr(std::endian::native == std::endian::big)
			return 0x00FF0000;
		else
			return 0x0000FF00;
	}

	constexpr std::uint32_t Bmask() noexcept
	{
		if constexpr(std::endian::native == std::endian::big)
			return 0x0000FF00;
		else
			return 0x00FF0000;
	}

	constexpr std::uint32_t Amask() noexcept
	{
		if constexpr(std::endian::native == std::endian::big)
			return 0x000000FF;
		else
			return 0xFF000000;
	}
}

#endif
