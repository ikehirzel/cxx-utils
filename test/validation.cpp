#include <hirzel/data/validation.h>

#include <iostream>
#include <cassert>

#include "assert.h"

using namespace hirzel::data;

#define STRI(x) #x
#define STR(x) STRI(x)
#define LINE STR(__LINE__)
#define LOC __FILE__ " @ " LINE ": "
#define ASSERT_FAIL LOC "Assertion failed! "

#define assert_no_errors(fmt, arg) {\
	auto errors = Validator(fmt)(arg);\
	if (errors.size()) {\
		std::string errors_glob;\
		for (auto s : errors)\
			errors_glob += "\n\t" + s;\
		std::string error_msg = "format " #fmt " produces " + std::to_string(errors.size()) + " error(s) with format " #fmt ":\n" + errors_glob;\
		hirzel::assertion_failure(error_msg.c_str());\
	}\
}

#define assert_has_errors(fmt, arg) {\
	if (Validator(fmt)(arg).empty()) {\
		hirzel::assertion_failure("Assertion failed! Expected format " #fmt " to produce errors with " #arg);\
	}\
}

#define assert_fmt_throws(fmt) assert_throws(Validator(fmt), FormatException)
#define assert_fmt_no_throw(fmt) assert_no_throw(Validator(fmt), FormatException)

void test_integer()
{
	assert_no_errors("#", 123);
	assert_has_errors("#", Data());
	assert_has_errors("#", "hello");

	assert_no_errors("#?", 123);
	assert_no_errors("#?", Data());
	assert_has_errors("#?", "hello");

	assert_no_errors("#[0,1]", 0);
	assert_no_errors("#[0,1]", 1);
	assert_has_errors("#(0,1]", 0);
	assert_has_errors("#[0,1)", 1);
	assert_has_errors("#(0,1)", 0);
	assert_has_errors("#(0,1)", 1);
	assert_no_errors("#[0,0]", 0);
	assert_has_errors("#(0,0)", 0);

	assert_no_errors("#[0,~]", 0);
	assert_no_errors("#[0,~]", 1);
	assert_no_errors("#[0,~]", LLONG_MAX);
	assert_no_errors("#[0,~)", 0);
	assert_no_errors("#[0,~)", 1);
	assert_has_errors("#[0,~)", LLONG_MAX);

	assert_has_errors("#[~,0]", 1);
	assert_no_errors("#[~,0]", 0);
	assert_no_errors("#[~,0]", -1);
	assert_no_errors("#[~,0]", -134543);
	assert_no_errors("#[~,0]", LLONG_MIN);
	assert_no_errors("#(~,0]", 0);
	assert_no_errors("#(~,0]", -1);
	assert_has_errors("#(~,0]", LLONG_MIN);
}

void test_decimal()
{
	assert_fmt_throws("%[");
	assert_fmt_throws("%[,");
	assert_fmt_throws("%[1]");
	assert_fmt_throws("%[1,");
	assert_fmt_throws("%[1,]");

	assert_fmt_throws("%[1,0]")

	assert_no_errors("%", 123);
	assert_has_errors("%", Data());
	assert_has_errors("%", "hello");

	assert_no_errors("%?", 123);
	assert_no_errors("%?", Data());
	assert_has_errors("%?", "hello");

	assert_no_errors("%[0.0,1.0]", 0.0);
	assert_no_errors("%[12,56]", 12.0);
	assert_has_errors("%(0,1]", 0);
	assert_has_errors("%[0,1)", 1);
	assert_has_errors("%(0,1)", 0);
	assert_has_errors("%(0,1)", 1);
	assert_no_errors("%[0,0]", 0);
	assert_has_errors("%(0,0)", 0);

	assert_no_errors("%[0,~]", 0.0);
	assert_no_errors("%[0,~]", 1.0);
	assert_no_errors("%[0,~]", DBL_MAX);
	assert_no_errors("%[0,~)", 0.0);
	assert_no_errors("%[0,~)", 1.0);
	assert_has_errors("%[0,~)", DBL_MAX);

	assert_has_errors("%[~,0]", 1.0);
	assert_no_errors("%[~,0]", 0.0);
	assert_no_errors("%[~,0]", -1.0);
	assert_no_errors("%[~,0]", -134543.0);
	assert_no_errors("%[~,0]", -DBL_MAX);
	assert_no_errors("%(~,0]", 0.0);
	assert_no_errors("%(~,0]", -1.0);
	assert_has_errors("%(~,0]", DBL_MAX);
}

void test_string()
{
	assert_no_errors("$", "hello");
	assert_has_errors("$", 1);
	assert_has_errors("$", Data());

	assert_no_errors("$?", Data());
	assert_no_errors("$?", "hello");
	assert_has_errors("$?", 1);
}

void test_boolean()
{
	assert_no_errors("&", true);
	assert_has_errors("&", 1);
	assert_has_errors("&", Data());

	assert_no_errors("&?", Data());
	assert_no_errors("&?", false);
	assert_has_errors("&?", 1);
}

void test_table()
{
	using Table = Data::Table;

	// formatting

	// validation
	assert_no_errors("{}", Table());
	assert_has_errors("{}", Data());
	assert_has_errors("{}", 1);
	assert_no_errors("{}", Table({{ "key", 4 }}));

	assert_no_errors("{}?", Data());
	assert_no_errors("{}?", Table());
	assert_has_errors("{}?", 1);

	assert_no_errors("{key:#}", Table({ { "key", 47 } }));
	assert_has_errors("{key:#}", Table({ { "key", Data() } }));
	assert_has_errors("{key:#}", Table({ }));

	assert_no_errors("{key:#?}", Table({{ "key", Data() }}));
	assert_no_errors("{key:#?}", Table({{ "key", 1 }}));
	assert_has_errors("{key:#?}", Table({{ "key", "hello" }}));

	assert_no_errors("{key:#,name:$}", Table({ { "key", 47 }, { "name", "Ike" } }));
	assert_has_errors("{key:#,name:$}", Table({ { "key", Data() } }));
	assert_has_errors("{key:#,name:$}", Table({ { "key", Data() }, { "name", Data() } }));
	assert_has_errors("{key:#,name:$}", Table({ }));
}

void test_array()
{
	using Array = Data::Array;

	// formatting
	assert_fmt_throws("[#..., #...]");

	// validation
	assert_no_errors("[]", Array());
	assert_has_errors("[]", Array({ 1 }));
	assert_has_errors("[]", Data());
	assert_has_errors("[]", 1);

	assert_no_errors("[]?", Array());
	assert_no_errors("[]?", Data());
	assert_has_errors("[]?", Array({ 1 }));
	assert_has_errors("[]?", 1);

	assert_no_errors("[#]", Array({ 1 }));
	assert_has_errors("[#]", Array({ 1, 2 }));
	assert_has_errors("[#]", Array({ "hello" }));
	assert_has_errors("[#]", Array({ Data() }));

	assert_no_errors("[#]?", Array({ 1 }));
	assert_has_errors("[#]?", Array({ 1, 2 }));
	assert_has_errors("[#]?", Array({ "hello" }));
	assert_has_errors("[#]?", Array({ Data() }));

	assert_no_errors("[#?]", Array({ Data() }));
	assert_has_errors("[#?]", Array({ "hello" }));
	assert_no_errors("[#?]", Array({ }));
	assert_no_errors("[#?, #?]", Array({ }));

	assert_no_errors("[#?]?", Array({ Data() }));
	assert_has_errors("[#?]?", Array({ "hello" }));
	assert_no_errors("[#?]?", Array({ }));
	assert_no_errors("[#?, #?]?", Array({ }));

	assert_no_errors("[#...]", Array());
	assert_no_errors("[#...]", Array({ 1 }));
	assert_no_errors("[#...]", Array({ 1, 2, 3 }));
	assert_has_errors("[#...]", Array({ "hello" }));
	assert_has_errors("[#...]", Array({ 1, 2, "hello" }));
	
	assert_no_errors("[#, #...]", Array({ 1 }));
	assert_no_errors("[#, #...]", Array({ 1, 2 }));
	assert_no_errors("[#, #...]", Array({ 1, 2, 234, 109 }));
	assert_has_errors("[#, #...]", Array({ }));	
}

void test_form()
{
	auto format = "{first_name:$,middle_name:$?,last_name:$,age:#[0,150],minor:&,patience:%[0,1],friends:[$, $, $]}";
	auto data = Data::Table({
		{ "first_name", "Ike" },
		{ "last_name", "Hirzel" },
		{ "age", 22 },
		{ "minor", false },
		{ "patience", 0.6 },
		{ "friends", Data::Array({ "Alex", "Jacob", "Memo" }) }
	});
	assert_no_errors(format, data);
}

#define TEST(name) std::cout << "Testing " #name "...\n"; test_##name(); std::cout << "\t\tAll tests passed\n";

int main()
{
	TEST(integer);
	TEST(decimal);
	TEST(boolean);
	TEST(string);
	TEST(array);
	TEST(table);
	TEST(form);

	return 0;
}
