#pragma once
#include <Core/Logs.h>
#include <Core/Format.h>

namespace mlx
{
	template<typename... Args>
	void DebugLog(unsigned int line, std::string_view file, std::string_view function, std::string message, const Args&... args)
	{
		using namespace std::literals;
		try
		{
			std::stringstream ss;
			ss << Format(message, args...);
			Logs::Report(LogType::Debug, line, file, function, ss.str());
		}
		catch(const std::exception& e)
		{
			Logs::Report(LogType::Error, line, file, function, "formatter exception catched in the log printer: "s + e.what());
		}
	}

	template<typename... Args>
	void Error(unsigned int line, std::string_view file, std::string_view function, std::string message, const Args&... args)
	{
		using namespace std::literals;
		try
		{
			std::stringstream ss;
			ss << Format(message, args...);
			Logs::Report(LogType::Error, line, file, function, ss.str());
		}
		catch(const std::exception& e)
		{
			Logs::Report(LogType::Error, line, file, function, "formatter exception catched in the log printer: "s + e.what());
		}
	}

	template<typename... Args>
	void Warning(unsigned int line, std::string_view file, std::string_view function, std::string message, const Args&... args)
	{
		using namespace std::literals;
		try
		{
			std::stringstream ss;
			ss << Format(message, args...);
			Logs::Report(LogType::Warning, line, file, function, ss.str());
		}
		catch(const std::exception& e)
		{
			Logs::Report(LogType::Error, line, file, function, "formatter exception catched in the log printer: "s + e.what());
		}
	}

	template<typename... Args>
	void Message(unsigned int line, std::string_view file, std::string_view function, std::string message, const Args&... args)
	{
		using namespace std::literals;
		try
		{
			std::stringstream ss;
			ss << Format(message, args...);
			Logs::Report(LogType::Message, line, file, function, ss.str());
		}
		catch(const std::exception& e)
		{
			Logs::Report(LogType::Error, line, file, function, "formatter exception catched in the log printer: "s + e.what());
		}
	}

	template<typename... Args>
	void FatalError(unsigned int line, std::string_view file, std::string_view function, std::string message, const Args&... args)
	{
		using namespace std::literals;
		try
		{
			std::stringstream ss;
			ss << Format(message, args...);
			Logs::Report(LogType::FatalError, line, file, function, ss.str());
		}
		catch(const std::exception& e)
		{
			Logs::Report(LogType::FatalError, line, file, function, "formatter exception catched in the log printer: "s + e.what());
		}
	}

	template<typename... Args>
	void Verify(bool cond, unsigned int line, std::string_view file, std::string_view function, std::string message, const Args&... args)
	{
		using namespace std::literals;
		if(cond)
			return;
		try
		{
			std::stringstream ss;
			ss << Format("Verification failed: %", message, args...);
			Logs::Report(LogType::FatalError, line, file, function, ss.str());
		}
		catch(const std::exception& e)
		{
			Logs::Report(LogType::FatalError, line, file, function, "formatter exception catched in the log printer: "s + e.what());
		}
	}

	#ifdef DEBUG
		template<typename... Args>
		void Assert(bool cond, unsigned int line, std::string_view file, std::string_view function, std::string message, const Args&... args)
		{
			using namespace std::literals;
			if(cond)
				return;
			try
			{
				std::stringstream ss;
				ss << Format("Assertion failed: %", message, args...);
				Logs::Report(LogType::FatalError, line, file, function, ss.str());
			}
			catch(const std::exception& e)
			{
				Logs::Report(LogType::FatalError, line, file, function, "formatter exception catched in the log printer: "s + e.what());
			}
		}
	#endif
}
