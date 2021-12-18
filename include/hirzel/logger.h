#ifndef HIRZEL_LOGGER_H
#define HIRZEL_LOGGER_H

#include <vector>
#include <mutex>
#include <cstring>
#include <fstream>
#include <iostream>

namespace hirzel
{
	class Logger
	{
	public:
		class Arg
		{
		public:
			enum ArgType
			{
				ARG_INTEGER,
				ARG_UNSIGNED,
				ARG_FLOATING,
				ARG_CHARACTER,
				ARG_BOOLEAN,
				ARG_STRING,
				ARG_ARRAY
			};

			struct Array
			{
				Arg *data;
				size_t size;
			};

		private:
			union
			{
				long long _integer;
				unsigned long long _unsigned;
				double _floating;
				char _character;
				bool _boolean;
				char *_string;
				Array _array;
			};

			ArgType _type;

			inline Arg() : Arg(0) {}

		public:

			inline Arg(long long i) : _type(ARG_INTEGER), _integer(i) {}
			inline Arg(long i) : Arg((long long)i) {}
			inline Arg(int i) : Arg((long long)i) {}
			inline Arg(short i) : Arg((long long)i) {}

			inline Arg(unsigned long long u) : _type(ARG_UNSIGNED), _unsigned(u) {}
			inline Arg(unsigned long u) : Arg((unsigned long long)u) {}
			inline Arg(unsigned int u) : Arg((unsigned long long)u) {}
			inline Arg(unsigned short u) : Arg((unsigned long long)u) {}

			inline Arg(double f) : _type(ARG_FLOATING), _floating((double)f) {}
			inline Arg(float f) : Arg((double)f) {}

			inline Arg(char c) : _type(ARG_CHARACTER), _character(c) {}

			inline Arg(bool b) : _type(ARG_BOOLEAN), _boolean(b) {}

			inline Arg(const char *s) : _type(ARG_STRING), _string(new char[std::strlen(s) + 1])
			{
				std::strcpy(_string, s);
			}

			inline Arg(char *s) : Arg((const char *)s) {}

			inline Arg(const std::string& s) : Arg(s.c_str()) {}

			inline Arg(const Arg *data, size_t size) : _type(ARG_ARRAY)
			{
				_array.size = size;
				_array.data = new Arg[size];

				for (size_t i = 0; i < _array.size; ++i)
					_array.data[i] = data[i];
			}

			inline Arg(const Array& a) : Arg(a.data, a.size) {}

			inline Arg(const std::vector<Arg>& a) : _type(ARG_ARRAY)
			{

				_array.size = a.size();
				_array.data = new Arg[a.size()];

				for (size_t i = 0; i < a.size(); ++i)
					_array.data[i] = a[i];
			}

			inline Arg(Arg&& other) { *this = other; }
			inline Arg(const Arg& other) { *this = other; }

			inline ~Arg()
			{
				switch (_type)
				{
					case ARG_STRING:
						delete[] _string;
						return;
					case ARG_ARRAY:
						delete[] _array.data;
						return;
				}
			}

			inline long long as_integer() const { return _integer; }
			inline unsigned long long as_unsigned() const { return _unsigned; }
			inline double as_floating() const { return _floating; }
			inline char as_character() const { return _character; }
			inline bool as_boolean() const { return _boolean; }
			inline const char *as_string() const { return _string; }
			inline const Array as_array() const { return _array; }

			inline std::string to_string() const
			{
				switch (_type)
				{
					case ARG_INTEGER:
						return std::to_string(_integer);
					case ARG_UNSIGNED:
						return std::to_string(_unsigned);
					case ARG_FLOATING:
						return std::to_string(_floating);
					case ARG_CHARACTER:
						return std::string(1, _character);
					case ARG_BOOLEAN:
						return _boolean ? "true" : "false";
					case ARG_STRING:
						return std::string(_string);
					case ARG_ARRAY:
					{
						std::string out = "[";
						if (_array.size)
						{
							out += _array.data[0].to_string();
							for (size_t i = 1; i < _array.size; ++i)
								out += ", " + _array.data[i].to_string();
						}
						return out + "]";
					}
					default:
						throw std::invalid_argument("Arg is an invalid type");
				}
			}

			inline ArgType type() const
			{
				return _type;
			}

			inline Arg& operator=(Arg&& other)
			{
				_type = other._type;
				switch (_type)
				{
					case ARG_INTEGER:
						_integer = other._integer;
						return *this;
					case ARG_UNSIGNED:
						_unsigned = other._unsigned;
						return *this;
					case ARG_FLOATING:
						_floating = other._floating;
						return *this;
					case ARG_CHARACTER:
						_character = other._character;
						return *this;
					case ARG_BOOLEAN:
						_boolean = other._boolean;
						return *this;

					case ARG_STRING:
						_string = other._string;
						other._string = nullptr;
						other._type = ARG_INTEGER;
						return *this;

					case ARG_ARRAY:
						_array = other._array;
						other._array.data = nullptr;
						other._type = ARG_INTEGER;
						return *this;

					default:
						throw std::invalid_argument("Arg is an invalid type");
				}
			}

			inline Arg& operator=(const Arg& other)
			{
				_type = other.type();
				switch (_type)
				{
					case ARG_INTEGER:
						return *this = other.as_integer();
					case ARG_UNSIGNED:
						return *this = other.as_unsigned();
					case ARG_FLOATING:
						return *this = other.as_floating();
					case ARG_CHARACTER:
						return *this = other.as_character();
					case ARG_BOOLEAN:
						return *this = other.as_boolean();
					case ARG_STRING:
						return *this = other.as_string();
					case ARG_ARRAY:
						return *this = Arg(other.as_array());
					default:
						throw std::invalid_argument("Arg is an invalid type");
				}
			}
		};

	private:

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

	public:

		static void init_log_file(const std::string& log_filepath);
		static void log(const char *color, const char *tag, const std::string& label,
			const std::string& str, const std::vector<Arg>& args);

		static std::string format(const std::string& str,
			const std::vector<Arg>& args = {});
		static void print(const std::string& str, const std::vector<Arg>& args = {});
		static void println(const std::string& str, const std::vector<Arg>& args = {});		

		inline static void log_debug(const std::string& label, const std::string& msg,
			const std::vector<Arg>& args = {})
		{
			log(_debug_color, "  DEBUG", label, msg, args);
		}

		inline static void log_info(const std::string& label, const std::string& msg,
			const std::vector<Arg>& args = {})
		{
			log(_info_color, "   INFO", label, msg, args);
		}

		inline static void log_success(const std::string& label, const std::string& msg,
			const std::vector<Arg>& args = {})
		{
			log(_success_color, "SUCCESS", label, msg, args);
		}

		inline static void log_warning(const std::string& label, const std::string& msg,
			const std::vector<Arg>& args = {})
		{
			log(_warning_color, "WARNING", label, msg, args);
			std::fputs("\033[0m", stdout);
		}

		inline static void log_error(const std::string& label, const std::string& msg,
			const std::vector<Arg>& args = {})
		{
			log(_error_color, "  ERROR", label, msg, args);
		}

		inline static void log_fatal(const std::string& label, const std::string& msg,
			const std::vector<Arg>& args = {})
		{
			log(_fatal_color, "  FATAL", label, msg, args);
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

		static void enable_log_printing(bool enable);
		static void enable_debug_mode(bool enable);
		static void enable_color(bool enable);

	private:

		std::string _label;

	public:

		Logger() : _label("") {}
		Logger(const std::string& label) : _label(label) {}

		inline void debug(const std::string& msg, const std::vector<Arg>& args = {}) const
		{
			Logger::log_debug(_label, msg, args);
		}

		inline void info(const std::string& msg, const std::vector<Arg>& args = {}) const
		{
			Logger::log_info(_label, msg, args);
		}

		inline void success(const std::string& msg, const std::vector<Arg>& args = {}) const
		{

			Logger::log_success(_label, msg, args);
		}

		inline void warning(const std::string& msg, const std::vector<Arg>& args = {}) const
		{
			Logger::log_warning(_label, msg, args);
		}

		inline void error(const std::string& msg, const std::vector<Arg>& args = {}) const
		{
			Logger::log_error(_label, msg, args);
		}

		inline void fatal(const std::string& msg, const std::vector<Arg>& args = {}) const
		{
			Logger::log_fatal(_label, msg, args);
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
#define OS_IS_WINDOWS true
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
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

			if (out.size() > 1 && out.back() == '\\')
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

				if (specified_index >= args.size())
					throw std::invalid_argument("invalid argument position given in format: {"
						+ specified_index_str
						+ "}");

				pos = num_literal_end;	
				arg_to_print_index = specified_index;
			}
			else
			{
				if (arg_index >= args.size())
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
			HANDLE outHandle = GetStdHandle(STD_OUTPUT_HANDLE);
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
