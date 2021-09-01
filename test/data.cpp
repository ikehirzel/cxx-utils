#define HIRZEL_IMPLEMENT
#include <hirzel/data/data.h>

#include <iostream>
#include <fstream>

#include "assert.h"

using namespace hirzel::data;

#define assert_cast_values(data, int_val, float_val, bool_val, string_val)\
	assert_true(data.as_int() == int_val);\
	assert_true(data.as_double() == float_val);\
	assert_true(data.as_bool() == bool_val);\
	assert_true(data.as_string() == string_val);

#define assert_primitive(data, typename, funcname, value)\
	assert_true(data.type() == typename);\
	assert_true(data.is_##funcname());\
	assert_true(data.size() == 1)

#define assert_not_primitive(data, typename, funcname)\
	assert_true(data.type() != typename);\
	assert_true(!data.is_##funcname())

#define assert_not_null(data)		assert_true(data.type() != Data::Type::NONE); assert_true(!data.is_null());
#define assert_not_bool(data)		assert_not_primitive(data, Data::Type::BOOLEAN, boolean)
#define assert_not_int(data)		assert_not_primitive(data, Data::Type::INTEGER, integer)
#define assert_not_float(data)		assert_not_primitive(data, Data::Type::DECIMAL, decimal)
#define assert_not_string(data)		assert_not_primitive(data, Data::Type::STRING, string)
#define assert_not_array(data)		assert_true(data.type() != Data::Type::ARRAY); assert_true(!data.is_array())
#define assert_not_table(data)		assert_true(data.type() != Data::Type::TABLE); assert_true(!data.is_table())

#define assert_equals(a, b) {\
	assert_true(a == b);\
	assert_true(b == a);\
	assert_false(a != b);\
	assert_false(b != a);\
}

#define assert_not_equals(a, b) {\
	assert_false(a == b);\
	assert_false(b == a);\
	assert_true(a != b);\
	assert_true(b != a);\
}

#define assert_null(data) {\
	assert_true(data.type() == Data::Type::NONE);\
	assert_true(data.is_null());\
	assert_true(data.size() == 0);\
	assert_cast_values(data, 0, 0.0, false, "null");\
	assert_not_int(data);\
	assert_not_float(data);\
	assert_not_bool(data);\
	assert_not_string(data);\
	assert_not_array(data);\
	assert_not_table(data);\
	assert_equals(data, Data());\
	assert_equals(data, Data(Data::Type::NONE));\
}

#define assert_boolean(data, value) {\
	assert_primitive(data, Data::Type::BOOLEAN, boolean, value);\
	assert_cast_values(data, (int)value, (double)value, value, (value ? "true" : "false"));\
	assert_not_null(data);\
	assert_not_int(data);\
	assert_not_float(data);\
	assert_not_string(data);\
	assert_not_array(data);\
	assert_not_table(data);\
	assert_no_throw(Data(value).get_boolean());\
	assert_equals(data, Data(value));\
}


#define assert_integer(data, value) {\
	assert_primitive(data, Data::Type::INTEGER, integer, value);\
	assert_cast_values(data, value, (double)value, (bool)value, std::to_string(value));\
	assert_true(data.is_number());\
	assert_not_null(data);\
	assert_not_bool(data);\
	assert_not_float(data);\
	assert_not_string(data);\
	assert_not_array(data);\
	assert_not_table(data);\
	assert_equals(data, Data(value));\
}

#define assert_decimal(data, value) {\
	assert_primitive(data, Data::Type::DECIMAL, decimal, value);\
	assert_cast_values(data, (int)value, (double)value, (bool)value, std::to_string(value));\
	assert_true(data.is_number());\
	assert_not_null(data);\
	assert_not_int(data);\
	assert_not_bool(data);\
	assert_not_string(data);\
	assert_not_array(data);\
	assert_not_table(data);\
	assert_equals(data, Data(value));\
}

#define assert_string(data, int_value, float_value, bool_value, string_value) {\
	assert_true(data.type() == Data::Type::STRING);\
	assert_true(std::string(data.type_name()) == "string");\
	assert_true(data.is_string());\
	assert_true(data.size() == data.as_string().size());\
	assert_cast_values(data, int_value, float_value, bool_value, string_value);\
	assert_not_null(data);\
	assert_not_int(data);\
	assert_not_float(data);\
	assert_not_bool(data);\
	assert_not_array(data);\
	assert_not_table(data);\
	assert_equals(data, Data(string_value));\
}

#define assert_array(data, value) {\
	assert_true(data.type() == Data::Type::ARRAY);\
	assert_true(data.is_array());\
	assert_not_null(data);\
	assert_not_int(data);\
	assert_not_float(data);\
	assert_not_bool(data);\
	assert_not_string(data);\
	assert_not_table(data);\
	assert_equals(data, Data(value));\
}

#define assert_table(data, value) {\
	assert_true(data.type() == Data::Type::TABLE);\
	assert_true(data.is_table());\
	assert_not_null(data);\
	assert_not_bool(data);\
	assert_not_int(data);\
	assert_not_float(data);\
	assert_not_string(data);\
	assert_not_array(data);\
	assert_equals(data, Data(value));\
}

void test_null()
{
	Data init = Data();
	assert_null(init);

	Data copy(init);
	assert_null(copy);
	assert_equals(copy, init);
	copy = init;
	assert_null(copy);
	assert_equals(copy, init);

	Data move(std::move(init));
	assert_null(move);
	assert_equals(move, copy);
	Data clone = copy;
	move = std::move(copy);
	assert_null(move);
	assert_equals(move, clone);

	assert_null(Data());
	assert_null(Data(Data::Type::NONE));

	assert_not_equals(move, Data(143));
	assert_not_equals(move, Data(234.82));
	assert_not_equals(move, Data("hello"));
	assert_not_equals(move, Data(true));
	assert_not_equals(move, Data(Data::Array()));
	assert_not_equals(move, Data(Data::Table()));
}

void test_integer()
{
	Data init = 234;
	assert_integer(init, 234);

	Data copy(init);
	assert_integer(copy, 234);
	assert_equals(copy, init);
	copy = init;
	assert_integer(copy, 234);
	assert_equals(copy, init);

	Data move(std::move(init));
	assert_integer(move, 234);
	assert_equals(move, copy);
	Data clone = copy;
	move = std::move(copy);
	assert_integer(move, 234);
	assert_equals(move, clone);

	assert_integer(Data(0), 0);
	assert_integer(Data(-0), -0);
	assert_integer(Data(1), 1);
	assert_integer(Data(-1), -1);
	assert_integer(Data(4L), 4L);
	assert_integer(Data(4U), 4U);
	assert_integer(Data(-1023), -1023);
	assert_integer(Data(62), 62);

	assert_not_equals(move, Data());
	assert_not_equals(move, Data(130));
	assert_not_equals(move, Data(-130));
	assert_not_equals(move, Data(-130.52));
	assert_not_equals(move, Data("hello"));
	assert_not_equals(move, Data(false));
	assert_not_equals(move, Data(Data::Type::ARRAY));
	assert_not_equals(move, Data(Data::Type::TABLE));
}

void test_decimal()
{
	Data init = 2.34;
	assert_decimal(init, 2.34);

	Data copy(init);
	assert_decimal(copy, 2.34);
	assert_equals(copy, init);
	copy = init;
	assert_decimal(copy, 2.34);
	assert_equals(copy, init);

	Data move(std::move(init));
	assert_decimal(move, 2.34);
	assert_equals(move, copy);
	Data clone = copy;
	move = std::move(copy);
	assert_decimal(move, 2.34);
	assert_equals(move, clone);

	assert_decimal(Data(Data::Type::DECIMAL), 0.0);
	assert_decimal(Data(1.34), 1.34);
	assert_decimal(Data(0.0f), 0.0f);
	assert_decimal(Data(-0.0), -0.0);
	assert_decimal(Data(-123592.235), -123592.235);
	assert_decimal(Data(2.0f), 2.0f);

	assert_not_equals(move, Data());
	assert_not_equals(move, Data(130));
	assert_not_equals(move, Data(-130.52));
	assert_not_equals(move, Data("hello"));
	assert_not_equals(move, Data(false));
	assert_not_equals(move, Data(Data::Type::ARRAY));
	assert_not_equals(move, Data(Data::Type::TABLE));
}

void test_boolean()
{
	Data init = true;
	assert_boolean(init, true);
	
	Data copy(init);
	assert_boolean(copy, true);
	assert_equals(copy, init);
	copy = init;
	assert_boolean(copy, true);
	assert_equals(copy, init);

	Data move(std::move(init));
	assert_boolean(move, true);
	assert_equals(move, copy);
	Data clone = copy;
	move = std::move(copy);
	assert_boolean(move, true);
	assert_equals(move, clone);

	assert_boolean(Data(Data::Type::BOOLEAN), false);
	assert_boolean(Data(true), true);
	assert_boolean(Data(false), false);

	assert_not_equals(move, Data());
	assert_not_equals(move, Data(130));
	assert_not_equals(move, Data(-130.52));
	assert_not_equals(move, Data("hello"));
	assert_not_equals(move, Data(false));
	assert_not_equals(move, Data(Data::Type::ARRAY));
	assert_not_equals(move, Data(Data::Type::TABLE));
}

void test_string()
{
	Data init = "this is a string";
	assert_string(init, 0, 0.0, true, "this is a string");

	Data copy(init);
	assert_string(copy, 0, 0.0, true, "this is a string");
	assert_equals(copy, init);
	copy = init;
	assert_string(copy, 0, 0.0, true, "this is a string");
	assert_equals(copy, init);

	Data move(std::move(init));
	assert_string(move, 0, 0.0, true, "this is a string");
	assert_equals(move, copy);
	Data clone = copy;
	move = std::move(copy);
	assert_string(move, 0, 0.0, true, "this is a string");
	assert_equals(move, clone);

	assert_string(Data(Data::Type::STRING), 0, 0.0, false, "");
	assert_string(Data(""), 0, 0.0, false, "");
	assert_string(Data("hello"), 0, 0.0, true, "hello");
	assert_string(Data("1.2"), 1, 1.2, true, "1.2");
	assert_string(Data("123"), 123, 123.0, true, "123");
	assert_string(Data("0"), 0, 0.0, true, "0");
	assert_string(Data("0000"), 0, 0.0, true, "0000");

	assert_not_equals(move, Data());
	assert_not_equals(move, Data(130));
	assert_not_equals(move, Data(-130.52));
	assert_not_equals(move, Data("hello"));
	assert_not_equals(move, Data(false));
	assert_not_equals(move, Data(Data::Type::ARRAY));
	assert_not_equals(move, Data(Data::Type::TABLE));
}

void test_array()
{
	using Array = Data::Array;

	{
		Data empty = Array {};
		assert_array(empty, Array());
		assert_true(empty.size() == 0);
		assert_true(empty.is_empty());

		Data copy(empty);
		assert_array(copy, Array());
		assert_true(copy.size() == 0);
		assert_true(copy.is_empty());
		assert_equals(copy, empty);
		copy = empty;
		assert_array(copy, Array());
		assert_true(copy.size() == 0);
		assert_true(copy.is_empty());
		assert_equals(copy, empty);

		Data move(std::move(empty));
		assert_array(move, Array());
		assert_true(move.size() == 0);
		assert_true(move.is_empty());
		assert_equals(move, copy);
		Data clone = copy;
		move = std::move(copy);
		assert_array(move, Array());
		assert_true(move.size() == 0);
		assert_true(move.is_empty());
		assert_equals(move, clone);

		assert_not_equals(move, Data());
		assert_not_equals(move, Data(130));
		assert_not_equals(move, Data(-130));
		assert_not_equals(move, Data(-130.52));
		assert_not_equals(move, Data("hello"));
		assert_not_equals(move, Data(false));
		assert_equals(move, Data(Data::Type::ARRAY));
		assert_not_equals(move, Data(Data::Array { 1, 2, 3}));
		assert_not_equals(move, Data(Data::Type::TABLE));
	}

	{
		Data init = Array { "hello", 1, true, 0.4 };
		assert_array(init, Array({ "hello", 1, true, 0.4 }));
		assert_true(init.size() == 4);
		assert_true(!init.is_empty());
		assert_string(init[0], 0, 0.0, true, "hello");
		assert_integer(init[1], 1);
		assert_boolean(init[2], true);
		assert_decimal(init[3], 0.4);

		Data copy(init);
		assert_array(copy, Array({ "hello", 1, true, 0.4 }));
		assert_true(copy.size() == 4);
		assert_true(!copy.is_empty());
		assert_string(copy[0], 0, 0.0, true, "hello");
		assert_integer(copy[1], 1);
		assert_boolean(copy[2], true);
		assert_decimal(copy[3], 0.4);
		assert_equals(copy, init);
		copy = init;
		assert_array(copy, Array({ "hello", 1, true, 0.4 }));
		assert_true(copy.size() == 4);
		assert_true(!copy.is_empty());
		assert_string(copy[0], 0, 0.0, true, "hello");
		assert_integer(copy[1], 1);
		assert_boolean(copy[2], true);
		assert_decimal(copy[3], 0.4);
		assert_equals(copy, init);

		Data move(std::move(init));
		assert_array(move, Array({ "hello", 1, true, 0.4 }));
		assert_true(move.size() == 4);
		assert_true(!move.is_empty());
		assert_string(move[0], 0, 0.0, true, "hello");
		assert_integer(move[1], 1);
		assert_boolean(move[2], true);
		assert_decimal(move[3], 0.4);
		assert_equals(move, copy);
		Data clone = copy;
		move = std::move(copy);
		assert_array(move, Array({ "hello", 1, true, 0.4 }));
		assert_true(move.size() == 4);
		assert_true(!move.is_empty());
		assert_string(move[0], 0, 0.0, true, "hello");
		assert_integer(move[1], 1);
		assert_boolean(move[2], true);
		assert_decimal(move[3], 0.4);
		assert_equals(move, clone);

		assert_not_equals(move, Data());
		assert_not_equals(move, Data(130));
		assert_not_equals(move, Data(-130));
		assert_not_equals(move, Data(-130.52));
		assert_not_equals(move, Data("hello"));
		assert_not_equals(move, Data(false));
		assert_not_equals(move, Data(Data::Type::ARRAY));
		assert_not_equals(move, Data(Data::Type::TABLE));
	}

	Data array = Array { 1, 2.0f, "3", false, Data() };
	assert_array(array, Array({ 1, 2.0f, "3", false, Data() }));
	assert_true(array.size() == 5);
	assert_true(!array.is_empty());
	assert_integer(array[0], 1);
	assert_decimal(array[1], 2.0f);
	assert_string(array[2], 3, 3.0, true, "3");
	assert_boolean(array[3], false);
	assert_null(array[4]);
}

void test_table()
{
	using Table = Data::Table;

	{
		Data empty = Table {};
		assert_table(empty, Table());
		assert_true(empty.size() == 0);
		assert_true(empty.is_empty());

		Data copy(empty);
		assert_table(copy, Table());
		assert_true(copy.size() == 0);
		assert_true(copy.is_empty());
		assert_equals(copy, empty);
		copy = empty;
		assert_table(copy, Table());
		assert_true(copy.size() == 0);
		assert_true(copy.is_empty());
		assert_equals(copy, empty);

		Data move(std::move(empty));
		assert_table(move, Table());
		assert_true(move.size() == 0);
		assert_true(move.is_empty());
		assert_equals(move, copy);
		Data clone = copy;
		move = std::move(copy);
		assert_table(move, Table());
		assert_true(move.size() == 0);
		assert_true(move.is_empty());
		assert_equals(move, clone);

		assert_not_equals(move, Data());
		assert_not_equals(move, Data(130));
		assert_not_equals(move, Data(-130));
		assert_not_equals(move, Data(-130.52));
		assert_not_equals(move, Data("hello"));
		assert_not_equals(move, Data(false));
		assert_not_equals(move, Data(Data::Type::ARRAY));
		assert_equals(move, Data(Data::Type::TABLE));
		assert_not_equals(move, Data(Table { { "key", "value" } }));
	}
	{
		auto table = Table {
			{ "boolean", true },
			{ "integer", 1362 },
			{ "decimal", 235.125 },
			{ "string", "this is text" },
			{ "null", Data() }
		}; 
		Data init = table;
		assert_table(init, table);
		assert_true(init.size() == 5);
		assert_true(!init.is_empty());
		assert_boolean(init["boolean"], true);
		assert_integer(init["integer"], 1362);
		assert_decimal(init["decimal"], 235.125);
		assert_string(init["string"], 0, 0.0, true, "this is text");
		assert_null(init["null"]);

		Data copy(init);
		assert_table(copy, table);
		assert_true(copy.size() == 5);
		assert_false(copy.is_empty());
		assert_equals(copy, init);
		copy = init;
		assert_table(copy, table);
		assert_true(copy.size() == 5);
		assert_false(copy.is_empty());
		assert_equals(copy, init);

		Data move(std::move(init));
		assert_table(move, table);
		assert_true(move.size() == 5);
		assert_false(move.is_empty());
		assert_equals(move, copy);
		Data clone = copy;
		move = std::move(copy);
		assert_table(move, table);
		assert_true(move.size() == 5);
		assert_false(move.is_empty());
		assert_equals(move, clone);

		assert_not_equals(move, Data());
		assert_not_equals(move, Data(130));
		assert_not_equals(move, Data(-130));
		assert_not_equals(move, Data(-130.52));
		assert_not_equals(move, Data("hello"));
		assert_not_equals(move, Data(false));
		assert_not_equals(move, Data(Data::Type::ARRAY));
		assert_not_equals(move, Data(Data::Type::TABLE));
	}
}

int main()
{
	test(null);
	test(integer);
	test(decimal);
	test(boolean);
	test(string);
	test(array);
	test(table);

	return 0;
}