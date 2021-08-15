#include <hirzel/data.h>
#include <cassert>
#include <iostream>
#include <fstream>

using namespace hirzel;

struct SourceLocation
{
  const char *filename;
  unsigned line;
  const char *function_name;
};

#define CURRENT_SOURCE_LOCATION SourceLocation{__FILE__, __LINE__, __func__}

void print_assert_message(SourceLocation location, const char *expression, const char *msg)
{
	std::cerr << location.filename << "@" << location.line << "::"
		<< location.function_name << ": assertion '" << expression
		<< "' failed!";

	if (msg)
		std::cerr << " : " << msg;
		
	std::cerr << std::endl;
}

#define assert_throws(expr, exception_type)\
{\
	try\
	{\
		expr;\
		print_assert_message(CURRENT_SOURCE_LOCATION, #expr,\
			"expression did not throw an exception");\
		std::abort();\
	}\
	catch(const exception_type& e) { }\
	catch(const std::exception& e)\
	{\
		print_assert_message(CURRENT_SOURCE_LOCATION, #expr,\
			"incorrect type of exception");\
		throw e;\
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

std::string colors_json =
R"=====({
  "colors": [
    {
      "color": "black",
      "category": "hue",
      "type": "primary",
      "code": {
        "rgba": [255,255,255,1],
        "hex": "#000"
      }
    },
    {
      "color": "white",
      "category": "value",
      "code": {
        "rgba": [0,0,0,1],
        "hex": "#FFF"
      }
    },
    {
      "color": "red",
      "category": "hue",
      "type": "primary",
      "code": {
        "rgba": [255,0,0,1],
        "hex": "#FF0"
      }
    },
    {
      "color": "blue",
      "category": "hue",
      "type": "primary",
      "code": {
        "rgba": [0,0,255,1],
        "hex": "#00F"
      }
    },
    {
      "color": "yellow",
      "category": "hue",
      "type": "primary",
      "code": {
        "rgba": [255,255,0,1],
        "hex": "#FF0"
      }
    },
    {
      "color": "green",
      "category": "hue",
      "type": "secondary",
      "code": {
        "rgba": [0,255,0,1],
        "hex": "#0F0"
      }
    }
  ]
})=====";

std::string pokemon_json= R"====(
	{
  "count": 1118,
  "next": "https://pokeapi.co/api/v2/pokemon?offset=20&limit=20",
  "previous": null,
  "results": [
    {
      "name": "bulbasaur",
      "url": "https://pokeapi.co/api/v2/pokemon/1/"
    },
    {
      "name": "ivysaur",
      "url": "https://pokeapi.co/api/v2/pokemon/2/"
    },
    {
      "name": "venusaur",
      "url": "https://pokeapi.co/api/v2/pokemon/3/"
    },
    {
      "name": "charmander",
      "url": "https://pokeapi.co/api/v2/pokemon/4/"
    },
    {
      "name": "charmeleon",
      "url": "https://pokeapi.co/api/v2/pokemon/5/"
    },
    {
      "name": "charizard",
      "url": "https://pokeapi.co/api/v2/pokemon/6/"
    },
    {
      "name": "squirtle",
      "url": "https://pokeapi.co/api/v2/pokemon/7/"
    },
    {
      "name": "wartortle",
      "url": "https://pokeapi.co/api/v2/pokemon/8/"
    },
    {
      "name": "blastoise",
      "url": "https://pokeapi.co/api/v2/pokemon/9/"
    },
    {
      "name": "caterpie",
      "url": "https://pokeapi.co/api/v2/pokemon/10/"
    },
    {
      "name": "metapod",
      "url": "https://pokeapi.co/api/v2/pokemon/11/"
    },
    {
      "name": "butterfree",
      "url": "https://pokeapi.co/api/v2/pokemon/12/"
    },
    {
      "name": "weedle",
      "url": "https://pokeapi.co/api/v2/pokemon/13/"
    },
    {
      "name": "kakuna",
      "url": "https://pokeapi.co/api/v2/pokemon/14/"
    },
    {
      "name": "beedrill",
      "url": "https://pokeapi.co/api/v2/pokemon/15/"
    },
    {
      "name": "pidgey",
      "url": "https://pokeapi.co/api/v2/pokemon/16/"
    },
    {
      "name": "pidgeotto",
      "url": "https://pokeapi.co/api/v2/pokemon/17/"
    },
    {
      "name": "pidgeot",
      "url": "https://pokeapi.co/api/v2/pokemon/18/"
    },
    {
      "name": "rattata",
      "url": "https://pokeapi.co/api/v2/pokemon/19/"
    },
    {
      "name": "raticate",
      "url": "https://pokeapi.co/api/v2/pokemon/20/"
    }
  ]
}
)====";

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

void test_parse_json()
{
	Data v;
	assert_not_throws(v = Data::parse_json("true"), Data::ParseException);
	assert_bool(v, true);
	assert_not_throws(v = Data::parse_json("false"), Data::ParseException);
	assert_bool(v, false);
	assert_not_throws(v = Data::parse_json("null"), Data::ParseException);
	assert_null(v);
	assert_not_throws(v = Data::parse_json("3"), Data::ParseException);
	assert_int(v, 3);
	assert_not_throws(v = Data::parse_json("-5"), Data::ParseException);
	assert_int(v, -5);
	assert_not_throws(v = Data::parse_json("-876.02"), Data::ParseException);
	assert_float(v, -876.02);
	assert_not_throws(v = Data::parse_json("5.342"), Data::ParseException);
	assert_float(v, 5.342);
	assert_throws(Data::parse_json("5.4."), Data::ParseException);

	assert_not_throws(v = Data::parse_json("\"hello\""), Data::ParseException);
	assert_string(v, 0, 0.0, true, "hello");

	assert_not_throws(v = Data::parse_json(R"([3,-5,"hello"])"), Data::ParseException);
	assert_int(v[0], 3);
	assert_int(v[1], -5);
	assert_string(v[2], 0, 0.0, true, "hello");

	std::string ex_json = R"({"num":3,"str":"abcdef","":-4})";
	Data second;
	assert_not_throws(v = Data::parse_json(ex_json), Data::ParseException);
	assert(v != second);
	assert(second != v);
	assert_not_throws(second = Data::parse_json(ex_json), Data::ParseException);
	assert(v == second);
	assert(second == v);
	assert_not_throws(second = Data::parse_json(v.as_json()), Data::ParseException);
	assert(second == v);
	assert(v == second);
	assert_not_throws(v = Data::parse_json(second.as_json()), Data::ParseException);
	assert(second == v);
	assert(v == second);
	assert_table(v);
	assert_int(v["num"], 3);
	assert_string(v["str"], 0, 0.0, true, "abcdef");
	assert_int(v[""], -4);

	ex_json = R"({"scooby":{"snacks":{"flavor":"spicy","size":3}},"num":3,"arr":[3,-5,2]})";
	assert_not_throws(v = Data::parse_json(ex_json), Data::ParseException);
	assert(v != second);
	assert(second != v);
	assert_not_throws(second = Data::parse_json(ex_json), Data::ParseException);
	assert(v == second);
	assert(second == v);
	assert_not_throws(second = Data::parse_json(v.as_json()), Data::ParseException);
	assert(second == v);
	assert(v == second);
	assert_not_throws(v = Data::parse_json(second.as_json()), Data::ParseException);
	assert(second == v);
	assert(v == second);
	assert(!v["scooby"].is_null());
	assert(v["scooby"].is_table());

	assert_not_throws(v = Data::parse_json("[]"), Data::ParseException);
	assert_array(v);
	assert(v.size() == 0);
	assert(!v.as_bool());

	assert_not_throws(v = Data::parse_json("{}"), Data::ParseException);
	assert_table(v);
	assert(v.size() == 0);

	std::string serial = v.as_json();
	assert_not_throws(v = Data::parse_json(pokemon_json), Data::ParseException);
	assert(v.contains("previous"));
	assert_null(v["previous"]);
	assert(v.contains("results"));
	assert_array(v["results"]);
	assert_table(v["results"][0]);
	assert(v["results"][0].contains("name"));
	assert_string(v["results"][0]["name"], 0, 0.0, true, "bulbasaur");
	assert(v["results"][0].contains("url"));
	assert_string(v["results"][0]["url"], 0, 0.0, true, "https://pokeapi.co/api/v2/pokemon/1/");

	assert_not_throws(v = Data::parse_json(colors_json), Data::ParseException);
	assert(v.contains("colors"));
	assert_array(v["colors"]);
	assert(v["colors"].size() == 6);
	assert_table(v["colors"][0]);
	assert(v["colors"][0].contains("color"));
	assert_string(v["colors"][0]["color"], 0, 0.0, true, "black");
}

#define TEST(name) std::cout << "Testing " #name "...\n"; test_##name(); std::cout << "\t\tAll tests passed\n";

int main()
{
	TEST(null);
	TEST(int);
	TEST(float);
	TEST(bool);
	TEST(string);
	TEST(array);
	TEST(table);

	TEST(parse_json);
	return 0;
}
