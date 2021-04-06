#ifndef HIRZEL_LOGGER_H
#define HIRZEL_LOGGER_H

#include <hirzel/data.h>
#include <vector>
#define LOGGER_DEBUG	0
#define LOGGER_INFO		1
#define LOGGER_SUCCESS	2
#define LOGGER_WARNING	3
#define LOGGER_ERROR	4
#define LOGGER_FATAL	5

namespace hirzel
{
	namespace logger
	{
		// specifies the output file and on windows enables VT100 control codes
		void init(bool debug_mode = true, bool print_logs = true,
			const std::string& log_filename = "", size_t max_log_count = 0);
		// returns a formatted string
		std::string format(const std::string& str, const std::vector<Data>& Datas = {});
		// prints string in specified fromat but does not push it to log list, essentially a type safe printf Dataiant
		void print(const std::string& str, const std::vector<Data>& Datas = {});
		// prints / pushes log based on settings
		void log(unsigned level, const char* name, int line, const std::string& str, const std::vector<Data>& vars);
		// flushes the list into specified output file and empties the list
		bool dump();
		// const ref to the log list for parsing
		const std::vector<std::string>& logs();
		const char* error();
	}
}

// checks if the user wants to use the macros as they may or may not cause name collisions
#ifndef HIRZEL_LOGGER_NO_MACROS

// macro for printFmt for convenience as it allows for variadic use
#define PRINT(msg, ...)		hirzel::logger::print(msg, { __VA_ARGS__ });
#define DEBUG(msg, ...)		hirzel::logger::log(LOGGER_DEBUG, __FILE__, __LINE__, msg, { __VA_ARGS__} )
#define INFO(msg, ...)		hirzel::logger::log(LOGGER_INFO, __FILE__, __LINE__, msg, { __VA_ARGS__} )
#define SUCCESS(msg, ...)	hirzel::logger::log(LOGGER_SUCCESS, __FILE__, __LINE__, msg, { __VA_ARGS__ })
#define WARNING(msg, ...)	hirzel::logger::log(LOGGER_WARNING, __FILE__, __LINE__, msg, { __VA_ARGS__ })
#define ERROR(msg, ...)		hirzel::logger::log(LOGGER_ERROR, __FILE__, __LINE__, msg, { __VA_ARGS__ })
#define FATAL(msg, ...)		hirzel::logger::log(LOGGER_FATAL, __FILE__, __LINE__, msg, { __VA_ARGS__ })

#endif // HIRZEL_LOGGER_NO_MACROS

#endif // HIRZEL_LOGGER_H

#ifdef HIRZEL_LOGGER_I
#undef HIRZEL_LOGGER_I

#define HIRZEL_DATA_I
#include <hirzel/data.h>

#include <ctime>
#include <fstream>
#include <iostream>
#include <mutex>
#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#define LOGGER_DEBUG_STR	"[DEBUG]  "
#define LOGGER_INFO_STR		"[INFO]   "
#define LOGGER_SUCCESS_STR	"[SUCCESS]"
#define LOGGER_WARNING_STR	"[WARNING]"
#define LOGGER_ERROR_STR	"[ERROR]  "
#define LOGGER_FATAL_STR	"[FATAL]  "

#define LOGGER_LEVELS\
	{\
		LOGGER_DEBUG_STR, LOGGER_INFO_STR, LOGGER_SUCCESS_STR,\
			LOGGER_WARNING_STR, LOGGER_ERROR_STR, LOGGER_FATAL_STR\
	}

#define color(x) "\033["#x"m"

#define LOGGER_RESET		color(0)
#define LOGGER_WHITE		color(37)
#define LOGGER_GREEN		color(32)
#define LOGGER_YELLOW		color(33)
#define LOGGER_RED		color(1;31)
#define LOGGER_BRIGHT_RED	color(31)
#define LOGGER_BLUE		color(34)

#define LOGGER_COLORS\
	{\
		 LOGGER_BLUE, LOGGER_RESET, LOGGER_GREEN,\
			LOGGER_YELLOW, LOGGER_RED, LOGGER_BRIGHT_RED\
	}

#define LOGGER_NAME_BUF_LEN 24

#ifndef LOGGER_TAB_SIZE
#define LOGGER_TAB_SIZE 4
#endif

#define LOGGER_INT		'd'
#define LOGGER_UINT		'u'
#define LOGGER_OCTAL		'o'
#define LOGGER_BINARY		'b'
#define LOGGER_HEXUPPER	'X'
#define LOGGER_HEXLOWER	'x'
#define LOGGER_FLOAT 		'f'
#define LOGGER_BOOL		't'
#define LOGGER_STRING		's'
#define LOGGER_CHAR		'c'
#define LOGGER_PERCENT	'%'
#define LOGGER_PTRLOWER	'p'
#define LOGGER_PTRUPPER	'P'

namespace hirzel
{
	namespace logger
	{
		namespace details
		{
			std::string utos(uintmax_t num, unsigned base, bool upper)
			{
				if (num == 0) return "0";

				char tmp[sizeof(uintmax_t)*8 + 1];
				char out[sizeof(uintmax_t)*8 + 1];
				char* tmpp = tmp;
				char* outp = out;
				const char* charset;

				if(upper)
				{
					charset = "0123456789ABCDEF";
				}
				else
				{
					charset = "0123456789abcdef";
				}

				while (num > 0)
				{
					*tmpp++ = charset[num % base];
					num /= base;
				}
			
				tmpp--;

				while (tmpp > tmp - 1)
				{
					*outp++ = *tmpp--;
				}

				*outp = 0;

				return std::string(out);
			}
		}

		std::mutex _mtx;
		const char* _error = nullptr;
		std::string _log_filename;
		std::vector<std::string> _logs;
		size_t _max_log_count = 0;
		bool _print_logs = false;
		bool _debug_mode = false;
		const char *_colors[] = LOGGER_COLORS;
		const char *_levels[] = LOGGER_LEVELS;

		std::string format(const std::string &str, const std::vector<Data>& objs)
		{
			char out[512];
			char* outp = out;
			std::string tmp;

			unsigned obj_index = 0;
			for (size_t i = 0; i < str.size(); i++)
			{
				if (str[i] != '%')
				{
					// adding char to output
					*outp++ = str[i];
					continue;
				}

				// incrementing to symbol associated with %
				i++;
				
				// no symbol was given
				if (i >= str.size())
				{
					throw std::invalid_argument("logger::format(): No Data type was supplied for '%'");
					*outp = 0;
					return out;
				}
				
				if (str[i] == '%')
				{
					*outp++ = '%';
					continue;
				}

				// there are no more Datas in queue for this
				if (obj_index >= objs.size())
				{
					throw std::invalid_argument("logger::format(): Not enough Datas supplied!");
					*outp = 0;
					return out;
				}

				switch (str[i])
				{
				case LOGGER_INT:
					tmp = std::to_string(objs[obj_index].to_int());
					break;

				case LOGGER_UINT:
					tmp = details::utos(objs[obj_index].to_uint(), 10, true);
					break;

				case LOGGER_OCTAL:
					tmp = "0o" + details::utos(objs[obj_index].to_uint(), 8, true);
					break;

				case LOGGER_BINARY:
					tmp = "Ob" + details::utos(objs[obj_index].to_uint(), 2, true);
					break;

				case LOGGER_HEXUPPER:
					tmp = "0x" + details::utos(objs[obj_index].to_uint(), 16, true);
					break;

				case LOGGER_HEXLOWER:
					tmp = "0x" + details::utos(objs[obj_index].to_uint(), 16, false);
					break;

				case LOGGER_FLOAT:
					tmp = std::to_string(objs[obj_index].to_double());
					break;

				case LOGGER_BOOL:
					tmp = objs[obj_index].to_bool() ? "true" : "false";
					break;

				case LOGGER_STRING:
					tmp = objs[obj_index].to_string();
					break;

				case LOGGER_CHAR:
					// incrementing Data_index here because this skips where it would naturally be done
					*outp++ = objs[obj_index++].to_char();
					continue;

				default:
					throw std::invalid_argument("logger::format(): invalid arguemnt type given");
					*outp = 0;
					return out;
				}

				// contents of tmp will be copied into out
				for (unsigned j = 0; j < tmp.size(); j++)
				{
					*outp++ = tmp[j];
				}

				obj_index++;
			}

			*outp = 0;
			return std::string(out);
		}

		void print(const std::string &str, const std::vector<Data> &Datas)
		{
			std::string out = format(str, Datas);
			for (size_t i = 0; i < out.size(); i++)
			{
				putchar(out[i]);
			}
		}

		void init(bool debug_mode, bool print_logs, const std::string& log_filename, size_t max_log_count)
		{
			_log_filename = log_filename;
			_print_logs = print_logs;
			_max_log_count = max_log_count;
			_debug_mode = debug_mode;
			if (!_log_filename.empty())
			{
				std::ofstream file;
				file.open(_log_filename);
				if (file.is_open())
				{
					file << "---- START OF LOG FILE ----\n";
				}
			}
#if defined(_WIN32) || defined(_WIN64)
			DWORD outMode = 0;
			HANDLE outHandle = GetStdHandle(STD_OUTPUT_HANDLE);
			GetConsoleMode(outHandle, &outMode);
			SetConsoleMode(outHandle, outMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif
		}

		void log(unsigned level, const char *name, int line, const std::string &str, const std::vector<Data> &list)
		{
			if(level == LOGGER_DEBUG && !_debug_mode)
			{
				return;
			}

			time_t currtime;
			std::time(&currtime);
			char timebuf[17];
			std::strftime(timebuf, 16, "%T", localtime(&currtime));
			std::string name_str, log, msg, line_buf;
			std::string name_buf(LOGGER_NAME_BUF_LEN, ' ');

			line_buf = std::to_string(line);
			name_str = std::string(name) + ':' + line_buf;
			name_str = name_buf + name_str;
			int spaces = LOGGER_NAME_BUF_LEN - name_str.size();

			for (int i = 0; i < LOGGER_NAME_BUF_LEN; i++)
			{
				name_buf[i] = name_str[i - spaces];
			}

			for (char &c : name_buf)
			{
				if (c == 0 || c == 32)
				{
					c = '.';
				}
			}
			
			msg = format(str, list);
			log = format("%s| %s | [%s] %s : %s" LOGGER_RESET "\n", { _colors[level], name_buf, std::string(timebuf), _levels[level], msg });

			if (!_log_filename.empty())
			{
				_mtx.lock();
				_logs.push_back(log);
				if (_logs.size() >= _max_log_count)
				{

					dump();
				}
				_mtx.unlock();
			}

			if (_print_logs)
			{
				_mtx.lock();
				for (int i = 0; i < log.size(); i++)
				{
					std::putchar(log[i]);
				}
				_mtx.unlock();
			}
		}

		bool dump()
		{
			if (_log_filename.empty())
			{
				_error = "The output filename has not been set";
				return false;
			}

			if (_logs.empty())
			{
				_error = "There were no logs to dump";
				return false;
			}

			std::ofstream file;
			file.open(_log_filename, std::ios_base::app);

			if (!file.is_open())
			{
				_error = "The file failed to open";
				return false;
			}

			for (std::string l : _logs)
			{
				std::string::iterator start = l.begin(), end = l.end();

				while(*start != '|')
				{
					start++;
				}

				while(*end != '\n')
				{
					end--;
				}

				file << std::string(start, end + 1);
			}

			_logs.clear();
			file.close();
			return true;
		}

		const std::vector<std::string>& logs() { return _logs; }
		const char* error() { return _error; }
	}
}

#endif // HIRZEL_LOGGER_IMPLEMENTATION