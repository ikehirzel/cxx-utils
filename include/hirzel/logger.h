#ifndef HIRZEL_LOGGER_H
#define HIRZEL_LOGGER_H

#ifdef HIRZEL_IMPLEMENT
#undef HIRZEL_IMPLEMENT
#include <hirzel/data.h>
#define HIRZEL_IMPLEMENT
#else
#include <hirzel/data.h>
#endif

#include <vector>
#include <mutex>

namespace hirzel
{
	class Logger
	{
	private:

		static std::mutex _mtx;
		static std::vector<std::string> _logs;
		static std::string _log_filename;
		static size_t _max_log_count;
		static bool _print_logs;
		static bool _debug_mode;

		static const char *_debug_color;
		static const char *_info_color;
		static const char *_success_color;
		static const char *_warning_color;
		static const char *_error_color;
		static const char *_fatal_color;

	public:

		static void init(bool debug_mode = true, bool print_logs = true,
			const std::string& log_filename = "", size_t max_log_count = 0);
		static std::string format(const std::string& str,
			const std::initializer_list<Data>& args = {});
		static void print(const std::string& str, const std::initializer_list<Data>& args = {});
		static void log(const char *tag, const std::string& label, const std::string& str,
			const std::initializer_list<Data>& args);

		inline static void debug(const std::string& label, const std::string& msg,
			const std::initializer_list<Data>& args = {})
		{
			std::fputs(_debug_color, stdout);
			log("[DEBUG]  ", label, msg, args);
			std::fputs("\033[0m", stdout);
		}

		inline static void info(const std::string& label, const std::string& msg,
			const std::initializer_list<Data>& args = {})
		{
			std::fputs(_info_color, stdout);
			log("[INFO]   ", label, msg, args);
			std::fputs("\033[0m", stdout);
		}

		inline static void success(const std::string& label, const std::string& msg,
			const std::initializer_list<Data>& args = {})
		{
			std::fputs(_success_color, stdout);
			log("[SUCCESS]", label, msg, args);
			std::fputs("\033[0m", stdout);
		}

		inline static void warning(const std::string& label, const std::string& msg,
			const std::initializer_list<Data>& args = {})
		{
			std::fputs(_warning_color, stdout);
			log("[WARNING]", label, msg, args);
			std::fputs("\033[0m", stdout);
		}

		inline static void error(const std::string& label, const std::string& msg,
			const std::initializer_list<Data>& args = {})
		{
			std::fputs(_error_color, stdout);
			log("[ERROR]  ", label, msg, args);
			std::fputs("\033[0m", stdout);
		}


		inline static void fatal(const std::string& label, const std::string& msg,
			const std::initializer_list<Data>& args = {})
		{
			std::fputs(_fatal_color, stdout);
			log("[FATAL]  ", label, msg, args);
			std::fputs("\033[0m", stdout);
		}

		inline static void set_debug_color(const char * const ansi_escape_color) noexcept
		{
			Logger::_debug_color = ansi_escape_color;
		}

		inline static void set_info_color(const char * const ansi_escape_color) noexcept
		{
			Logger::_info_color = ansi_escape_color;
		}

		inline static void set_success_color(const char * const ansi_escape_color) noexcept
		{
			Logger::_success_color = ansi_escape_color;
		}

		inline static void set_warning_color(const char * const ansi_escape_color) noexcept
		{
			Logger::_warning_color = ansi_escape_color;
		}

		inline static void set_error_color(const char * const ansi_escape_color) noexcept
		{
			Logger::_error_color = ansi_escape_color;
		}

		inline static void set_fatal_color(const char * const ansi_escape_color) noexcept
		{
			Logger::_fatal_color = ansi_escape_color;
		}


		static void dump_logs();

		inline static const std::vector<std::string>& logs()
		{
			return _logs;
		}

	private:

		std::string _label;

	public:
		Logger(const std::string& label) : _label(label) {}

		inline void debug(const std::string& msg, const std::initializer_list<Data>& args = {}) const
		{
			return Logger::debug(_label, msg, args);
		}

		inline void info(const std::string& msg, const std::initializer_list<Data>& args = {}) const
		{
			return Logger::info(_label, msg, args);
		}

		inline void success(const std::string& msg, const std::initializer_list<Data>& args = {}) const
		{
			return Logger::success(_label, msg, args);
		}

		inline void warning(const std::string& msg, const std::initializer_list<Data>& args = {}) const
		{
			return Logger::warning(_label, msg, args);
		}

		inline void error(const std::string& msg, const std::initializer_list<Data>& args = {}) const
		{
			return Logger::error(_label, msg, args);
		}

		inline void fatal(const std::string& msg, const std::initializer_list<Data>& args = {}) const
		{
			return Logger::fatal(_label, msg, args);
		}	
	};
}

#endif // HIRZEL_LOGGER_H

#ifdef HIRZEL_IMPLEMENT

// standard library
#include <ctime>
#include <fstream>
#include <iostream>
#include <mutex>

// environment libraries
#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

// external libraries
#include <hirzel/data.h>

#define color(x) "\033[" #x "m"

#define LOGGER_RESET		color(0)
#define LOGGER_BLUE			color(34)
#define LOGGER_GREEN		color(32)
#define LOGGER_YELLOW		color(33)
#define LOGGER_RED			color(31)
#define LOGGER_BRIGHT_RED	color(1;31)

#define LOGGER_NAME_BUF_LEN 24

#ifndef LOGGER_TAB_SIZE
#define LOGGER_TAB_SIZE 4
#endif

#define LOGGER_INT		'd'
#define LOGGER_UINT		'u'
#define LOGGER_OCTAL	'o'
#define LOGGER_BINARY	'b'
#define LOGGER_HEXUPPER	'X'
#define LOGGER_HEXLOWER	'x'
#define LOGGER_FLOAT 	'f'
#define LOGGER_BOOL		't'
#define LOGGER_STRING	's'
#define LOGGER_CHAR		'c'
#define LOGGER_PERCENT	'%'
#define LOGGER_PTRLOWER	'p'
#define LOGGER_PTRUPPER	'P'

namespace hirzel
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

	std::mutex Logger::_mtx;
	std::vector<std::string> Logger::_logs;
	std::string Logger::_log_filename;
	size_t Logger::_max_log_count;
	bool Logger::_print_logs;
	bool Logger::_debug_mode;
	const char *Logger::_debug_color = LOGGER_BLUE;
	const char *Logger::_info_color = LOGGER_RESET;
	const char *Logger::_success_color = LOGGER_GREEN;
	const char *Logger::_warning_color = LOGGER_YELLOW;
	const char *Logger::_error_color = LOGGER_RED;
	const char *Logger::_fatal_color = LOGGER_BRIGHT_RED;

	std::string Logger::format(const std::string &str, const std::initializer_list<Data>& args)
	{
		char out[512];
		char* outp = out;
		std::string tmp;

		auto arg_iterator = args.begin();

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
				throw std::invalid_argument("No data type was supplied for '%'");
				*outp = 0;
				return out;
			}
			
			if (str[i] == '%')
			{
				*outp++ = '%';
				continue;
			}

			if (arg_iterator == args.end())
				throw std::invalid_argument("not enough args supplied!");			

			switch (str[i])
			{
			case LOGGER_INT:
				tmp = std::to_string(arg_iterator->as_int());
				break;

			case LOGGER_UINT:
				tmp = details::utos(arg_iterator->as_int(), 10, true);
				break;

			case LOGGER_OCTAL:
				tmp = "0o" + details::utos(arg_iterator->as_int(), 8, true);
				break;

			case LOGGER_BINARY:
				tmp = "Ob" + details::utos(arg_iterator->as_int(), 2, true);
				break;

			case LOGGER_HEXUPPER:
				tmp = "0x" + details::utos(arg_iterator->as_int(), 16, true);
				break;

			case LOGGER_HEXLOWER:
				tmp = "0x" + details::utos(arg_iterator->as_int(), 16, false);
				break;

			case LOGGER_FLOAT:
				tmp = std::to_string(arg_iterator->as_float());
				break;

			case LOGGER_BOOL:
				tmp = arg_iterator->as_bool() ? "true" : "false";
				break;

			case LOGGER_STRING:
				tmp = arg_iterator->as_string();
				break;

			case LOGGER_CHAR:
				// incrementing Data_index here because this skips where it would naturally be done
				*outp++ = arg_iterator->as_int();
				arg_iterator += 1;
				continue;

			default:
				throw std::invalid_argument("invalid argument type given");
			}

			// contents of tmp will be copied into out
			for (unsigned j = 0; j < tmp.size(); j++)
			{
				*outp++ = tmp[j];
			}

			arg_iterator += 1;
		}

		*outp = 0;
		return std::string(out);
	}

	void Logger::print(const std::string &str, const std::initializer_list<Data>& args)
	{
		std::string out = format(str, args);
		std::fputs(out.c_str(), stdout);
	}

	void Logger::init(bool debug_mode, bool print_logs, const std::string& log_filename, size_t max_log_count)
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

	void Logger::log(const char *tag, const std::string& label, const std::string &str,
		const std::initializer_list<Data> &args)
	{
		std::string msg = format(str, args);

		time_t currtime;
		char timebuf[17];

		std::time(&currtime);
		std::strftime(timebuf, 16, "%T", localtime(&currtime));
		
		std::string log = format("[%s] %s %s : %s\n", { timebuf, tag, label, msg });

		if (!_log_filename.empty())
		{
			_mtx.lock();
			_logs.push_back(log);
			if (_logs.size() >= _max_log_count)
				dump_logs();
			_mtx.unlock();
		}

		if (_print_logs)
		{
			_mtx.lock();
			std::fputs(log.c_str(), stdout);
			_mtx.unlock();
		}
	}

	void Logger::dump_logs()
	{
		if (_log_filename.empty())
			throw std::invalid_argument("output filename must set before dumping logs");

		if (_logs.empty())
			return;

		std::ofstream file(_log_filename, std::ios_base::app);

		if (!file.is_open())
			throw std::runtime_error("failed to open log file");

		for (const std::string& l : _logs)
			file << l;

		_logs.clear();
		file.close();
	}
}

#endif // HIRZEL_IMPLEMENTATION