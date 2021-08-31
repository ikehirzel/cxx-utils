#ifndef HIRZEL_ASSERT_H
#define HIRZEL_ASSERT_H

#include <iostream>
#include <string>

namespace hirzel
{

	void _assertion_failure(const char *file, unsigned line, const char *expr, const std::string& msg)
	{
		std::cerr << file << " @ " << line << ": Assertion '" << expr << "' failed! " << msg << std::endl;
		std::abort();
	}

	template <typename T>
	void _assert_throws(void (*function)(), const char *file, unsigned line, const char *type, const char *expr)
	{
		try {
			function();
			_assertion_failure(file, line, expr, "Expected to catch " + std::string(type) + " but nothing was thrown");
		} catch (const T& e) {
			return;
		} catch (const std::exception& e) {
			_assertion_failure(file, line, expr, "Expected to catch " + std::string(type) + " but caught unhandled exception: " + e.what());
		} catch (...) {
			_assertion_failure(file, line, expr, "Expected to catch " + std::string(type) + " but caught unknown error");
		}
	}

	void _assert_no_throw(void (*function)(), const char *file, unsigned line, const char *expr)
	{
		try {
			function();
		} catch (const std::exception& e) {\
			_assertion_failure(file, line, expr, "Expected no errors but caught unhandled exception: " + std::string(e.what()));\
		} catch (...) {\
			_assertion_failure(file, line, expr, "Expected no errors but caught unknown error");
		}
	}

	#define assert_throws(expr, type) hirzel::_assert_throws<type>([](){ expr; }, __FILE__, __LINE__, #type, #expr)
	#define assert_no_throw(expr) hirzel::_assert_no_throw([](){expr;}, __FILE__, __LINE__, #expr)
	#define assert_true(expr) if (!(expr)) hirzel::_assertion_failure(__FILE__, __LINE__, #expr, "Expected true but got false")
	#define assert_false(expr) if ((expr)) hirzel::_assertion_failure(__FILE__, __LINE__, #expr, "Expected false but got true")
	#define assert_true_msg(expr, msg) if (!(expr)) hirzel::_assertion_failure(__FILE__, __LINE__, #expr, msg)
	#define test(name) std::cout << "Testing " #name "...\n"; test_##name(); std::cout << "\t\tAll tests passed\n";
}

#endif
