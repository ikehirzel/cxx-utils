#include "fountain.h"

#include <chrono>
#include <iostream>
#include <fstream>
#include <stdarg.h>
#include <string.h>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#define FOUNTAIN_INFO_STR		"[INFO]   "
#define FOUNTAIN_SUCCESS_STR	"[SUCCESS]"
#define FOUNTAIN_WARNING_STR	"[WARNING]"
#define FOUNTAIN_ERROR_STR		"[ERROR]  "
#define FOUNTAIN_FATAL_STR		"[FATAL]  "

#define FOUNTAIN_LEVELS { FOUNTAIN_INFO_STR, FOUNTAIN_SUCCESS_STR,\
	FOUNTAIN_WARNING_STR, FOUNTAIN_ERROR_STR, FOUNTAIN_FATAL_STR }

#define color(x) 		"\033["#x"m"
#define FOUNTAIN_RESET		color(0)
#define FOUNTAIN_WHITE		color(37)
#define FOUNTAIN_GREEN		color(32)
#define FOUNTAIN_YELLOW		color(33)
#define FOUNTAIN_RED		color(31)
#define FOUNTAIN_BRIGHT_RED	color(1;31)

#define FOUNTAIN_COLORS { FOUNTAIN_WHITE, FOUNTAIN_GREEN,\
	FOUNTAIN_YELLOW, FOUNTAIN_RED, FOUNTAIN_BRIGHT_RED }

#define FOUNTAIN_BUF_LEN		256
#define FOUNTAIN_NAME_BUF_LEN	24

#ifndef FOUNTAIN_TAB_SIZE
#define FOUNTAIN_TAB_SIZE 4
#endif

namespace hirzel
{
	static std::string logfilename;
	static std::vector<std::string> logs;
	
	const char *colors[] = FOUNTAIN_COLORS;
	const char *levels[] = FOUNTAIN_LEVELS;

	void log_init(const char* _logfilename)
	{
		logfilename = _logfilename;

#if defined(_WIN32) || defined(_WIN64)
		DWORD outMode = 0;
		HANDLE outHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		GetConsoleMode(outHandle, &outMode);
		SetConsoleMode(outHandle, outMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif
	}

	void log_push(int level, const char* name, int line, const char* str, ...)
	{
		va_list list;
		va_start(list, str);
	
		uint32_t timestamp = std::chrono::duration_cast<std::chrono::seconds>
			(std::chrono::system_clock::now().time_since_epoch()).count();
		
		char log_buf[FOUNTAIN_BUF_LEN];
		char msg_buf[FOUNTAIN_BUF_LEN / 4];
		char name_buf[FOUNTAIN_NAME_BUF_LEN*2];
		char line_buf[6];
		
		sprintf(line_buf, "%d", line);
		int line_len = strlen(line_buf);
		line_buf[line_len] = 0;
		
		strcpy(name_buf, name);
		int name_len = strlen(name_buf);
		name_buf[name_len] = 0;
		
		int tot_len = name_len + line_len + 1;
		
		name_buf[name_len] = ':';
		for (int i = name_len + 1; i < tot_len; i++)
		{
			name_buf[i] = line_buf[i - (name_len + 1)];
		}
		//printf("name: %s len: %d\n", name_buf, name_len);
		
		int diff = FOUNTAIN_NAME_BUF_LEN - tot_len;
		
		// name is too long and has to be trimmed
		if(diff < 0)
		{
			name_buf[FOUNTAIN_NAME_BUF_LEN] = 0;
		}
		// name is either short or the perfect size;
		else
		{
			char tmp[FOUNTAIN_NAME_BUF_LEN];
			strcpy(tmp, name_buf);
			for (int i = 0; i < diff; i++)
			{
				name_buf[i] = ' ';
			}
			//char[FOUNTAIN_NAME_BUF_LEN - tot_len];
			for (int i = diff; i < FOUNTAIN_NAME_BUF_LEN; i++)
			{
				name_buf[i] = tmp[i - diff];
			}
			name_buf[FOUNTAIN_NAME_BUF_LEN] = 0;
			
		}
		// creating the buffer for the log message
		vsprintf(msg_buf, str, list);
		// creating buffer for log output
		sprintf(log_buf, "| %s | [%d] %s : %s", name_buf, timestamp, levels[level], msg_buf);
		// printing log with colors
		printf("%s%s%s\n", colors[level], log_buf, FOUNTAIN_RESET);
		// creating string for log and pushing to vector
		std::string log = std::string(log_buf);
		logs.push_back(log);
	}
	
	void log_dump()
	{
		if(logfilename.empty())
		{
			std::cout << "The logger has not been initialized!\n";
			return;
		}
		
		std::ofstream file;
		file.open(logfilename);

		for (std::string l : logs)
		{
			file << l << std::endl;
		}

		file.close();
	}
}
