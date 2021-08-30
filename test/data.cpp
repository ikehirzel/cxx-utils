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

#define assert_not_null(data)		assert_true(data.type() != Data::NULL_TYPE); assert_true(!data.is_null());
#define assert_not_bool(data)		assert_not_primitive(data, Data::BOOLEAN_TYPE, boolean)
#define assert_not_int(data)		assert_not_primitive(data, Data::INTEGER_TYPE, integer)
#define assert_not_float(data)		assert_not_primitive(data, Data::DECIMAL_TYPE, decimal)
#define assert_not_string(data)		assert_not_primitive(data, Data::STRING_TYPE, string)
#define assert_not_array(data)		assert_true(data.type() != Data::ARRAY_TYPE); assert_true(!data.is_array())
#define assert_not_table(data)		assert_true(data.type() != Data::TABLE_TYPE); assert_true(!data.is_table())


#define assert_null(data) {\
	assert_true(data.type() == Data::NULL_TYPE);\
	assert_true(data.is_null());\
	assert_true(data.size() == 0);\
	assert_cast_values(data, 0, 0.0, false, "null");\
	assert_not_int(data);\
	assert_not_float(data);\
	assert_not_bool(data);\
	assert_not_string(data);\
	assert_not_array(data);\
	assert_not_table(data);\
}

#define assert_boolean(arg, value) {\
	Data data = arg;\
	assert_primitive(data, Data::BOOLEAN_TYPE, boolean, value);\
	assert_cast_values(data, (int)value, (double)value, value, (value ? "true" : "false"));\
	assert_not_null(data);\
	assert_not_int(data);\
	assert_not_float(data);\
	assert_not_string(data);\
	assert_not_array(data);\
	assert_not_table(data);\
	assert_no_throw(Data(value).get_boolean());\
}


#define assert_integer(arg, value) {\
	Data data = arg;\
	assert_primitive(data, Data::INTEGER_TYPE, integer, value);\
	assert_cast_values(data, value, (double)value, (bool)value, std::to_string(value));\
	assert_true(data.is_number());\
	assert_not_null(data);\
	assert_not_bool(data);\
	assert_not_float(data);\
	assert_not_string(data);\
	assert_not_array(data);\
	assert_not_table(data);\
}

#define assert_decimal(arg, value) {\
	Data data = arg;\
	assert_primitive(data, Data::DECIMAL_TYPE, decimal, value);\
	assert_cast_values(data, (int)value, (double)value, (bool)value, std::to_string(value));\
	assert_true(data.is_number());\
	assert_not_null(data);\
	assert_not_int(data);\
	assert_not_bool(data);\
	assert_not_string(data);\
	assert_not_array(data);\
	assert_not_table(data);\
}

#define assert_string(arg, int_value, float_value, bool_value, string_value) {\
	Data data = arg;\
	assert_true(data.type() == Data::STRING_TYPE);\
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
}

#define assert_array(arg) {\
	Data data = arg;\
	assert_true(data.type() == Data::ARRAY_TYPE);\
	assert_true(data.is_array());\
	assert_not_null(data);\
	assert_not_int(data);\
	assert_not_float(data);\
	assert_not_bool(data);\
	assert_not_string(data);\
	assert_not_table(data);\
}

#define assert_table(arg) {\
	Data data = arg;\
	assert_true(data.type() == Data::TABLE_TYPE);\
	assert_true(data.is_table());\
	assert_not_null(data);\
	assert_not_bool(data);\
	assert_not_int(data);\
	assert_not_float(data);\
	assert_not_string(data);\
	assert_not_array(data);\
}

void test_null()
{
	assert_null(Data());

	Data basic;

	assert_null(basic);

	Data init = Data();

	assert_null(init);

	Data init1 = Data(Data::Type::NULL_TYPE);

	assert_null(init1);

	Data copy = 1;
	copy = Data();

	assert_null(copy);

	Data copy1 = true;
	copy1 = copy;

	assert_null(copy1);

	Data move = "hello";
	move = std::move(Data());

	assert_null(move);

	Data move1 = Data::Array { 1, 2, 3};
	Data to_be_moved;
	move1 = std::move(to_be_moved);

	assert_null(move1);
}

void test_integer()
{
	assert_integer(0, 0);
	assert_integer(-0, -0);
	assert_integer(1, 1);
	assert_integer(-1, -1);
	assert_integer(4L, 4L);
	assert_integer(4U, 4U);
	assert_integer(-1023, -1023);
	assert_integer(62, 62);

	Data init = 234;

	assert_integer(init, 234);

	Data copy = "hello";
	copy = 258;

	assert_copy(copy, 258);

	assign = 958;
	Data assign1 = 

}

void test_decimal()
{
	assert_decimal(1.34, 1.34);
	assert_decimal(0.0f, 0.0f);
	assert_decimal(-0.0, -0.0);
	assert_decimal(-123592.235, -123592.235);
	assert_decimal(2.0f, 2.0f);
}

void test_boolean()
{
	assert_boolean(true, true);
	assert_boolean(false, false);
}

void test_string()
{
	assert_string("", 0, 0.0, false, "");
	assert_string("hello", 0, 0.0, true, "hello");
	assert_string("1.2", 1, 1.2, true, "1.2");
	assert_string("123", 123, 123.0, true, "123");
	assert_string("0", 0, 0.0, true, "0");
	assert_string("0000", 0, 0.0, true, "0000");
}

void test_array()
{
	using Array = Data::Array;

	Data empty = Array {};

	assert_array(empty);
	assert_true(empty.size() == 0);
	assert_true(empty.is_empty());

	Data array = Array { "hello", 1, true, 0.4 };

	assert_true(array.is_array());
	assert_true(array.size() == 4);
	assert_true(!array.is_empty());

	assert_string(array[0], 0, 0.0, true, "hello");
	assert_integer(array[1], 1);
	assert_boolean(array[2], true);
	assert_decimal(array[3], 0.4);
}

void test_table()
{
	// Data assign;

	using Table = Data::Table;

	Data empty = Table {};

	assert_table(empty);
	assert_true(empty.size() == 0);
	assert_true(empty.is_empty());

	Data table = Table {
		{ "boolean", true },
		{ "integer", 1362 },
		{ "decimal", 235.125 },
		{ "string", "this is text" },
		{ "null", Data() }
	};

	assert_table(table);
	assert_true(table.size() == 5);
	assert_true(!table.is_empty());

	assert_boolean(table["boolean"], true);
	assert_integer(table["integer"], 1362);
	assert_decimal(table["decimal"], 235.125);
	assert_string(table["string"], 0, 0.0, true, "this is text");
	assert_null(table["null"]);
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