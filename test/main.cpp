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

#define compare(v, t, n, i, u, d, c, b, s)\
	assert(v.type() == t);\
	assert(v.size() == n);\
	assert(v.to_int() == i);\
	assert(v.to_uint() == u);\
	assert(v.to_double() == d);\
	assert(v.to_char() == c);\
	assert(v.to_bool() == b);\
	assert(v.to_string() == s);

void test_null()
{
	var v;
	compare(v, var::NULL_TYPE, 0, 0, 0u, 0.0, (char)0, false, "null");
}

void test_int()
{
	var v;
	assert(v.type() != var::INT_TYPE);
	v = 62;
	compare(v, var::INT_TYPE, sizeof(intmax_t), 62, 62u, 62.0, (char)62, true, "62");
	v = -1023;
	compare(v, var::INT_TYPE, sizeof(intmax_t), -1023, (uintmax_t)(-1023), -1023.0, (char)-1023, true, "-1023");
}

void test_uint()
{
	var v;
	assert(v.type() != var::UINT_TYPE);
	v = 45u;
	compare(v, var::UINT_TYPE, sizeof(uintmax_t), 45, 45u, 45.0, (char)45, true, "45");
}

void test_float()
{
	var v;
	assert(v.type() != var::FLOAT_TYPE);
	v = 12.4;
	compare(v, var::FLOAT_TYPE, sizeof(double), 12, 12u, 12.4, (char)12, true, std::to_string(12.4));
}

void test_char()
{
	var v;
	assert(v.type() != var::CHAR_TYPE);
	v = ' ';
	compare(v, var::CHAR_TYPE, sizeof(char), 32, 32u, 32.0, ' ', true, " ");
}

void test_bool()
{
	var v;
	assert(v.type() != var::BOOL_TYPE);
	v = true;
	compare(v, var::BOOL_TYPE, sizeof(bool), 1, 1u, 1.0, 1, true, "true");
	v = false;
	compare(v, var::BOOL_TYPE, sizeof(bool), 0, 0u, 0.0, 0, false, "false");
}

void test_string()
{
	var v;
	assert(v.type() != var::STRING_TYPE);
	v = "";
	compare(v, var::STRING_TYPE, 0, 0, 0u, 0, (char)0, false, "");
	v = "HELLO";
	compare(v, var::STRING_TYPE, 5, 0, 0, 0.0, 'H', true, "HELLO");
}

void test_array()
{
	var j = { 1, "HELLO", true };
	assert(j.size() == 3);
	assert(j.type() == var::ARRAY_TYPE);

	var& v0 = j[0];
	assert(v0.type() == var::INT_TYPE);
	assert(v0.to_int() == 1);
	assert(v0.size() == sizeof(intmax_t));

	var& v1 = j[1];
	assert(v1.type() == var::STRING_TYPE);
	assert(v1.to_string() == "HELLO");
	assert(v1.size() == 5);

	var& v2 = j[2];
	assert(v2.type() == var::BOOL_TYPE);
	assert(v2.to_bool() == true);
	assert(v2.to_string() == "true");
	assert(v2.size() == sizeof(bool));
}

void test_table()
{
	var m;
	m["val"] = 2;
	assert(m["val"].to_int() == 2);
	assert(m.size() == 1);
	m[0] = "Hello";
	assert(m[0].to_string() == "Hello");
}

void test_parse_json()
{
	var v;
	// v = var::parse_json("true");
	// assert(v.type() == var::BOOL_TYPE);
	// assert(v.to_bool() == true);

	// v = var::parse_json("false");
	// assert(v.type() == var::BOOL_TYPE);
	// assert(v.to_bool() == false);

	// v = var::parse_json("null");
	// assert(v.type() == var::NULL_TYPE);

	// v = var::parse_json("3");
	// assert(v.type() == var::UINT_TYPE);
	// assert(v.to_int() == 3);

	// v = var::parse_json("-5");
	// assert(v.type() == var::INT_TYPE);
	// assert(v.to_int() == -5);

	// v = var::parse_json("-876.02");
	// assert(v.type() == var::FLOAT_TYPE);
	// assert(v.to_double() == -876.02);

	// v = var::parse_json("5.342");
	// assert(v.to_double() == 5.342);

	// //v = var::parse_json("5.4.");
	// //assert(v.type() == var::NULL_TYPE);

	// v = var::parse_json("\"hello\"");
	// assert(v.to_string() == "hello");

	// v = var::parse_json("[3,5,\"hello\"]");
	// assert(v[0].to_int() == 3);
	// assert(v[1].to_int() == 5);
	// assert(v[2].to_string() == "hello");

	// v = var::parse_json("{\"num\":3,\"str\":\"abcdef\",\"\":4}");
	// if (v.is_error()) std::cout << v << std::endl;
	// assert(v["num"].to_int() == 3);
	// assert(v["str"].to_string() == "abcdef");

	v = var::parse_json("[3,5,\"hello\"]");
	assert(!v.is_error());
	std::string ex_json = "{\"scooby\":{\"snacks\":{\"flavor\":\"spicy\",\"size\":3}},\"num\":3,\"str\":\"asdf\"}";
	v = var::parse_json(ex_json);
	assert(!v.is_error());
	assert(!v["scooby"].is_null());
	assert(v["scooby"].is_table());


	std::string serial = v.to_json();
	assert(!v.is_error());
	v = var::parse_json(pokemon_json);
	assert(v["previous"].is_null());
	assert(v["results"].is_array());
	assert(v["results"][0].is_table());
	assert(v["results"][0]["name"].is_string());
	assert(v["results"][0]["url"].is_string());

	v = var::parse_json(colors_json);
	std::cout << v << std::endl;
	// std::ifstream file;
	// file.open("./colors.json");
	// std::string json_str;
	// json_str.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
	// file.close();
	// v = var::parse_json(json_str);
	// if (v.is_error()) std::cout << v << std::endl;
	// assert(!v.is_error());
	// var& colors = v["colors"];
	// std::cout << colors.type() << std::endl;

	//assert(colors.size() == 
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