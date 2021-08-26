#ifndef HIRZEL_ASSERT_H
#define HIRZEL_ASSERT_H

#define STRI(x) #x
#define STR(x) STRI(x)

#include <iostream>

namespace hirzel
{
	inline void assertion_failure(const char *expr, const char *msg)
	{
		std::cerr << __FILE__ " @ " STR(__LINE__) ": Assertion ";
		if (expr)
			std::cerr << "'" << expr << "' ";
		std::cerr << "failed! " << msg << std::endl;
		std::abort();
	}
}

#endif
