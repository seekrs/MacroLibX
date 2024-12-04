#ifndef __MLX_NON_COPYABLE__
#define __MLX_NON_COPYABLE__

namespace mlx
{
	class NonCopyable
	{
		protected:
			NonCopyable() = default;
			virtual ~NonCopyable() = default;

		public:
			NonCopyable(const NonCopyable&) = delete;
			NonCopyable(NonCopyable&&) noexcept = default;
			NonCopyable &operator=(const NonCopyable&) = delete;
			NonCopyable &operator=(NonCopyable&&) noexcept = default;
	};

}

#endif // __MLX_NON_COPYABLE__
