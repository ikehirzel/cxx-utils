#define HIRZEL_IMPLEMENT
#include <hirzel/data/validation.h>

#include <iostream>

#include "assert.h"

using namespace hirzel;

#define assert_no_errors(fmt, ...) assert_true_msg(Validator(fmt)(__VA_ARGS__).empty(), "Format " #fmt " produces error(s) with argument: " #__VA_ARGS__)
#define assert_has_errors(fmt, ...) assert_true_msg(Validator(fmt)(__VA_ARGS__).size(), "Format " #fmt " produces no errors with argument: " #__VA_ARGS__)
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

	assert_fmt_throws("%[1,0]");

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
	assert_has_errors("[]", Array { 1 });
	assert_has_errors("[]", Data());
	assert_has_errors("[]", 1);

	assert_no_errors("[]?", Array());
	assert_no_errors("[]?", Data());
	assert_has_errors("[]?", Array{ 1 });
	assert_has_errors("[]?", 1);

	assert_no_errors("[#]", Array { 1 });
	assert_has_errors("[#]", Array { 1, 2 });
	assert_has_errors("[#]", Array { "hello" });
	assert_has_errors("[#]", Array { Data() });

	assert_no_errors("[#]?", Array { 1 });
	assert_has_errors("[#]?", Array { 1, 2 });
	assert_has_errors("[#]?", Array { "hello" });
	assert_has_errors("[#]?", Array { Data() });

	assert_no_errors("[#?]", Array { Data() });
	assert_has_errors("[#?]", Array { "hello" });
	assert_no_errors("[#?, #?, #]", Array { Data(), Data(), 3});

	assert_no_errors("[#?]?", Array { Data() });
	assert_has_errors("[#?]?", Array { "hello" });
	assert_no_errors("[#?]?", Data());

	assert_no_errors("[#...]", Array());
	assert_no_errors("[#...]", Array { 1 });
	assert_no_errors("[#...]", Array { 1, 2, 3 });
	assert_has_errors("[#...]", Array { "hello" });
	assert_has_errors("[#...]", Array { 1, 2, "hello" });
	
	assert_no_errors("[#, #...]", Array { 1 });
	assert_no_errors("[#, #...]", Array { 1, 2 });
	assert_no_errors("[#, #...]", Array { 1, 2, 234, 109 });
	assert_has_errors("[#, #...]", Array());	

	// nullable variadic
	assert_no_errors("[#?...]", Array{ });
	assert_no_errors("[#?...]", Array{ Data() });
	assert_no_errors("[#?...]", Array{ 1 });
	assert_no_errors("[#?...]", Array{ 1, Data(), 2, Data() });
	assert_no_errors("[#?...]", Array{ Data(), Data(), Data() });
	assert_has_errors("[#?...]", Array { "hello" });
	assert_has_errors("[#?...]", Array { 1, "hello" });
	assert_has_errors("[#?...]", Array { Data(), "hello" });
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

void test_misc()
{
	using Table = Data::Table;
	using Array = Data::Array;

	auto data = Table {
		{ "array_key", Array({ 1, 2, 3, 4}) },
		{ "bool", Data() },
		{ "nullable_int", "hello" },
		{ "sub_table", Table {
				{ "a", true }
			}
		}
	};

	Validator is_valid(R"==(
		{
			array_key: [#[1, 3]...],
			bool: &,
			nullable_int: #?,
			sub_table: {
				a: #
			}
		}
	)==");

	auto errors = is_valid(data);

	for (auto err : errors)
		std::cout << err << std::endl;
}

int main()
{
	test(integer);
	test(decimal);
	test(boolean);
	test(string);
	test(array);
	test(table);
	test(form);
	//test(misc);

	return 0;
}
