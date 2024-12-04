#ifndef __MLX_LOGS__
#define __MLX_LOGS__

#include <Core/Enums.h>

namespace mlx
{
	template<typename... Args>
	void DebugLog(unsigned int line, std::string_view file, std::string_view function, std::string message, const Args&... args);

	template<typename... Args>
	void Error(unsigned int line, std::string_view file, std::string_view function, std::string message, const Args&... args);

	template<typename... Args>
	void Warning(unsigned int line, std::string_view file, std::string_view function, std::string message, const Args&... args);

	template<typename... Args>
	void Message(unsigned int line, std::string_view file, std::string_view function, std::string message, const Args&... args);

	template<typename... Args>
	void FatalError(unsigned int line, std::string_view file, std::string_view function, std::string message, const Args&... args);

	template<typename... Args>
	void Verify(bool cond, unsigned int line, std::string_view file, std::string_view function, std::string message, const Args&... args);

	class Logs
	{
		public:
			Logs() = delete;

			static void Report(LogType type, std::string message);
			static void Report(LogType type, unsigned int line, std::string_view file, std::string_view function, std::string message);

			~Logs() = delete;
	};

	#ifdef DEBUG
		template<typename... Args>
		void Assert(bool cond, unsigned int line, std::string_view file, std::string_view function, std::string message, const Args&... args);
	#else
		template<typename... Args>
		void Assert([[maybe_unused]] bool cond, [[maybe_unused]] unsigned int line, [[maybe_unused]] std::string_view file, [[maybe_unused]] std::string_view function, [[maybe_unused]] std::string message, [[maybe_unused]] const Args&... args) {}
	#endif
}

#include <Core/Logs.inl>

namespace mlx
{
	#undef  DebugLog
	#define DebugLog(...) DebugLog(__LINE__, __FILE__, __func__, __VA_ARGS__)

	#undef  Message
	#define Message(...) Message(__LINE__, __FILE__, __func__, __VA_ARGS__)

	#undef  Warning
	#define Warning(...) Warning(__LINE__, __FILE__, __func__, __VA_ARGS__)

	#undef  Error
	#define Error(...) Error(__LINE__, __FILE__, __func__, __VA_ARGS__)

	#undef  FatalError
	#define FatalError(...) FatalError(__LINE__, __FILE__, __func__, __VA_ARGS__)

	#undef  Verify
	#define Verify(cond, ...) Verify(cond, __LINE__, __FILE__, __func__, __VA_ARGS__)

	#undef  Assert
	#define Assert(cond, ...) Assert(cond, __LINE__, __FILE__, __func__, __VA_ARGS__)
}

#endif
