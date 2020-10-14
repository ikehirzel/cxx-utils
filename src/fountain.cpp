#include "fountain.h"

#include <chrono>
#include <iostream>
#include <fstream>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#define HLOG_INFO			0
#define HLOG_INFO_STR		"[INFO]   "

#define HLOG_SUCCESS		1
#define HLOG_SUCCESS_STR	"[SUCCESS]"

#define HLOG_WARNING		2
#define HLOG_WARNING_STR	"[WARNING]"

#define HLOG_ERROR			3
#define HLOG_ERROR_STR		"[ERROR]  "

#define HLOG_FATAL			4
#define HLOG_FATAL_STR		"[FATAL]  "

#define HLOG_LEVELS { HLOG_INFO_STR, HLOG_SUCCESS_STR,\
	HLOG_WARNING_STR, HLOG_ERROR_STR, HLOG_FATAL_STR }

#define color(x) 		"\033["#x"m"
#define HLOG_RESET		color(0)
#define HLOG_WHITE		color(37)
#define HLOG_GREEN		color(32)
#define HLOG_YELLOW		color(33)
#define HLOG_RED		color(31)
#define HLOG_BRIGHT_RED	color(1;31)

#define HLOG_COLORS { HLOG_WHITE, HLOG_GREEN, HLOG_YELLOW, HLOG_RED, HLOG_BRIGHT_RED }

namespace hirzel
{
	std::string logfilename, loggername;
	static std::vector<Log> logs;
	const char *colors[] = HLOG_COLORS;

	Log::Log(unsigned int level, const std::string& classname, const std::string& msg)
	{
		this->level = level;
		this->classname = classname;
		this->msg = msg;
		timestamp = std::to_string(std::chrono::duration_cast<std::chrono::seconds>
			(std::chrono::system_clock::now().time_since_epoch()).count());
	}

	std::string Log::content()
	{
		const char *levels[] = HLOG_LEVELS;
		std::string str;

		if (classname.empty())
		{
			classname = "NULL";
		}

		str = "[";
		str += timestamp;
		str += "] ";
		str += levels[level];
		str += " [";
		str += classname;
		str += "]\t : ";
		str += msg;

		return str;
	}

	void Log::print()
	{
		std::string output = colors[level] + content() + HLOG_RESET;
		std::cout << output << std::endl;
	}

	void log_init(const std::string& _logfilename)
	{
		logfilename = _logfilename;

#if defined(_WIN32) || defined(_WIN64)
		DWORD outMode = 0;
		HANDLE outHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		GetConsoleMode(outHandle, &outMode);
		SetConsoleMode(outHandle, outMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif
	}

	void log_set_name(const std::string& _loggername)
	{
		loggername = _loggername;
	}

	void push(unsigned int level, const std::string& msg)
	{
		Log l(level, loggername, msg);
		logs.push_back(l);
		l.print();
	}

	void log_info(const std::string& msg)
	{
		push(HLOG_INFO, msg);
	}

	void log_success(const std::string& msg)
	{
		push(HLOG_SUCCESS, msg);
	}

	void log_warning(const std::string& msg)
	{
		push(HLOG_WARNING, msg);
	}

	void log_error(const std::string& msg)
	{
		push(HLOG_ERROR, msg);
	}

	void log_fatal(const std::string& msg)
	{
		push(HLOG_FATAL, msg);
	}

	void log_dump()
	{
		if(logfilename.empty())
		{
			push(HLOG_ERROR, "The logger has not been initialized!");
		}
		
		std::ofstream file;
		file.open(logfilename);

		for (Log l : logs)
		{
			file << l.content() << std::endl;
		}

		file.close();
	}
}
