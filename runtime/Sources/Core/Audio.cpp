#include <Core/Audio.h>
#include <Core/SDLManager.h>

namespace mlx
{

	Sound::Sound(std::shared_ptr<std::vector<uint8_t>> data, uint32_t frame_count)
	: m_data(data), m_frame_count(frame_count) {}

	void AudioChannel::Play(const Sound* sound, float start, float end, bool loop) noexcept
	{
		if (!sound->IsValid()) return;

		SDL_AudioSpec spec = SDLManager::Get().GetAudioDeviceSpec();
		uint32_t start_frame = std::clamp<float>(std::signbit(start) ?
			sound->m_frame_count + start * spec.freq
			: start * spec.freq,
			0.0, sound->m_frame_count);
		uint32_t end_frame = std::clamp<float>(std::signbit(end) ?
			sound->m_frame_count + end * spec.freq
			: end * spec.freq,
			0.0, sound->m_frame_count);

		SDLManager::AudioDeviceLock lock;
		m_buffer = sound->m_data;
		m_start = start_frame;
		m_end = end_frame;
		m_position = (m_speed > 0) ? start_frame : end_frame - 1e-6;
		m_looping = loop;
		m_paused = false;
	}

	void AudioChannel::Pause() noexcept
	{
		SDLManager::AudioDeviceLock lock;
		m_paused = true;
	}

	void AudioChannel::Resume() noexcept
	{
		SDLManager::AudioDeviceLock lock;
		m_paused = false;
	}

	float AudioChannel::GetPlaybackPosition() const noexcept
	{
		SDLManager::AudioDeviceLock lock;
		return m_position / SDLManager::Get().GetAudioDeviceSpec().freq;
	}

	void AudioChannel::SetVolume(float left, float right) noexcept
	{
		SDLManager::AudioDeviceLock lock;
		m_volume_left = std::max(left, 0.0f);
		m_volume_right = std::max(right, 0.0f);
		m_gains = {};
	}

	void AudioChannel::SetSpeed(float speed) noexcept
	{
		SDLManager::AudioDeviceLock lock;
		m_speed = speed;
	}

	void AudioChannel::ComputeGains(int channels) noexcept
	{
		float &l = m_volume_left, &r = m_volume_right;
		float c = m_volume_left * m_volume_right / 2;

		switch (channels) {
			case 1: m_gains = {c}; break;
			case 2: m_gains = {l, r}; break;
			case 4: m_gains = {l, r, l, r}; break;
			case 6: m_gains = {l, r, c, c, l, r}; break;
			case 8: m_gains = {l, r, c, c, l, r, l, r}; break;
			default: m_gains.assign(channels, c); break;
		}
	}

	void AudioChannel::MixInto(int32_t* accum, int frames, int channels) noexcept
	{
		if (m_paused || !m_buffer) return;

		int16_t* src = reinterpret_cast<int16_t*>(m_buffer->data());
		uint32_t src_frame_count = m_buffer->size() / (2 * channels);
		if (channels != static_cast<int>(m_gains.size()))
			ComputeGains(channels);

		for (int i = 0; i < frames; i++) {
			if (m_position >= m_end) {
				m_position -= (m_end - m_start);
				if (!m_looping) {
					m_paused = true;
					break;
				}
			} else if (m_position < m_start) {
				m_position += (m_end - m_start);
				if (!m_looping) {
					m_paused = true;
					break;
				}
			}

			uint32_t i0 = m_position;
			uint32_t i1 = std::min(i0 + 1, (src_frame_count > 0) ? src_frame_count - 1 : 0);
			float frac = m_position - i0;

			for (int c = 0; c < channels; c++) {
				int16_t s0 = (i0 < src_frame_count) ? src[i0 * channels + c] : 0;
				int16_t s1 = (i1 < src_frame_count) ? src[i1 * channels + c] : s0;
				float sample = s0 + (s1 - s0) * frac;
				accum[i * channels + c] += sample * m_gains[c];
			}
			m_position += m_speed;
		}
	}
}
