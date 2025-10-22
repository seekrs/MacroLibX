#ifndef __MLX_UUID__
#define __MLX_UUID__

namespace mlx
{
	class UUID
	{
		public:
			UUID();
			inline operator std::uint64_t() const noexcept { return m_uuid; }

		private:
			std::uint64_t m_uuid;
	};
}

#endif
