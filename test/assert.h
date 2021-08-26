#ifndef HIRZEL_ASSERT_H
#define HIRZEL_ASSERT_H

#define STRI(x) #x
#define STR(x) STRI(x)

#include <iostream>

namespace hirzel
{
	inline void assertion_failure(const char *msg)
	{
		std::cerr << __FILE__ " @ " STR(__LINE__) ": " << msg << std::endl;
		std::abort();
	}

	#define assert_true(expr) {\
		if (!(expr))\
			hirzel::assertion_failure("Assertion '" #expr "' == true failed!");\
	}

	#define assert_false(expr) {\
		if (expr)\
			hirzel::assertion_failure("Assertion '" #expr "' == false failed!");\
	}

	#define assert_throws(expr, type) {\
		try {\
			expr;\
			hirzel::assertion_failure("Assertion '" #expr "' failed! Expected to catch " #type " but nothing was thrown");\
		} catch (const type& e) {\
		} catch (const std::exception& e) {\
			hirzel::assertion_failure("Assertion '" #expr "' failed! Expected " #type " but caught unhandled exception");\
		} catch (...) {\
			hirzel::assertion_failure("Assertion '" #expr "' failed! Expected " #type " but caught unknown error");\
		}\
	}

	#define assert_no_throw(expr, type)\
		try {\
			expr;\
		} catch (const type& e) {\
			hirzel::assertion_failure("Assertion '" #expr "' failed! Expected not to catch, but caught " #type);\
		} catch (const std::exception& e) {\
			hirzel::assertion_failure("Assertion '" #expr "' failed! Expected not to catch, but caught unhandled exception");\
		} catch (...) {\
			hirzel::assertion_failure("Assertion '" #expr "' failed! Expected not to catch, but caught unknown error");\
		}\
	}
}

#endif
