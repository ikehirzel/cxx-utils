#ifndef HIRZEL_SYSTEM_HPP
#define HIRZEL_SYSTEM_HPP

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

	extern const OsType currentOsType;
	extern const char* currentOsName;
}

#endif
