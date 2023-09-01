#ifndef HIRZEL_SYSTEM_HPP
#define HIRZEL_SYSTEM_HPP

#include <string>
#include <hirzel/primitives.hpp>

#ifdef _WIN32
#define HIRZEL_DIRECTORY_SEPARATOR	('\\')
#define HIRZEL_PATH_SEPARATOR		(':')
#else
#define HIRZEL_DIRECTORY_SEPARATOR	('/')
#define HIRZEL_PATH_SEPARATOR		(';')
#endif


#if defined(_WIN32)
#define HIRZEL_CURRENT_OS	Windows
#elif defined(__linux__)
#define HIRZEL_CURRENT_OS	Linux
#elif defined(__APPLE__)
#define HIRZEL_CURRENT_OS	Apple
#elif defined(__FreeBSD__)
#define HIRZEL_CURRENT_OS	FreeBSD
#elif defined(__ANDROID__)
#define HIRZEL_CURRENT_OS	Android
#else
#define HIRZEL_CURRENT_OS	Unknown
#endif

#define HIRZEL_STRI(x)	(#x)
#define HIRZEL_STR(x) (HIRZEL_STRI(x))

namespace hirzel::system
{
	enum class OsType
	{
		Windows,
		Linux,
		Apple,
		FreeBSD,
		Android,
		Unknown
	};

	constexpr const OsType currentOsType = OsType::HIRZEL_CURRENT_OS;
	constexpr const char* currentOsName = HIRZEL_STR(HIRZEL_CURRENT_OS);
	constexpr const char pathSeparator = HIRZEL_PATH_SEPARATOR;
	constexpr const char directorySeparator = HIRZEL_DIRECTORY_SEPARATOR;

	std::string pathToCurrentExecutable();
	std::string pathToCurrentExecutableDirectory();
}

#undef HIRZEL_STR
#undef HIRZEL_STRI

#endif
