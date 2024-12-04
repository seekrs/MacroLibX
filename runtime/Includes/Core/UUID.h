#ifndef __MLX_UUID__
#define __MLX_UUID__

namespace mlx
{
	class UUID
	{
		public:
			UUID();
			UUID(std::uint64_t uuid);

			inline operator std::uint64_t() const { return m_uuid; }

		private:
			std::uint64_t m_uuid;
	};
}

#endif
