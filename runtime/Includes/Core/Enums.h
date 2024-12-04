#ifndef __MLX_CORE_ENUMS__
#define __MLX_CORE_ENUMS__

#include <cstddef>

namespace mlx
{
	enum class LogType
	{
		Message = 0,
		Warning,
		Error,
		FatalError,
		Debug,

		EndEnum
	};
	constexpr std::size_t LogTypeCount = static_cast<std::size_t>(LogType::EndEnum);

	enum class Event
	{
		ResizeEventCode = 56,
		FrameBeginEventCode = 57,
		FatalErrorEventCode = 168,

		EndEnum
	};
	constexpr std::size_t EventCount = static_cast<std::size_t>(Event::EndEnum);
}

#endif
