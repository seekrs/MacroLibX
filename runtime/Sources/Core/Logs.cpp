#include <PreCompiled.h>
#include <Core/Enums.h>
#include <Utils/Ansi.h>

namespace mlx
{
	namespace Internal
	{
		struct FatalErrorEvent : public EventBase
		{
			Event What() const override { return Event::FatalErrorEventCode; }
		};
	}

	std::uint32_t Logs::s_nesting = 0;

	constexpr int LOGS_TABS_WIDTH = 4;

	void Logs::Report(LogType type, std::string message)
	{
		Report(type, 0, {}, {}, std::move(message));
	}

	void Logs::Report(LogType type, unsigned int line, std::string_view file, std::string_view function, std::string message)
	{
		using namespace std::literals;

		#ifndef DEBUG
			if(type == LogType::Debug)
				return;
		#endif

		std::string code_infos;
		if((type == LogType::Error || type == LogType::FatalError) && !file.empty() && !function.empty())
		{
			code_infos += "{in file '"s;
			code_infos += file;
			code_infos += "', line "s + std::to_string(line) + ", in function '"s;
			code_infos += function;
			code_infos += "'} "s;
		}

		switch(type)
		{
			case LogType::Debug:
			{
				std::cout << Ansi::blue << "[MLX Debug] " << Ansi::def << std::flush;
				std::printf("%*s", s_nesting * LOGS_TABS_WIDTH, "");
				std::fflush(stdout);
				std::cout << code_infos << message << std::endl;
				break;
			}
			case LogType::Message: std::cout << Ansi::blue << "[MLX Message] " << Ansi::def << code_infos << message << '\n'; break;
			case LogType::Warning: std::cout << Ansi::magenta << "[MLX Warning] " << Ansi::def << code_infos << message << '\n'; break;
			case LogType::Error: std::cerr << Ansi::red << "[MLX Error] " << Ansi::def << code_infos << message << '\n'; break;
			case LogType::FatalError: std::cerr << Ansi::red << "[MLX Fatal Error] " << Ansi::def << code_infos << message << '\n'; break;

			default: break;
		}
		if(type == LogType::FatalError)
		{
			std::cout << Ansi::bg_red << "Fatal Error: emergency exit" << Ansi::bg_def << std::endl;
			EventBus::Send("__MlxApplication", Internal::FatalErrorEvent{});
		}
	}

	void Logs::BeginSection()
	{
		s_nesting++;
	}

	void Logs::EndSection()
	{
		if(s_nesting > 0)
			s_nesting--;
	}
}
