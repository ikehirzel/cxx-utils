#define HIRZEL_VAR_IMPLEMENTATION
#include <hirzel/var.h>
#include <cassert>
#include <iostream>

using hirzel::var;

void test_null(){}
void test_int() {}
void test_uint() {}
void test_float() {}
void test_char() {}
void test_str() {}
void test_bool() {}

void test_array()
{
	var j = { 1, "HELLO", true };
	assert(j.size() == 3);
	assert(j.type() == hirzel::ARRAY_TYPE);

	var& v0 = j[0];
	assert(v0.type() == hirzel::INT_TYPE);
	assert(v0.to_int() == 1);
	assert(v0.size() == sizeof(intmax_t));

	var& v1 = j[1];
	assert(v1.type() == hirzel::STR_TYPE);
	assert(v1.to_string() == "HELLO");
	assert(v1.size() == 5);

	var& v2 = j[2];
	assert(v2.type() == hirzel::BOOL_TYPE);
	assert(v2.to_bool() == true);
	assert(v2.to_string() == "true");
	assert(v2.size() == sizeof(bool));
}

void test_map() {}

int main()
{
	test_array();
	return 0;
}