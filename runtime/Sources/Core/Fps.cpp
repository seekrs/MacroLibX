#include <PreCompiled.h>
#include <Core/Fps.h>

#ifndef MLX_PLAT_MACOS
#include <emmintrin.h>
#endif

namespace mlx
{
	void FpsManager::Init()
	{
		m_current_time = fps_clock::now();
		m_target_time = m_current_time + m_target_delta;
	}

	void FpsManager::WaitUntilNextFrame()
	{
		m_current_time = fps_clock::now();
		if(m_current_time < m_target_time)
		{
			std::this_thread::sleep_until(m_target_time - m_sleep_margin);
			m_current_time = fps_clock::now();
			while (m_current_time < m_target_time)
			{
				#ifndef MLX_PLAT_MACOS
					_mm_pause(); // reduces CPU usage on x86 without yielding
				#endif
				m_current_time = fps_clock::now();
			}
		}
		else if (m_target_time < m_current_time - m_target_delta * 4)
			m_target_time = m_current_time;
		m_target_time += m_target_delta;
		m_delta_time = m_current_time - m_last_time_record;
		m_last_time_record = m_current_time;
	}
}
