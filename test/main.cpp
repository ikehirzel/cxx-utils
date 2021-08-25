#include <hirzel/data/data.h>
#include <hirzel/data/json.h>
#include <hirzel/data/validation.h>
#include <cassert>
#include <iostream>
#include <fstream>

#include "assert.h"

using namespace hirzel::data;

#define assert_throws(expr, exception_type)\
{\
	try\
	{\
		expr;\
		assertion_failure("'" #expr "' expected " #exception_type " but no exception was thrown.");\
	}\
	catch(const exception_type& e) { }\
	catch(const std::exception& e)\
	{\
		assertion_failure("'" #expr "' expected " #exception_type " but caught unhandled exception");\
	}\
}

#define assert_not_throws(expr, exception_type)\
{\
	try\
	{\
		expr;\
	}\
	catch (const std::exception&)\
	{\
		print_assert_message(CURRENT_SOURCE_LOCATION, #expr,\
			"expression threw an exception");\
		throw;\
	}\
}



#define assert_cast_values(data, int_val, float_val, bool_val, string_val)\
	assert(data.as_int() == int_val);\
	assert(data.as_double() == float_val);\
	assert(data.as_bool() == bool_val);\
	assert(data.as_string() == string_val);\
	assert_throws(data.get_array(), Data::TypeException);\
	assert_throws(data.get_table(), Data::TypeException)

#define assert_primitive(data, typename, funcname, value)\
	assert(data.type() == typename);\
	assert(data.is_##funcname());\
	assert(data.size() == 1)

#define assert_not_primitive(data, typename, funcname)\
	assert(data.type() != typename);\
	assert(!data.is_##funcname())

#define assert_not_null(data)		assert(data.type() != Data::NULL_TYPE); assert(!data.is_null());
#define assert_not_bool(data)		assert_not_primitive(data, Data::BOOLEAN_TYPE, boolean)
#define assert_not_int(data)		assert_not_primitive(data, Data::INTEGER_TYPE, integer)
#define assert_not_float(data)		assert_not_primitive(data, Data::DECIMAL_TYPE, decimal)
#define assert_not_string(data)		assert_not_primitive(data, Data::STRING_TYPE, string)
#define assert_not_array(data)		assert(data.type() != Data::ARRAY_TYPE); assert(!data.is_array())
#define assert_not_table(data)		assert(data.type() != Data::TABLE_TYPE); assert(!data.is_table())

#define assert_null(data)\
	assert(data.type() == Data::NULL_TYPE); assert(data.is_null()); assert(data.size() == 0);\
	assert_cast_values(data, 0, 0.0, false, "null");\
	assert_not_int(data);\
	assert_not_float(data);\
	assert_not_bool(data);\
	assert_not_string(data);\
	assert_not_array(data);\
	assert_not_table(data)

#define assert_bool(data, value)\
	assert_primitive(data, Data::BOOLEAN_TYPE, boolean, value);\
	assert_cast_values(data, (int)value, (double)value, value, (value ? "true" : "false"));\
	assert_not_null(data);\
	assert_not_int(data);\
	assert_not_float(data);\
	assert_not_string(data);\
	assert_not_array(data);\
	assert_not_table(data)

#define assert_int(data, value)\
	assert_primitive(data, Data::INTEGER_TYPE, integer, value);\
	assert_cast_values(data, value, (double)value, (bool)value, std::to_string(value));\
	assert(data.is_number());\
	assert_not_null(data);\
	assert_not_bool(data);\
	assert_not_float(data);\
	assert_not_string(data);\
	assert_not_array(data);\
	assert_not_table(data)

#define assert_float(data, value)\
	assert_primitive(data, Data::DECIMAL_TYPE, decimal, value);\
	assert_cast_values(data, (int)value, (double)value, (bool)value, std::to_string(value));\
	assert(data.is_number());\
	assert_not_null(data);\
	assert_not_int(data);\
	assert_not_bool(data);\
	assert_not_string(data);\
	assert_not_array(data);\
	assert_not_table(data)

#define assert_string(data, int_value, float_value, bool_value, string_value)\
	assert(data.type() == Data::STRING_TYPE); assert(data.is_string()); assert(data.size() == data.as_string().size());\
	assert_cast_values(data, int_value, float_value, bool_value, string_value);\
	assert_not_null(data);\
	assert_not_int(data);\
	assert_not_float(data);\
	assert_not_bool(data);\
	assert_not_array(data);\
	assert_not_table(data)

#define assert_array(data)\
	assert(data.type() == Data::ARRAY_TYPE); assert(data.is_array());\
	assert_not_null(data);\
	assert_not_int(data);\
	assert_not_float(data);\
	assert_not_bool(data);\
	assert_not_string(data);\
	assert_not_table(data)

#define assert_table(data)\
	assert(data.type() == Data::TABLE_TYPE); assert(data.is_table());\
	assert_not_null(data);\
	assert_not_bool(data);\
	assert_not_int(data);\
	assert_not_float(data);\
	assert_not_string(data);\
	assert_not_array(data)

void test_null()
{
	Data data;
	assert_null(data);

	data = Data();
	assert_null(data);
}

void test_int()
{
	Data assign;
	assert_null(assign);
	assign = 0;
	assert_int(assign, 0);
	assign = 4L;
	assert_int(assign, 4);
	assign = -1023LL;
	assert_int(assign, -1023);

	Data init0 = 62;
	assert_int(init0, 62);
	Data init1 = 0;
	assert_int(init1, 0);
	Data init2 = -54;
	assert_int(init2, -54);
}

void test_float()
{
	Data assign;
	assert_null(assign);
	assign = 1.34;
	assert_float(assign, 1.34);
	assign = 0.0f;
	assert_float(assign, 0.0f);
	assign = -123592.235;
	assert_float(assign, -123592.235);

	Data init0 = 2.0f;
	assert_float(init0, 2.0f);
	Data init1 = 0.0;
	assert_float(init1, 0.0);
	Data init2 = -124.0f;
	assert_float(init2, -124.0f);
}

void test_bool()
{
	Data assign;
	assign = true;
	assert_bool(assign, true);
	assign = false;
	assert_bool(assign, false);
	bool truth = false;
	assign = truth;
	assert_bool(assign, false);
	truth = true;
	assign = truth;
	assert_bool(assign, true);

	Data init0 = true;
	assert_bool(init0, true);
	Data init1 = false;
	assert_bool(init1, false);
}

void test_string()
{
	Data assign;
	assert_null(assign);
	assign = "";
	assert_string(assign, 0, 0.0, false, "");
	assign = "hello";
	assert_string(assign, 0, 0.0, true, "hello");
	assign = "1.2";
	assert_string(assign, 1, 1.2, true, "1.2");
	assign = "123";
	assert_string(assign, 123, 123.0, true, "123");
	assign = "0";
	assert_string(assign, 0, 0.0, true, "0");
	assign = "0000";
	assert_string(assign, 0, 0.0, true, "0000");

	Data init0 = "hello";
	assert_string(init0, 0, 0.0, true, "hello");
}

void test_array()
{
	Data assign;
	assert_null(assign);
	assign = Data::Array({ 1, false, -23.4, "hello" });;
	assert_array(assign);
	assert(assign.size() == 4);
	assert_int(assign[0], 1);
	assert_bool(assign[1], false);
	assert_float(assign[2], -23.4);
	assert_string(assign[3], 0, 0.0, true, "hello");
}

void test_table()
{
	Data assign;
	assert_null(assign);
	assign = Data::Table();
	assert_table(assign);
	assert(assign.size() == 0);
	assign["key"] = false;
	assert_bool(assign["key"], false);
	assign["arr"] = Data::Array();
	assert_array(assign["arr"]);
	assert(assign["arr"].size() == 0);

	Data init0 = Data::Table({
		{"key", "value"},
		{"number", 17.5}
	});
	assert_table(init0);
	assert_string(init0["key"], 0, 0.0, true, "value");
	assert_float(init0["number"], 17.5);
	assert(init0.size() == 2);

}

void test_json()
{
	
}

#define TEST(name) std::cout << "Testing " #name "...\n"; test_##name(); std::cout << "\t\tAll tests passed\n";

int main()
{
	std::cout
		<< "\n"
		<< "##################################################################\n"
		<< "Testing Data\n"
		<< "##################################################################\n"
		<< "\n";

	TEST(null);
	TEST(int);
	TEST(float);
	TEST(bool);
	TEST(string);
	TEST(array);
	TEST(table);
	TEST(json);

	return 0;
}
