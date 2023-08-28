#ifndef HIRZEL_STRING_HPP
#define HIRZEL_STRING_HPP

#include <string>
#include <string_view>

namespace hirzel
{
	namespace string
	{
		bool is_invisible(unsigned char c) noexcept
		{
			return c <= ' ';
		}

		bool is_alpha(unsigned char c) noexcept
		{
			return (c >= 'A'  && c <= 'Z') || (c >= 'a' && c <= 'z');
		}

		bool is_digit(unsigned char c) noexcept
		{
			return c >= '0' && c <= '9';
		}

		bool is_special(unsigned char c) noexcept
		{
			return (c >= '!' && c <= '/') || (c >= ':' && c <= '@')
				|| (c >= '[' && c <= '`') || (c >= '{' && c <= '~');
		}

		std::string get_folder(const char *filepath) noexcept;
		std::string get_filename(const char *filepath, bool include_extension = true) noexcept;
		std::string get_extension(const char *filepath) noexcept;
	}
}

#endif

#if defined(HIRZEL_IMPLEMENT) && !defined(HIRZEL_STRING_I)
#define HIRZEL_STRING_I

#if defined(_WIN32) || defined(WIN32) || defined(WIN64)
#define HIRZEL_STRING_WINDOWS true
#else
#define HIRZEL_STRING_WINDOWS false
#endif

namespace hirzel
{
	namespace string
	{
		size_t get_last_slash_index(const char *filepath) noexcept
		{
			size_t slash_index = 0;

			for (size_t i = 0; filepath[i]; ++i)
			{
				bool is_dir_char = filepath[i] == '/';
				#if HIRZEL_STRING_WINDOWS
				is_dir_char = is_dir_char || filepath[i] == '\\';
				#endif

				if (is_dir_char)
					slash_index = i + 1;
			}

			return slash_index;
		}

		size_t get_first_dot_index(const char *filepath) noexcept
		{
			size_t size = 0;

			while (filepath[size])
			{
				if (filepath[size] == '.')
					break;

				++size;
			}

			return size;
		}

		std::string get_folder(const char *filepath) noexcept
		{
			size_t slash_index = get_last_slash_index(filepath);

			return slash_index > 0
				? std::string(filepath, slash_index)
				: "./";
		}

		std::string get_filename(const char *filepath, bool include_extension) noexcept
		{
			size_t slash_index = get_last_slash_index(filepath);
			const char *filename = filepath + slash_index;

			size_t size = 0;

			if (include_extension)
			{
				while (filename[size])
					++size;
			}
			else
			{
				size = get_first_dot_index(filename);
			}

			return std::string(filename, size);
		}

		std::string get_extension(const char *filepath) noexcept
		{
			size_t slash_index = get_last_slash_index(filepath);
			const char *filename = filepath + slash_index;
			
			size_t dot_index = get_first_dot_index(filename);
			const char *extension = filename + dot_index;
			
			size_t size = 0;

			while (extension[size])
				++size;

			return std::string(extension, size);
		}
	}
}

#endif