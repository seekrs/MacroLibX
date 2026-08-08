#ifndef __MLX_FPS__
#define __MLX_FPS__

#include <chrono>

namespace mlx
{
	typedef std::chrono::steady_clock fps_clock;

	class FpsManager
	{
		public:
			FpsManager() = default;

			void Init();
			void WaitUntilNextFrame();
			inline void SetMaxFPS(std::uint32_t fps) noexcept { m_target_delta = fps_clock::duration(std::chrono::seconds(1)) / fps;}

			~FpsManager() = default;

		private:
			fps_clock::time_point m_current_time;
			fps_clock::time_point m_target_time;
			fps_clock::time_point m_last_time_record;
			fps_clock::duration m_delta_time = fps_clock::duration().zero();
			fps_clock::duration m_target_delta = fps_clock::duration().zero();
			fps_clock::duration m_sleep_margin = std::chrono::microseconds(200);
	};
}

#endif
