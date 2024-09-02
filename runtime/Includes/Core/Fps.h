#ifndef __MLX_FPS__
#define __MLX_FPS__

namespace mlx
{
	class FpsManager
	{
		public:
			FpsManager() = default;

			void Init();
			bool Update();
			inline void SetMaxFPS(std::uint32_t fps) noexcept { m_max_fps = fps; m_ns = 1000000000.0 / fps; }

			~FpsManager() = default;

		private:
			double m_ns = 1000000000.0 / 1'337'000.0;
			std::int64_t m_fps_before = 0;
			std::int64_t m_fps_now = 0;
			std::int64_t m_timer = 0;
			std::uint32_t m_max_fps = 1'337'000;
			std::uint32_t m_fps_elapsed_time = 0;
	};
}

#endif
