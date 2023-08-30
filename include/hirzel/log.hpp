#ifndef HIRZEL_LOG_HPP
#define HIRZEL_LOG_HPP

#include <hirzel/print.hpp>

namespace hirzel::log
{
	enum class LogLevel
	{
		Debug,
		Info,
		Success,
		Warning,
		Error,
		Fatal
	};

	//void initLogFile(const char* filepath);
	void printHeader(LogLevel level);

	template <typename... Arg>
	void debug(const char* format, Arg const&... args)
	{
#ifndef NDEBUG
		printHeader(LogLevel::Debug);
		println(format, args...);
#endif
	}

	template <typename... Arg>
	void info(const char* format, Arg const&... args)
	{
		printHeader(LogLevel::Info);
		println(format, args...);
	}

	template <typename... Arg>
	void success(const char* format, Arg const&... args)
	{
		printHeader(LogLevel::Success);
		println(format, args...);
	}

	template <typename... Arg>
	void warning(const char* format, Arg const&... args)
	{
		printHeader(LogLevel::Warning);
		println(format, args...);
	}

	template <typename... Arg>
	void error(const char* format, Arg const&... args)
	{
		printHeader(LogLevel::Error);
		println(format, args...);
	}

	template <typename... Arg>
	void fatal(const char* format, Arg const&... args)
	{
		printHeader(LogLevel::Fatal);
		println(format, args...);
	}

	void setDebugColor(const char* color);
	void setInfoColor(const char* color);
	void setSuccessColor(const char* color);
	void setWarningColor(const char* color);
	void setErrorColor(const char* color);
	void setFatalColor(const char* color);
}

#endif
