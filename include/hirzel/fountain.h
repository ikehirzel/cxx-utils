#ifndef FOUNTAIN_H
#define FOUNTAIN_H

#include <hirzel/obj.h>
#include <vector>
#define FOUNTAIN_DEBUG		0
#define FOUNTAIN_INFO		1
#define FOUNTAIN_SUCCESS	2
#define FOUNTAIN_WARNING	3
#define FOUNTAIN_ERROR		4
#define FOUNTAIN_FATAL		5

namespace hirzel
{
	namespace fountain
	{
		// specifies the output file and on windows enables VT100 control codes
		void init(const std::string& log_filename, bool debug_mode, bool print_logs, bool push_logs, int max_log_size);
		// returns a formatted string
		std::string format(const std::string& str, const std::vector<Obj>& Objs = {});
		// prints string in specified fromat but does not push it to log list, essentially a type safe printf Objiant
		void print(const std::string& str, const std::vector<Obj>& Objs = {});
		// prints / pushes log based on settings
		void log(unsigned level, const char* name, int line, const std::string& str, const std::vector<Obj>& Objs);
		// flushes the list into specified output file and empties the list
		bool dump();
		// const ref to the log list for parsing
		const std::vector<std::string>& logs();
		const char* error();
	}
}

// checks if the user wants to use the macros as they may or may not cause name collisions
#ifndef EXCLUDE_FOUNTAIN_MACROS

// macro for printFmt for convenience as it allows for Objiadic 
#define PRINT(msg, ...)		hirzel::fountain::print(msg, { __VA_ARGS__ });
#define DEBUG(msg, ...)		hirzel::fountain::log(FOUNTAIN_DEBUG, __FILE__, __LINE__, msg, { __VA_ARGS__} )
#define INFO(msg, ...)		hirzel::fountain::log(FOUNTAIN_INFO, __FILE__, __LINE__, msg, { __VA_ARGS__} )
#define SUCCESS(msg, ...)	hirzel::fountain::log(FOUNTAIN_SUCCESS, __FILE__, __LINE__, msg, { __VA_ARGS__ })
#define WARNING(msg, ...)	hirzel::fountain::log(FOUNTAIN_WARNING, __FILE__, __LINE__, msg, { __VA_ARGS__ })
#define ERROR(msg, ...)		hirzel::fountain::log(FOUNTAIN_ERROR, __FILE__, __LINE__, msg, { __VA_ARGS__ })
#define FATAL(msg, ...)		hirzel::fountain::log(FOUNTAIN_FATAL, __FILE__, __LINE__, msg, { __VA_ARGS__ })

#endif // EXCLUDE_FOUNTAIN_MACROS

#endif // FOUNTAIN_H

#ifdef HIRZEL_FOUNTAIN_IMPLEMENTATION
#undef HIRZEL_FOUNTAIN_IMPLEMENTATION

#define HIRZEL_OBJ_IMPLEMENTATION
#include <hirzel/obj.h>

#include <ctime>
#include <fstream>
#include <iostream>
#include <mutex>
#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#define FOUNTAIN_DEBUG_STR		"[DEBUG]  "
#define FOUNTAIN_INFO_STR		"[INFO]   "
#define FOUNTAIN_SUCCESS_STR	"[SUCCESS]"
#define FOUNTAIN_WARNING_STR	"[WARNING]"
#define FOUNTAIN_ERROR_STR		"[ERROR]  "
#define FOUNTAIN_FATAL_STR		"[FATAL]  "

#define FOUNTAIN_LEVELS                                                  \
	{                                                                    \
		FOUNTAIN_DEBUG_STR, FOUNTAIN_INFO_STR, FOUNTAIN_SUCCESS_STR,      \
			FOUNTAIN_WARNING_STR, FOUNTAIN_ERROR_STR, FOUNTAIN_FATAL_STR \
	}

#define color(x) "\033["#x"m"

#define FOUNTAIN_RESET		color(0)
#define FOUNTAIN_WHITE		color(37)
#define FOUNTAIN_GREEN		color(32)
#define FOUNTAIN_YELLOW		color(33)
#define FOUNTAIN_RED		color(1;31)
#define FOUNTAIN_BRIGHT_RED	color(31)
#define FOUNTAIN_BLUE		color(34)

#define FOUNTAIN_COLORS											\
	{															\
		 FOUNTAIN_BLUE, FOUNTAIN_RESET, FOUNTAIN_GREEN,			\
			FOUNTAIN_YELLOW, FOUNTAIN_RED, FOUNTAIN_BRIGHT_RED	\
	}

#define FOUNTAIN_NAME_BUF_LEN 24

#ifndef FOUNTAIN_TAB_SIZE
#define FOUNTAIN_TAB_SIZE 4
#endif

#define FOUNTAIN_INT		'd'
#define FOUNTAIN_UINT		'u'
#define FOUNTAIN_OCTAL		'o'
#define FOUNTAIN_BINARY		'b'
#define FOUNTAIN_HEXUPPER	'X'
#define FOUNTAIN_HEXLOWER	'x'
#define FOUNTAIN_FLOAT 		'f'
#define FOUNTAIN_BOOL		't'
#define FOUNTAIN_STRING		's'
#define FOUNTAIN_CHAR		'c'
#define FOUNTAIN_PERCENT	'%'
#define FOUNTAIN_PTRLOWER	'p'
#define FOUNTAIN_PTRUPPER	'P'

namespace hirzel
{
	namespace fountain
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
		const char *_colors[] = FOUNTAIN_COLORS;
		const char *_levels[] = FOUNTAIN_LEVELS;

		std::string format(const std::string &str, const std::vector<Obj>& objs)
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
					throw std::invalid_argument("fountain::format(): No Obj type was supplied for '%'");
					*outp = 0;
					return out;
				}
				
				if (str[i] == '%')
				{
					*outp++ = '%';
					continue;
				}

				// there are no more Objs in queue for this
				if (obj_index >= objs.size())
				{
					throw std::invalid_argument("fountain::format(): Not enough Objs supplied!");
					*outp = 0;
					return out;
				}

				switch (str[i])
				{
				case FOUNTAIN_INT:
					tmp = std::to_string(objs[obj_index].to_int());
					break;

				case FOUNTAIN_UINT:
					tmp = details::utos(objs[obj_index].to_uint(), 10, true);
					break;

				case FOUNTAIN_OCTAL:
					tmp = "0o" + details::utos(objs[obj_index].to_uint(), 8, true);
					break;

				case FOUNTAIN_BINARY:
					tmp = "Ob" + details::utos(objs[obj_index].to_uint(), 2, true);
					break;

				case FOUNTAIN_HEXUPPER:
					tmp = "0x" + details::utos(objs[obj_index].to_uint(), 16, true);
					break;

				case FOUNTAIN_HEXLOWER:
					tmp = "0x" + details::utos(objs[obj_index].to_uint(), 16, false);
					break;

				case FOUNTAIN_FLOAT:
					tmp = std::to_string(objs[obj_index].to_double());
					break;

				case FOUNTAIN_BOOL:
					tmp = objs[obj_index].to_bool() ? "true" : "false";
					break;

				case FOUNTAIN_STRING:
					tmp = objs[obj_index].to_string();
					break;

				case FOUNTAIN_CHAR:
					// incrementing Obj_index here because this skips where it would naturally be done
					*outp++ = objs[obj_index++].to_char();
					continue;

				default:
					throw std::invalid_argument("fountain::format(): invalid arguemnt type given");
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

		void print(const std::string &str, const std::vector<Obj> &Objs)
		{
			std::string out = format(str, Objs);
			for (size_t i = 0; i < out.size(); i++)
			{
				putchar(out[i]);
			}
		}

		void init(bool debug_mode = true, bool print_logs = true,
			const std::string& log_filename = "", size_t max_log_count = 0)
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

		void log(unsigned level, const char *name, int line, const std::string &str, const std::vector<Obj> &list)
		{
			if(level == FOUNTAIN_DEBUG && !_debug_mode)
			{
				return;
			}

			time_t currtime;
			std::time(&currtime);
			char timebuf[17];
			std::strftime(timebuf, 16, "%T", localtime(&currtime));
			std::string name_str, log, msg, line_buf;
			std::string name_buf(FOUNTAIN_NAME_BUF_LEN, ' ');

			line_buf = std::to_string(line);
			name_str = std::string(name) + ':' + line_buf;
			name_str = name_buf + name_str;
			int spaces = FOUNTAIN_NAME_BUF_LEN - name_str.size();

			for (int i = 0; i < FOUNTAIN_NAME_BUF_LEN; i++)
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
			log = format("%s| %s | [%s] %s : %s" FOUNTAIN_RESET "\n", { _colors[level], name_buf, std::string(timebuf), _levels[level], msg });

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

#endif // HIRZEL_FOUNTAIN_IMPLEMENTATION