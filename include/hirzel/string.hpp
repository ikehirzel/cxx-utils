#ifndef HIRZEL_STRING_HPP
#define HIRZEL_STRING_HPP

#include <string>
#include <string_view>

namespace hirzel::string
{
	inline bool isCharInvisible(char c)
	{
		return c <= ' ';
	}

	inline bool isCharAlpha(char c)
	{
		return (c >= 'A'  && c <= 'Z') || (c >= 'a' && c <= 'z');
	}

	inline bool isCharDigit(char c)
	{
		return c >= '0' && c <= '9';
	}

	inline bool isCharSpecial(char c)
	{
		return (c >= '!' && c <= '/') || (c >= ':' && c <= '@')
			|| (c >= '[' && c <= '`') || (c >= '{' && c <= '~');
	}

	std::string directoryOf(const char *filepath);
	std::string filenameOf(const char *filepath, bool includeExtension = true);
	std::string fileExtensionOf(const char *filepath);
}

#endif
