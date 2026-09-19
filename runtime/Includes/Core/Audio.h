#ifndef __MLX_AUDIO__
#define __MLX_AUDIO__

#include <PreCompiled.h>
#include <vector>

namespace mlx
{
	class Sound
	{
		public:
			Sound(std::shared_ptr<std::vector<uint8_t>> data, uint32_t frame_count);

			inline bool IsValid() const noexcept { return (m_data != nullptr); }

			~Sound() = default;

		private:
			friend class AudioChannel;

			std::shared_ptr<std::vector<uint8_t>> m_data;
			uint32_t m_frame_count = 0;
	};

	class AudioChannel: NonCopyable
	{
		public:
			AudioChannel() = default;

			void Play(const Sound* sound, float start, float end, bool loop) noexcept;

			void Pause() noexcept;
			void Resume() noexcept;

			float GetPlaybackPosition() const noexcept;
			void SetVolume(float left, float right) noexcept;
			void SetSpeed(float speed) noexcept;

			void ComputeGains(int channels) noexcept;
			void MixInto(int32_t* accum, int frames, int channels) noexcept;

			~AudioChannel() = default;

		private:
			std::shared_ptr<std::vector<uint8_t>> m_buffer;
			double m_position = 0.0;
			uint32_t m_start = 0;
			uint32_t m_end = 0;
			float m_speed = 1.0;
			float m_volume_left = 1.0;
			float m_volume_right = 1.0;
			std::vector<float> m_gains{1.0, 1.0};
			bool m_looping = false;
			bool m_paused = true;
	};
}

#endif
