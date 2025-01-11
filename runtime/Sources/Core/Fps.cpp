#include <PreCompiled.h>
#include <Core/Fps.h>

namespace mlx
{
	void FpsManager::Init()
	{
		m_timer = static_cast<std::uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count());
		m_fps_before = m_timer;
		m_fps_now = m_timer;
	}

	bool FpsManager::Update()
	{
		using namespace std::chrono_literals;
		m_fps_now = static_cast<std::uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count());

		if(std::chrono::duration<std::uint64_t>{m_fps_now - m_timer} >= 1s)
			m_timer += m_fps_now;

		m_fps_elapsed_time = m_fps_now - m_fps_before;
		if(m_fps_elapsed_time >= m_ns)
		{
			m_fps_before += m_ns;
			return true;
		}
		std::this_thread::sleep_for(std::chrono::duration<double, std::nano>(m_ns - 1));
		return false;
	}
}
