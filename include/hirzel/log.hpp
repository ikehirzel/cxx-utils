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

	void initLogFile(const char* filepath);
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

#endif // HIRZEL_LOGGER_HPP

#ifdef HIRZEL_IMPLEMENT

#include <string>
#include <fstream>
#include <iostream>
#include <mutex>
#include <ctime>
#include <cassert>

#if defined(_WIN32) || defined(_WIN64)

#define OS_IS_WINDOWS true
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#else 

#define OS_IS_WINDOWS false

#endif

#define COLOR(x)			("\033[" #x "m")
#define COLOR_RESET			COLOR(0)
#define COLOR_BLUE			COLOR(34)
#define COLOR_GREEN			COLOR(32)
#define COLOR_YELLOW		COLOR(33)
#define COLOR_RED			COLOR(31)
#define COLOR_BRIGHT_RED	COLOR(1;31)

#define LOGGER_NAME_BUF_LEN 24

#ifndef LOGGER_TAB_SIZE
#define LOGGER_TAB_SIZE 4
#endif

namespace hirzel::log
{
	std::mutex mutex;
	std::string logFilepath;
	std::ofstream logFile;
	bool isLogPrintingEnabled = true;
	bool isColorEnabled = true;
	const char *debugColor = COLOR_BLUE;
	const char *infoColor = COLOR_RESET;
	const char *successColor = COLOR_GREEN;
	const char *warningColor = COLOR_YELLOW;
	const char *errorColor = COLOR_RED;
	const char *fatalColor = COLOR_BRIGHT_RED;	

	void initLogFile(const char* filepath)
	{
		if (filepath == nullptr || filepath[0] == '\0')
			throw std::invalid_argument("Filename must not be empty.");

		logFilepath = filepath;
		logFile = std::ofstream(filepath);

		if (!logFile.is_open())
			throw std::runtime_error("Failed to open log file: " + std::string(filepath) + ".");
	}

	void printHeader(LogLevel level)
	{
		const char* header;
		const char* color;

		switch (level)
		{
			case LogLevel::Debug:
				header = "  DEBUG";
				color = debugColor;
				break;

			case LogLevel::Info:
				header = "   INFO";
				color = infoColor;
				break;

			case LogLevel::Success:
				header = "SUCCESS";
				color = successColor;
				break;

			case LogLevel::Warning:
				header = "WARNING";
				color = warningColor;
				break;

			case LogLevel::Error:
				header = "  ERROR";
				color = errorColor;
				break;

			case LogLevel::Fatal:
				header = "  FATAL";
				color = fatalColor;
				break;

			default:
				header = "   ????";
				color = "";
				break;
		}

		// TODO: Check if std::cout supports colors

		auto now = time(nullptr);
		auto* time = localtime(&now);

		std::cout << "\r[" << time->tm_hour << ":" << time->tm_min << ":" << time->tm_sec << " " << color << header << COLOR_RESET << "] : ";
	}

	void enableLogPrinting(bool enable)
	{
		isLogPrintingEnabled = enable;
	}

	void enableColor(bool enable)
	{
		isColorEnabled = enable;

//#if OS_IS_WINDOWS
//		if (isColorEnabled)
//		{
//			DWORD outMode = 0;
//			HANDLE outHandle = GetStdHandle(STD_OUTPUT_HPPANDLE);
//			GetConsoleMode(outHandle, &outMode);
//			SetConsoleMode(outHandle, outMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
//		}
//#endif
	}

	void setDebugColor(const char* color)
	{
		assert(color != nullptr);
		debugColor = color;
	}

	void setInfoColor(const char* color)
	{
		assert(color != nullptr);
		infoColor = color;
	}

	void setSuccessColor(const char* color)
	{
		assert(color != nullptr);
		successColor = color;
	}

	void setWarningColor(const char* color)
	{
		assert(color != nullptr);
		warningColor = color;
	}

	void setErrorColor(const char* color)
	{
		assert(color != nullptr);
		errorColor = color;
	}

	void setFatalColor(const char* color)
	{
		assert(color != nullptr);
		fatalColor = color;
	}
}

#endif
