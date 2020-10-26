#include "fountain.h"

#include <hirzel/var.h>

#include <ctime>
#include <iostream>
#include <fstream>

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

#define color(x) 			"\033["#x"m"
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
#define FOUNTAIN_TAB_SIZE		4
#endif

#define FOUNTAIN_STRING			's'
#define FOUNTAIN_CHAR			'c'
#define FOUNTAIN_INT			'd'
#define FOUNTAIN_UINT			'u'
#define FOUNTAIN_OCTAL			'o'
#define FOUNTAIN_HEXLOWER		'x'
#define FOUNTAIN_HEXUPPER		'X'
#define FOUNTAIN_FLOAT			'f'
#define FOUNTAIN_PERCENT		'%'

namespace hirzel
{
	static std::string logfilename;
	static std::vector<std::string> logs;
	
	const char *colors[] = FOUNTAIN_COLORS;
	const char *levels[] = FOUNTAIN_LEVELS;

	std::string formatstr(const std::string& str, const std::vector<var>& vars)
	{
		std::string out;
		std::string tmp;

		int bytes;
		double f;
		long long d;
		unsigned long long u;
		char c;

		size_t oi = 0, li = 0;
		out.resize(256, 0);

		for(size_t i = 0; i < str.size(); i++)
		{
			while (str[i] != '%' && i < str.size())
			{
				out[oi] = str[i];
				oi++;
				i++;
			}

			i++;

			switch(str[i])
			{
				case FOUNTAIN_INT:
					std::cout << "INT\n";
					break;

				case FOUNTAIN_UINT:
					std::cout << "UINT\n";
					break;

				case FOUNTAIN_STRING:
					std::cout << "STRING\n";
					break;

				case FOUNTAIN_CHAR:
					std::cout << "CHAR\n";
					break;
			}
		}

		out.shrink_to_fit();
		return out;
	}

	void log_init(const std::string& _logfilename)
	{
		hirzel::logfilename = _logfilename;

#if defined(_WIN32) || defined(_WIN64)
		DWORD outMode = 0;
		HANDLE outHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		GetConsoleMode(outHandle, &outMode);
		SetConsoleMode(outHandle, outMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif

	}
	void log_push(int level, const char* name, int line, const std::string& str, const std::vector<var>& list)
	{
		std::cout << "LEVEL: " << level << std::endl;
		std::cout << "NAME: " << name << std::endl;
		std::cout << "LINE: " << line << std::endl;
		std::cout << "VARS: \n";
		for(var v : list)
		{
			std::cout << v.as_string() << std::endl;
		}
		/*
		va_list list;
		va_start(list, str);
		uintmax_t timestamp = time(NULL);
		
		std::string name_str, log, msg, line_buf;
		std::string name_buf(FOUNTAIN_NAME_BUF_LEN, ' ');
		
		line_buf = std::to_string(line);
		name_str = name + ':' + line_buf;
		name_str = name_buf + name_str;
		int spaces = FOUNTAIN_NAME_BUF_LEN - name_str.size();
		std::cout << "MAybe it break\n";
		for (int i = 0; i < FOUNTAIN_NAME_BUF_LEN; i++)
		{
			name_buf[i] = name_str[i - spaces];
		}
		std::cout << "it no break\n";
		for(char &c : name_buf)
		{
			if(c == 0)
			{
				c = ' ';
			}
		}

		msg = vstrf(str, list);
		log = strf("| %s | [%d] %s : %s", &name_buf, timestamp, levels[level], &msg);

		logs.push_back(log);
		*/
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
