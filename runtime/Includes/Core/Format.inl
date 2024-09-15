#pragma once
#include <Core/Format.h>

namespace mlx
{
	namespace Internal
	{
		template<typename It>
		void Format(std::stringstream& ss, It first, It last)
		{
			for(auto it = first; it != last; ++it)
			{
				switch(*it)
				{
					case '%':
						throw std::invalid_argument{"too few arguments"};
					case '/':
						++it;
						if(it == last)
							throw std::invalid_argument{"stray '/'"};
						[[fallthrough]];

					default: ss << *it;
				}
			}
		}

		template<typename It, typename T, typename... Args>
		void Format(std::stringstream& ss, It first, It last, const T& arg, const Args&... args)
		{
			for(auto it = first; it != last; ++it)
			{
				switch(*it)
				{
					case '%':
						ss << arg;
						return Format(ss, ++it, last, args...);
					case '/':
						++it;
						if(it == last)
							throw std::invalid_argument{"stray '/'"};
						[[fallthrough]];

					default: ss << *it;
				}
			}
			throw std::invalid_argument{"too many arguments"};
		}

		template<typename It>
		void Format(std::ostream& os, It first, It last)
		{
			for(auto it = first; it != last; ++it)
			{
				switch(*it)
				{
					case '%':
						throw std::invalid_argument{"too few arguments"};
					case '/':
						++it;
						if(it == last)
							throw std::invalid_argument{"stray '/'"};
						[[fallthrough]];

					default: os << *it;
				}
			}
		}

		template<typename It, typename T, typename... Args>
		void Format(std::ostream& os, It first, It last, const T& arg, const Args&... args)
		{
			for(auto it = first; it != last; ++it)
			{
				switch(*it)
				{
					case '%':
						os << arg;
						return Format(os, ++it, last, args...);
					case '/':
						++it;
						if(it == last)
							throw std::invalid_argument{"stray '/'"};
						[[fallthrough]];

					default: os << *it;
				}
			}
			throw std::invalid_argument{"too many arguments"};
		}

		template<typename... Args>
		struct Formatter
		{
			std::string_view format;
			std::tuple<const Args&...> args;
		};

		template<typename... Args, std::size_t... Is>
		void FormatHelper(std::stringstream& ss, const Formatter<Args...>& formatter, std::index_sequence<Is...>)
		{
			Format(ss, formatter.format.begin(), formatter.format.end(),
			std::get<Is>(formatter.args)...);
		}

		template<typename... Args>
		std::stringstream& operator<<(std::stringstream& ss, const Formatter<Args...>& printer)
		{
			FormatHelper(ss, printer, std::index_sequence_for<Args...>{});
			return ss;
		}

		template<typename... Args, std::size_t... Is>
		void FormatHelper(std::ostream& os, const Formatter<Args...>& formatter, std::index_sequence<Is...>)
		{
			Format(os, formatter.format.begin(), formatter.format.end(),
			std::get<Is>(formatter.args)...);
		}

		template<typename... Args>
		std::ostream& operator<<(std::ostream& os, const Formatter<Args...>& printer)
		{
			FormatHelper(os, printer, std::index_sequence_for<Args...>{});
			return os;
		}
	}

	template<typename... Args, std::enable_if_t<std::conjunction_v<IsOstreamable<Args>...>, int>>
	auto Format(std::string_view format, const Args&... args)
	{
		return Internal::Formatter<Args...>{format, std::forward_as_tuple(args...)};
	}
}
