#define HIRZEL_IMPLEMENT
#include <hirzel/data.hpp>

#include <iostream>
#include <fstream>
#include <cassert>

using namespace hirzel;

#define assert_no_throw(expr) {\
	try {\
		expr;\
	} catch (const std::exception&) {\
		assert(false && #expr " threw an exception");\
	}\
}

#define assert_cast_values(data, int_val, float_val, bool_val, string_val)\
	assert(data.asI32() == int_val);\
	assert(data.asF64() == float_val);\
	assert(data.asBool() == bool_val);\
	assert(data.asString() == string_val);

#define assert_primitive(data, typename, funcname, value)\
	assert(data.type() == typename);\
	assert(data.is##funcname());\
	assert(data.length() == 1)

#define assert_not_primitive(data, typename, funcname)\
	assert(data.type() != typename);\
	assert(!data.is##funcname())

#define assert_not_null(data)		assert(data.type() != DataType::Null); assert(!data.isNull());
#define assert_not_bool(data)		assert_not_primitive(data, DataType::Boolean, Boolean)
#define assert_not_int(data)		assert_not_primitive(data, DataType::Integer, Integer)
#define assert_not_float(data)		assert_not_primitive(data, DataType::Float, Float)
#define assert_not_string(data)		assert_not_primitive(data, DataType::String, String)
#define assert_not_array(data)		assert(data.type() != DataType::Array); assert(!data.isArray())
#define assert_not_table(data)		assert(data.type() != DataType::Table); assert(!data.isTable())

#define assert_equals(a, b) {\
	assert(a == b);\
	assert(b == a);\
	assert(!(a != b));\
	assert(!(b != a));\
}

#define assert_not_equals(a, b) {\
	assert(!(a == b));\
	assert(!(b == a));\
	assert(a != b);\
	assert(b != a);\
}

#define assert_null(data) {\
	assert(data.type() == DataType::Null);\
	assert(data.isNull());\
	assert(data.length() == 0);\
	assert_cast_values(data, 0, 0.0, false, "null");\
	assert_not_int(data);\
	assert_not_float(data);\
	assert_not_bool(data);\
	assert_not_string(data);\
	assert_not_array(data);\
	assert_not_table(data);\
	assert_equals(data, Data());\
	assert_equals(data, Data(DataType::Null));\
}

#define assert_boolean(data, value) {\
	assert_primitive(data, DataType::Boolean, Boolean, value);\
	assert_cast_values(data, (int)value, (double)value, value, (value ? "true" : "false"));\
	assert_not_null(data);\
	assert_not_int(data);\
	assert_not_float(data);\
	assert_not_string(data);\
	assert_not_array(data);\
	assert_not_table(data);\
	assert_no_throw(Data(value).toBool());\
	assert_equals(data, Data(value));\
}


#define assert_integer(data, value) {\
	assert_primitive(data, DataType::Integer, Integer, value);\
	assert_cast_values(data, value, (double)value, (bool)value, std::to_string(value));\
	assert(data.isNumber());\
	assert_not_null(data);\
	assert_not_bool(data);\
	assert_not_float(data);\
	assert_not_string(data);\
	assert_not_array(data);\
	assert_not_table(data);\
	assert_equals(data, Data(value));\
}

#define assert_decimal(data, value) {\
	assert_primitive(data, DataType::Float, Float, value);\
	assert_cast_values(data, (int)value, (double)value, (bool)value, std::to_string(value));\
	assert(data.isNumber());\
	assert_not_null(data);\
	assert_not_int(data);\
	assert_not_bool(data);\
	assert_not_string(data);\
	assert_not_array(data);\
	assert_not_table(data);\
	assert_equals(data, Data(value));\
}

#define assert_string(data, int_value, float_value, bool_value, string_value) {\
	assert(data.type() == DataType::String);\
	assert(std::string(data.typeName()) == "string");\
	assert(data.isString());\
	assert(data.length() == data.asString().length());\
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
	assert(data.type() == DataType::Array);\
	assert(data.isArray());\
	assert_not_null(data);\
	assert_not_int(data);\
	assert_not_float(data);\
	assert_not_bool(data);\
	assert_not_string(data);\
	assert_not_table(data);\
	assert_equals(data, Data(value));\
}

#define assert_table(data, value) {\
	assert(data.type() == DataType::Table);\
	assert(data.isTable());\
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
	assert_null(Data(DataType::Null));

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
	assert_not_equals(move, Data(DataType::Array));
	assert_not_equals(move, Data(DataType::Table));
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

	assert_decimal(Data(DataType::Float), 0.0);
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
	assert_not_equals(move, Data(DataType::Array));
	assert_not_equals(move, Data(DataType::Table));
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

	assert_boolean(Data(DataType::Boolean), false);
	assert_boolean(Data(true), true);
	assert_boolean(Data(false), false);

	assert_not_equals(move, Data());
	assert_not_equals(move, Data(130));
	assert_not_equals(move, Data(-130.52));
	assert_not_equals(move, Data("hello"));
	assert_not_equals(move, Data(false));
	assert_not_equals(move, Data(DataType::Array));
	assert_not_equals(move, Data(DataType::Table));
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

	assert_string(Data(DataType::String), 0, 0.0, false, "");
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
	assert_not_equals(move, Data(DataType::Array));
	assert_not_equals(move, Data(DataType::Table));
}

void test_array()
{
	using Array = Data::Array;

	{
		Data empty = Array {};
		assert_array(empty, Array());
		assert(empty.length() == 0);
		assert(empty.isEmpty());

		Data copy(empty);
		assert_array(copy, Array());
		assert(copy.length() == 0);
		assert(copy.isEmpty());
		assert_equals(copy, empty);
		copy = empty;
		assert_array(copy, Array());
		assert(copy.length() == 0);
		assert(copy.isEmpty());
		assert_equals(copy, empty);

		Data move(std::move(empty));
		assert_array(move, Array());
		assert(move.length() == 0);
		assert(move.isEmpty());
		assert_equals(move, copy);
		Data clone = copy;
		move = std::move(copy);
		assert_array(move, Array());
		assert(move.length() == 0);
		assert(move.isEmpty());
		assert_equals(move, clone);

		assert_not_equals(move, Data());
		assert_not_equals(move, Data(130));
		assert_not_equals(move, Data(-130));
		assert_not_equals(move, Data(-130.52));
		assert_not_equals(move, Data("hello"));
		assert_not_equals(move, Data(false));
		assert_equals(move, Data(DataType::Array));
		assert_not_equals(move, Data(Data::Array { 1, 2, 3}));
		assert_not_equals(move, Data(DataType::Table));
	}

	{
		Data init = Array { "hello", 1, true, 0.4 };
		assert_array(init, Array({ "hello", 1, true, 0.4 }));
		assert(init.length() == 4);
		assert(!init.isEmpty());
		assert_string(init[0], 0, 0.0, true, "hello");
		assert_integer(init[1], 1);
		assert_boolean(init[2], true);
		assert_decimal(init[3], 0.4);

		Data copy(init);
		assert_array(copy, Array({ "hello", 1, true, 0.4 }));
		assert(copy.length() == 4);
		assert(!copy.isEmpty());
		assert_string(copy[0], 0, 0.0, true, "hello");
		assert_integer(copy[1], 1);
		assert_boolean(copy[2], true);
		assert_decimal(copy[3], 0.4);
		assert_equals(copy, init);
		copy = init;
		assert_array(copy, Array({ "hello", 1, true, 0.4 }));
		assert(copy.length() == 4);
		assert(!copy.isEmpty());
		assert_string(copy[0], 0, 0.0, true, "hello");
		assert_integer(copy[1], 1);
		assert_boolean(copy[2], true);
		assert_decimal(copy[3], 0.4);
		assert_equals(copy, init);

		Data move(std::move(init));
		assert_array(move, Array({ "hello", 1, true, 0.4 }));
		assert(move.length() == 4);
		assert(!move.isEmpty());
		assert_string(move[0], 0, 0.0, true, "hello");
		assert_integer(move[1], 1);
		assert_boolean(move[2], true);
		assert_decimal(move[3], 0.4);
		assert_equals(move, copy);
		Data clone = copy;
		move = std::move(copy);
		assert_array(move, Array({ "hello", 1, true, 0.4 }));
		assert(move.length() == 4);
		assert(!move.isEmpty());
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
		assert_not_equals(move, Data(DataType::Array));
		assert_not_equals(move, Data(DataType::Table));
	}

	Data array = Array { 1, 2.0f, "3", false, Data() };
	assert_array(array, Array({ 1, 2.0f, "3", false, Data() }));
	assert(array.length() == 5);
	assert(!array.isEmpty());
	assert_integer(array[0], 1);
	assert_decimal(array[1], 2.0f);
	assert_string(array[2], 3, 3.0, true, "3");
	assert_boolean(array[3], false);
	assert_null(array[4]);

	std::vector<std::string> array_of_strings = { "this", "is", "a", "sentence" };

	array = Data::from(array_of_strings);

	for (size_t i = 0; i < array_of_strings.size(); ++i)
		assert(array[i] == array_of_strings[i] && "got unexpected element value");

	std::vector<int> array_of_ints = { 6, 6345, 26, 34, -1231 };

	array = Data::from(array_of_ints);

	for (size_t i = 0; i < array_of_ints.size(); ++i)
		assert(array[i] == array_of_ints[i] && "got unexpected element value");
}

void test_table()
{
	using Table = Data::Table;

	{
		Data empty = Table {};
		assert_table(empty, Table());
		assert(empty.length() == 0);
		assert(empty.isEmpty());

		Data copy(empty);
		assert_table(copy, Table());
		assert(copy.length() == 0);
		assert(copy.isEmpty());
		assert_equals(copy, empty);
		copy = empty;
		assert_table(copy, Table());
		assert(copy.length() == 0);
		assert(copy.isEmpty());
		assert_equals(copy, empty);

		Data move(std::move(empty));
		assert_table(move, Table());
		assert(move.length() == 0);
		assert(move.isEmpty());
		assert_equals(move, copy);
		Data clone = copy;
		move = std::move(copy);
		assert_table(move, Table());
		assert(move.length() == 0);
		assert(move.isEmpty());
		assert_equals(move, clone);

		assert_not_equals(move, Data());
		assert_not_equals(move, Data(130));
		assert_not_equals(move, Data(-130));
		assert_not_equals(move, Data(-130.52));
		assert_not_equals(move, Data("hello"));
		assert_not_equals(move, Data(false));
		assert_not_equals(move, Data(DataType::Array));
		assert_equals(move, Data(DataType::Table));
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
		assert(init.length() == 5);
		assert(!init.isEmpty());
		assert_boolean(init["boolean"], true);
		assert_integer(init["integer"], 1362);
		assert_decimal(init["decimal"], 235.125);
		assert_string(init["string"], 0, 0.0, true, "this is text");
		assert_null(init["null"]);

		Data copy(init);
		assert_table(copy, table);
		assert(copy.length() == 5);
		assert(!(copy.isEmpty()));
		assert_equals(copy, init);
		copy = init;
		assert_table(copy, table);
		assert(copy.length() == 5);
		assert(!(copy.isEmpty()));
		assert_equals(copy, init);

		Data move(std::move(init));
		assert_table(move, table);
		assert(move.length() == 5);
		assert(!(move.isEmpty()));
		assert_equals(move, copy);
		Data clone = copy;
		move = std::move(copy);
		assert_table(move, table);
		assert(move.length() == 5);
		assert(!(move.isEmpty()));
		assert_equals(move, clone);

		assert_not_equals(move, Data());
		assert_not_equals(move, Data(130));
		assert_not_equals(move, Data(-130));
		assert_not_equals(move, Data(-130.52));
		assert_not_equals(move, Data("hello"));
		assert_not_equals(move, Data(false));
		assert_not_equals(move, Data(DataType::Array));
		assert_not_equals(move, Data(DataType::Table));
	}

	std::unordered_map<std::string, std::string> table_of_strings = {
		{ "abc", "def" },
		{ "hij", "klm" },
		{ "nop", "qrs" },
		{ "tuv", "wxyz" }
	};

	Data table = Data::from(table_of_strings);

	for (const auto& pair : table_of_strings)
		assert(table[pair.first] == pair.second && "got unexpected table value");

	std::unordered_map<std::string, double> table_of_doubles = {
		{ "123", 123.0 },
		{ "194.65", 194.65 },
		{ "987.125", 987.125 }
	};

	table = Data::from(table_of_doubles);
	
	for (const auto& pair : table_of_doubles)
		assert(table[pair.first] == pair.second && "did not get expected table value");
}

#define test(func) test_##func()

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