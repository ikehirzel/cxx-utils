#include <hirzel/system.hpp>

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
	const OsType currentOsType = OsType::HIRZEL_CURRENT_OS;
	const char* currentOsName = HIRZEL_STR(HIRZEL_CURRENT_OS);
}
