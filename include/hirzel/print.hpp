#ifndef HIRZEL_PRINT_HPP
#define HIRZEL_PRINT_HPP

#include <iostream>
#include <sstream>
#include <stdexcept>

namespace hirzel
{
	inline void output(std::ostream& out, const char *str)
	{
		const char *iter = str;

		while (true)
		{
			switch (iter[0])
			{
				case '\\':
				{
					auto c = iter[1];

					if (c == '\0')
						throw std::runtime_error("There is no character to escape.");

					out.write(str, iter - str);
					out << c;
					iter += 2;
					str = iter;
					continue;
				}

				case '$':
					throw std::runtime_error("There is no argument given.");

				case '\0':
					out.write(str, iter - str);
					return;

				default:
					iter += 1;
					continue;
			}
		}
	}

	template <typename First, typename... Arg>
	void output(std::ostream& out, const char *fmt, const First& first, Arg const&... args)
	{
		const char *iter = fmt;

		while (true)
		{
			switch (*iter)
			{
				case '\\':
					out.write(fmt, iter - fmt);

					out << iter[1];

					iter += 2;
					fmt = iter;
					continue;

				case '$':
					break;

				case '\0':
					throw std::runtime_error("No format specifier was given for argument");

				default:
					iter += 1;
					continue;
			}

			break;
		}

		out.write(fmt, iter - fmt);

		if (std::is_floating_point<First>::value)
			out << std::fixed;
		
		out << first;

		iter += 1;

		output(out, iter, args...);
	}

	template <typename... Arg>
	std::string format(const char *fmt, Arg const&... args)
	{
		std::stringstream out;

		output(out, fmt, args...);

		return out.str();
	}

	template <typename... Arg>
	void print(const char *fmt, Arg const&... args)
	{
		output(std::cout, fmt, args...);
	}

	template <typename... Arg>
	void println(const char * const fmt, Arg const&... args)
	{
		output(std::cout, fmt, args...);

		std::cout << std::endl;
	}
}

#endif
