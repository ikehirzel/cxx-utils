#include <hirzel/log.hpp>
#include <string>
#include <fstream>
#include <iostream>
#include <mutex>
#include <ctime>
#include <cassert>

#ifdef _WIN32
#include <windows.h>
#endif

#define COLOR(x)			("\033[" #x "m")
#define COLOR_RESET			COLOR(0)
#define COLOR_BLUE			COLOR(34)
#define COLOR_CYAN			COLOR(36)
#define COLOR_GREEN			COLOR(32)
#define COLOR_YELLOW		COLOR(33)
#define COLOR_RED			COLOR(31)
#define COLOR_BRIGHT_RED	COLOR(1;31)

namespace hirzel::log
{
	std::mutex mutex;
	std::string logFilepath;
	std::ofstream logFile;
	const char* debugColor = COLOR_BLUE;
	const char* infoColor = COLOR_CYAN;
	const char* successColor = COLOR_GREEN;
	const char* warningColor = COLOR_YELLOW;
	const char* errorColor = COLOR_BRIGHT_RED;
	const char* fatalColor = COLOR_RED;

	//void initLogFile(const char* filepath)
	//{
	//	// TODO: Ad an atexit flag to create a log
	//	if (filepath == nullptr || filepath[0] == '\0')
	//		throw std::invalid_argument("Filename must not be empty.");

	//	logFilepath = filepath;
	//	logFile = std::ofstream(filepath);

	//	if (!logFile.is_open())
	//		throw std::runtime_error("Failed to open log file: " + std::string(filepath) + ".");
	//}

	static inline void printNumber(int num)
	{
		if (num < 10)
			std::cout << "0";

		std::cout << num;
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

		std::cout << color << "\r[";
		
		printNumber(time->tm_hour);

		std::cout << ":";

		printNumber(time->tm_min);
		
		std::cout << ":";

		printNumber(time->tm_sec);

		std::cout << " " << color << header << "]" << COLOR_RESET << ": ";
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