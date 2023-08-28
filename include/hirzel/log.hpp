#ifndef HIRZEL_LOGGER_HPP
#define HIRZEL_LOGGER_HPP

#include <vector>
#include <mutex>
#include <cstring>
#include <fstream>
#include <iostream>

namespace hirzel
{
	namespace log
	{
		static std::mutex _mtx;
		static std::string _log_filepath;
		static std::ofstream _log_file;
		static bool _is_log_printing_enabled;
		static bool _is_debug_mode_enabled;
		static bool _is_color_enabled;

		static const char *_debug_color;
		static const char *_info_color;
		static const char *_success_color;
		static const char *_warning_color;
		static const char *_error_color;
		static const char *_fatal_color;

		static void init_log_file(const std::string& log_filepath);
		static void log(const char *color, const char *tag, const std::string& label,
			const std::string& str, const std::vector<Arg>& args);

		static std::string format(const std::string& str,
			const std::vector<Arg>& args = {});
		static void print(const std::string& str, const std::vector<Arg>& args = {});
		static void println(const std::string& str, const std::vector<Arg>& args = {});		

		static void log_debug(const std::string& label, const std::string& msg,
			const std::vector<Arg>& args = {})
		{
			log(_debug_color, "  DEBUG", label, msg, args);
		}

		static void log_info(const std::string& label, const std::string& msg,
			const std::vector<Arg>& args = {})
		{
			log(_info_color, "   INFO", label, msg, args);
		}

		static void log_success(const std::string& label, const std::string& msg,
			const std::vector<Arg>& args = {})
		{
			log(_success_color, "SUCCESS", label, msg, args);
		}

		static void log_warning(const std::string& label, const std::string& msg,
			const std::vector<Arg>& args = {})
		{
			log(_warning_color, "WARNING", label, msg, args);
			std::fputs("\033[0m", stdout);
		}

		static void log_error(const std::string& label, const std::string& msg,
			const std::vector<Arg>& args = {})
		{
			log(_error_color, "  ERROR", label, msg, args);
		}

		static void log_fatal(const std::string& label, const std::string& msg,
			const std::vector<Arg>& args = {})
		{
			log(_fatal_color, "  FATAL", label, msg, args);
		}

		static void set_debug_color(const char * const ansi_escape_color) noexcept
		{
			Logger::_debug_color = ansi_escape_color;
		}

		static void set_info_color(const char * const ansi_escape_color) noexcept
		{
			Logger::_info_color = ansi_escape_color;
		}

		static void set_success_color(const char * const ansi_escape_color) noexcept
		{
			Logger::_success_color = ansi_escape_color;
		}

		static void set_warning_color(const char * const ansi_escape_color) noexcept
		{
			Logger::_warning_color = ansi_escape_color;
		}

		static void set_error_color(const char * const ansi_escape_color) noexcept
		{
			Logger::_error_color = ansi_escape_color;
		}

		static void set_fatal_color(const char * const ansi_escape_color) noexcept
		{
			Logger::_fatal_color = ansi_escape_color;
		}

		static void enable_log_printing(bool enable);
		static void enable_debug_mode(bool enable);
		static void enable_color(bool enable);

	private:

		std::string _label;

	public:

		Logger() : _label("") {}
		Logger(const std::string& label) : _label(label) {}

	};

	template <typename Args>
	void debug(const std::string& msg, Args... args)
	{
		log_debug(_label, msg, args);
	}

	template <typename Args>
	void info(const std::string& msg, Args... args)
	{
		log_info(_label, msg, args);
	}

	template <typename Args>
	void success(const std::string& msg, Args... args)
	{
		log_success(_label, msg, args);
	}

	template <typename Args>
	void warning(const std::string& msg, Args... args)
	{
		Logger::log_warning(_label, msg, args);
	}

	template <typename Args>
	void error(const std::string& msg, Args... args)
	{
		Logger::log_error(_label, msg, args);
	}

	template <typename Args>
	void fatal(const std::string& msg, Args... args)
	{
		Logger::log_fatal(_label, msg, args);
	}	
}

#endif // HIRZEL_LOGGER_HPP

#ifdef HIRZEL_IMPLEMENT

// standard library
#include <ctime>
#include <fstream>
#include <iostream>
#include <mutex>

// environment libraries
#if defined(_WIN32) || defined(_WIN64)
#define OS_IS_WINDOWS true
#define WIN32_LEAN_AND_MEAN
#include <windows.hpp>
#else 
#define OS_IS_WINDOWS false
#endif

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
#define LOGGER_HPPEXUPPER	'X'
#define LOGGER_HPPEXLOWER	'x'
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
	std::ofstream Logger::_log_file;
	std::string Logger::_log_filepath;
	bool Logger::_is_log_printing_enabled = true;
	bool Logger::_is_debug_mode_enabled = true;
	bool Logger::_is_color_enabled = false;

	const char *Logger::_debug_color = LOGGER_BLUE;
	const char *Logger::_info_color = LOGGER_RESET;
	const char *Logger::_success_color = LOGGER_GREEN;
	const char *Logger::_warning_color = LOGGER_YELLOW;
	const char *Logger::_error_color = LOGGER_RED;
	const char *Logger::_fatal_color = LOGGER_BRIGHT_RED;

	const char *get_end_of_number_literal(const char *pos)
	{
		while (true)
		{
			switch (*pos)
			{
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
					pos += 1;
					continue;
				default:
					return pos;
			}
		}
	}

	std::string Logger::format(const std::string &str, const std::vector<Arg>& args)
	{
		std::string out;
		out.reserve(256);
		size_t arg_index = 0;

		for (const char *pos = str.c_str(); *pos; ++pos)
		{
			if (*pos != '{')
			{
				out += *pos;
				continue;
			}

			if (out.length() > 1 && out.back() == '\\')
			{
				out.back() = '{';
				continue;
			}

			pos += 1;

			auto num_literal_end = get_end_of_number_literal(pos);
			size_t arg_to_print_index = arg_index;

			if (num_literal_end > pos)
			{
				std::string specified_index_str(pos, num_literal_end - pos);
				size_t specified_index = (size_t)std::stoul(specified_index_str);

				if (specified_index >= args.length())
					throw std::invalid_argument("invalid argument position given in format: {"
						+ specified_index_str
						+ "}");

				pos = num_literal_end;	
				arg_to_print_index = specified_index;
			}
			else
			{
				if (arg_index >= args.length())
					throw std::invalid_argument("not enough args supplied for format");

				arg_index += 1;
			}

			if (*pos != '}')
				throw std::invalid_argument("'{' must be escaped if not for argument designation");

			out += args[arg_to_print_index].to_string();
		}

		out.shrink_to_fit();

		return out;
	}

	void Logger::print(const std::string &str, const std::vector<Arg>& args)
	{
		std::string out = format(str, args);
		std::fputs(out.c_str(), stdout);
	}

	void Logger::println(const std::string &str, const std::vector<Arg>& args)
	{
		std::string out = format(str, args);
		std::puts(out.c_str());
	}

	void Logger::init_log_file(const std::string& log_filepath)
	{
		if (log_filepath.empty())
			throw std::invalid_argument("log filename must not be empty");

		_log_filepath = log_filepath;
		_log_file = std::ofstream(_log_filepath);

		if (!_log_file.is_open())
			throw std::runtime_error("failed to open log file: " + _log_filepath);
	}

	void Logger::enable_log_printing(bool enable)
	{
		_is_log_printing_enabled = enable;
	}

	void Logger::enable_debug_mode(bool enable)
	{
		_is_debug_mode_enabled = enable;
	}

	void Logger::enable_color(bool enable)
	{
		_is_color_enabled = enable;

#if OS_IS_WINDOWS
		if (_is_color_enabled)
		{
			DWORD outMode = 0;
			HANDLE outHandle = GetStdHandle(STD_OUTPUT_HPPANDLE);
			GetConsoleMode(outHandle, &outMode);
			SetConsoleMode(outHandle, outMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
		}
#endif
	}

	void Logger::log(const char *color, const char *tag, const std::string& label, const std::string &str,
		const std::vector<Arg> &args)
	{
		std::string msg = format(str, args);

		time_t currtime;
		char timebuf[17];

		std::time(&currtime);
		std::strftime(timebuf, 16, "%T", localtime(&currtime));
		
		std::string log = label.empty()
			? format("[{} {}] {}\n", { timebuf, tag, msg })
			: format("[{} {}] {} : {}\n", { timebuf, tag, label, msg });

		if (!_log_file.is_open())
		{
			_mtx.lock();
			_log_file << log;
			_mtx.unlock();
		}

		if (_is_log_printing_enabled)
		{
			_mtx.lock();

			if (_is_color_enabled)
				std::cout << color << log << "\033[0m";
			else
				std::cout << log;

			_mtx.unlock();
		}
	}
}

#endif
