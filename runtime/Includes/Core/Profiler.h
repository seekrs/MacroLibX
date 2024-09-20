#ifndef __MLX_PROFILER__
#define __MLX_PROFILER__

namespace mlx
{
	using FloatingPointMilliseconds = std::chrono::duration<double, std::milli>;

	struct ProfileResult
	{
		std::string name;
		FloatingPointMilliseconds elapsed_time;
		std::thread::id thread_id;
	};

	class Profiler
	{
		public:
			Profiler(const Profiler&) = delete;
			Profiler(Profiler&&) = delete;
			Profiler() { BeginRuntimeSession(); s_instance = this; }

			void AppendProfileData(ProfileResult&& result);

			inline static bool IsInit() noexcept { return s_instance != nullptr; }
			inline static Profiler& Get() noexcept { return *s_instance; }

			~Profiler();

		private:
			void BeginRuntimeSession();
			void WriteProfile(const ProfileResult& result);
			void EndRuntimeSession();
			inline void WriteHeader()
			{
				m_output_stream << "{\"profileData\":[{}";
				m_output_stream.flush();
			}

			inline void WriteFooter()
			{
				m_output_stream << "]}";
				m_output_stream.flush();
			}

		private:
			static Profiler* s_instance;

			std::unordered_map<std::string, std::pair<std::size_t, ProfileResult>> m_profile_data;
			std::ofstream m_output_stream;
			std::mutex m_mutex;
			bool m_runtime_session_began = false;
	};

	class ProfilerTimer
	{
		public:
			ProfilerTimer(const char* name) : m_name(name)
			{
				m_start_timepoint = std::chrono::steady_clock::now();
			}

			inline void Stop()
			{
				auto end_timepoint = std::chrono::steady_clock::now();
				auto high_res_start = FloatingPointMilliseconds{ m_start_timepoint.time_since_epoch() };
				auto elapsed_time = std::chrono::time_point_cast<std::chrono::milliseconds>(end_timepoint).time_since_epoch() - std::chrono::time_point_cast<std::chrono::milliseconds>(m_start_timepoint).time_since_epoch();

				Profiler::Get().AppendProfileData({ m_name, elapsed_time, std::this_thread::get_id() });

				m_stopped = true;
			}

			~ProfilerTimer()
			{
				if(!m_stopped)
					Stop();
			}

		private:
			std::chrono::time_point<std::chrono::steady_clock> m_start_timepoint;
			const char* m_name;
			bool m_stopped = false;
	};

	namespace ProfilerUtils 
	{
		template <std::size_t N>
		struct ChangeResult
		{
			char data[N];
		};

		template <std::size_t N, std::size_t K>
		constexpr auto CleanupOutputString(const char(&expr)[N], const char(&remove)[K])
		{
			ChangeResult<N> result = {};

			std::size_t src_index = 0;
			std::size_t dst_index = 0;
			while(src_index < N)
			{
				std::size_t match_index = 0;
				while(match_index < K - 1 && src_index + match_index < N - 1 && expr[src_index + match_index] == remove[match_index])
					match_index++;
				if(match_index == K - 1)
					src_index += match_index;
				result.data[dst_index++] = expr[src_index] == '"' ? '\'' : expr[src_index];
				src_index++;
			}
			return result;
		}
	}
}

#ifdef PROFILER
	#define MLX_PROFILE_SCOPE_LINE2(name, line) constexpr auto fixed_name_##line = ::mlx::ProfilerUtils::CleanupOutputString(name, "__cdecl ");\
												::mlx::ProfilerTimer timer##line(fixed_name_##line.data)
	#define MLX_PROFILE_SCOPE_LINE(name, line) MLX_PROFILE_SCOPE_LINE2(name, line)
	#define MLX_PROFILE_SCOPE(name) MLX_PROFILE_SCOPE_LINE(name, __LINE__)
	#define MLX_PROFILE_FUNCTION() MLX_PROFILE_SCOPE(MLX_FUNC_SIG)
#else
	#define MLX_PROFILE_SCOPE(name)
	#define MLX_PROFILE_FUNCTION()
#endif

#endif
