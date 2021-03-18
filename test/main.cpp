#define HIRZEL_VAR_IMPLEMENTATION
#include <hirzel/var.h>
#include <cassert>
#include <iostream>
#include <fstream>

using hirzel::var;

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

#define compare(v, t, n, i, u, d, c, b, s, e)\
	assert(v.type() == t);\
	assert(v.size() == n);\
	assert(v.to_int() == i);\
	assert(v.to_uint() == u);\
	assert(v.to_double() == d);\
	assert(v.to_char() == c);\
	assert(v.to_bool() == b);\
	assert(v.to_string() == s);\
	assert(v.empty() == e);

#define check_prim(v, val, t, func)\
	assert(v.type() == t);\
	assert(v.is_##func ());\
	assert(v.to_##func() == val);

#define check_null(v)			assert(v.type() == var::NULL_TYPE); assert(v.is_null());
#define check_error(v)			assert(v.type() == var::ERROR_TYPE); assert(v.is_error());
#define check_char(v, val)		check_prim(v, val, var::CHAR_TYPE, char)
#define check_bool(v, val)		check_prim(v, val, var::BOOL_TYPE, bool)
#define check_int(v, val)		check_prim(v, val, var::INT_TYPE, int)
#define check_uint(v, val)		check_prim(v, val, var::UINT_TYPE, uint)
#define check_float(v, val)		assert(v.type() == var::FLOAT_TYPE); assert(v.is_float()); assert(v.to_double() == val);
#define check_string(v, val)	check_prim(v, val, var::STRING_TYPE, string);
#define check_table(v, val)		assert(v.type() == var::TABLE_TYPE); assert(v.is_table()); assert(v.empty() == val);
#define check_array(v, val)		assert(v.type() == var::ARRAY_TYPE); assert(v.is_array()); assert(v.empty() == val);

void test_null()
{
	var v;
	assert(v.is_null());
	compare(v, var::NULL_TYPE, 0, 0, 0u, 0.0, (char)0, false, "null", true);
}

void test_int()
{
	var v;
	assert(!v.is_int());
	v = 62;
	assert(v.is_int() && v.is_num());
	compare(v, var::INT_TYPE, sizeof(intmax_t), 62, 62u, 62.0, (char)62, true, "62", false);
	v = -1023;
	compare(v, var::INT_TYPE, sizeof(intmax_t), -1023, (uintmax_t)(-1023), -1023.0, (char)-1023, true, "-1023", false);
}

void test_uint()
{
	var v;
	assert(!v.is_uint());
	v = 45u;
	assert(v.is_uint() && v.is_num());
	compare(v, var::UINT_TYPE, sizeof(uintmax_t), 45, 45u, 45.0, (char)45, true, "45", false);
}

void test_float()
{
	var v;
	assert(!v.is_float());
	v = 12.4;
	assert(v.is_float() && v.is_num());
	compare(v, var::FLOAT_TYPE, sizeof(double), 12, 12u, 12.4, (char)12, true, std::to_string(12.4), false);
}

void test_char()
{
	var v;
	assert(!v.is_char());
	v = ' ';
	assert(v.is_char());
	compare(v, var::CHAR_TYPE, sizeof(char), 32, 32u, 32.0, ' ', true, " ", false);
}

void test_bool()
{
	var v;
	assert(!v.is_bool());
	v = true;
	assert(v.is_bool());
	compare(v, var::BOOL_TYPE, sizeof(bool), 1, 1u, 1.0, 1, true, "true", false);
	v = false;
	compare(v, var::BOOL_TYPE, sizeof(bool), 0, 0u, 0.0, 0, false, "false", false);
}

void test_string()
{
	var v;
	assert(!v.is_string());
	v = "";
	assert(v.is_string());
	compare(v, var::STRING_TYPE, 0, 0, 0u, 0, (char)0, false, "", true);
	v = "HELLO";
	compare(v, var::STRING_TYPE, 5, 0, 0, 0.0, 'H', true, "HELLO", false);
}

void test_array()
{
	var v;
	assert(!v.is_array());
	v = { 1, "HELLO", true };
	assert(v.type() == var::ARRAY_TYPE);
	assert(v.is_array());
	assert(v.size() == 3);
	assert(!v.empty());
	check_int(v[0], 1);
	check_string(v[1], "HELLO");
	check_bool(v[2], true);
}

void test_table()
{
	var v;
	assert(!v.is_table());
	v["val"] = 2;
	assert(v.is_table());
	assert(v.contains("val"));
	assert(v.size() == 1);
	check_int(v["val"], 2);
	v[0] = "Hello";
	assert(v.contains("0"));
	assert(v.size() == 2);
	check_string(v[0], "Hello");
}

void test_parse_json()
{
	var v;
	v = var::parse_json("true");
	check_bool(v, true);
	v = var::parse_json("false");
	check_bool(v, false);
	v = var::parse_json("null");
	check_null(v);
	v = var::parse_json("3");
	check_uint(v, 3);
	v = var::parse_json("-5");
	check_int(v, -5);
	v = var::parse_json("-876.02");
	check_float(v, -876.02);
	v = var::parse_json("5.342");
	check_float(v, 5.342);
	v = var::parse_json("5.4.");
	check_error(v);
	v = var::parse_json("\"hello\"");
	check_string(v, "hello");

	v = var::parse_json("[3,-5,\"hello\"]");
	check_uint(v[0], 3);
	check_int(v[1], -5);
	check_string(v[2], "hello");

	std::string ex_json = "{\"num\":3,\"str\":\"abcdef\",\"\":-4}";
	v = var::parse_json(ex_json);
	assert(v.to_json() == ex_json);
	check_table(v, false);
	check_uint(v["num"], 3);
	check_string(v["str"], "abcdef");
	check_int(v[""], -4);

	ex_json = "{\"scooby\":{\"snacks\":{\"flavor\":\"spicy\",\"size\":3}},\"num\":3,\"arr\":[3,-5,2]}";
	v = var::parse_json(ex_json);
	assert(v.to_json() == ex_json);
	assert(!v.is_error());
	assert(!v["scooby"].is_null());
	assert(v["scooby"].is_table());

	v = var::parse_json("[]");
	check_array(v, true);
	v = var::parse_json("{}");
	check_table(v, true);

	std::string serial = v.to_json();
	assert(!v.is_error());
	v = var::parse_json(pokemon_json);
	assert(v.contains("previous"));
	check_null(v["previous"]);
	assert(v.contains("results"));
	check_array(v["results"], false);
	check_table(v["results"][0], false);
	assert(v["results"][0].contains("name"));
	check_string(v["results"][0]["name"], "bulbasaur");
	assert(v["results"][0].contains("url"));
	check_string(v["results"][0]["url"], "https://pokeapi.co/api/v2/pokemon/1/");

	v = var::parse_json(colors_json);
	assert(v.contains("colors"));
	check_array(v["colors"], false);
	assert(v["colors"].size() == 6);
	check_table(v["colors"][0], false);
	assert(v["colors"][0].contains("color"));
	check_string(v["colors"][0]["color"], "black");
}

#define TEST(name) std::cout << "Testing " #name "...\n"; test_##name(); std::cout << "\t\tAll tests passed\n";

int main()
{
	TEST(null);
	TEST(int);
	TEST(uint);
	TEST(float);
	TEST(char);
	TEST(bool);
	TEST(string);
	TEST(array);
	TEST(table);
	TEST(parse_json);
	return 0;
}