#include <hirzel/data.h>
#include <cassert>
#include <iostream>
#include <fstream>

using hirzel::Data;

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
	assert(v.empty() == e);\

#define check_prim(v, val, t, func)\
	assert(v.type() == t);\
	assert(v.is_##func ());\
	assert(v.to_##func() == val);

#define check_null(v)			assert(v.type() == Data::NULL_TYPE); assert(v.is_null());
#define check_char(v, val)		check_prim(v, val, Data::CHAR_TYPE, char)
#define check_bool(v, val)		check_prim(v, val, Data::BOOL_TYPE, bool)
#define check_int(v, val)		check_prim(v, val, Data::INT_TYPE, int)
#define check_uint(v, val)		check_prim(v, val, Data::UINT_TYPE, uint)
#define check_float(v, val)		assert(v.type() == Data::FLOAT_TYPE); assert(v.is_float()); assert(v.to_double() == val);
#define check_string(v, val)	check_prim(v, val, Data::STRING_TYPE, string);
#define check_table(v, val)		assert(v.type() == Data::TABLE_TYPE); assert(v.is_table()); assert(v.empty() == val);
#define check_array(v, val)		assert(v.type() == Data::ARRAY_TYPE); assert(v.is_array()); assert(v.empty() == val);

#define assert_throws(x, type) { bool throws = false; try { x; } catch (type e) { throws = true; } assert(throws); }

void test_null()
{
	Data v;
	assert(v.is_null());
	compare(v, Data::NULL_TYPE, 1, 0, 0u, 0.0, (char)0, false, "null", true);
	assert(v.to_array().empty());
	assert(v.to_table().empty());
}

void test_int()
{
	Data v;
	assert(!v.is_int());
	v = 62;
	assert(v.is_int() && v.is_num());
	compare(v, Data::INT_TYPE, 1, 62, 62u, 62.0, (char)62, true, "62", false);
	v = -1023;
	compare(v, Data::INT_TYPE, 1, -1023, (uintmax_t)(-1023), -1023.0, (char)-1023, true, "-1023", false);
	assert(v.to_array().empty());
	assert(v.to_table().empty());
}

void test_uint()
{
	Data v;
	assert(!v.is_uint());
	v = 45u;
	assert(v.is_uint() && v.is_num());
	compare(v, Data::UINT_TYPE, 1, 45, 45u, 45.0, (char)45, true, "45", false);
	assert(v.to_array().empty());
	assert(v.to_table().empty());
}

void test_float()
{
	Data v;
	assert(!v.is_float());
	v = 12.4;
	assert(v.is_float() && v.is_num());
	compare(v, Data::FLOAT_TYPE, 1, 12, 12u, 12.4, (char)12, true, std::to_string(12.4), false);
	assert(v.to_array().empty());
	assert(v.to_table().empty());
}

void test_char()
{
	Data v;
	assert(!v.is_char());
	v = ' ';
	assert(v.is_char());
	compare(v, Data::CHAR_TYPE, 1, 32, 32u, 32.0, ' ', true, " ", false);
	assert(v.to_array().empty());
	assert(v.to_table().empty());
}

void test_bool()
{
	Data v;
	assert(!v.is_bool());
	v = true;
	assert(v.is_bool());
	compare(v, Data::BOOL_TYPE, 1, 1, 1u, 1.0, 1, true, "true", false);
	v = false;
	compare(v, Data::BOOL_TYPE, 1, 0, 0u, 0.0, 0, false, "false", false);
	assert(v.to_array().empty());
	assert(v.to_table().empty());
}

void test_string()
{
	Data v;
	assert(!v.is_string());
	v = "";
	assert(v.is_string());
	compare(v, Data::STRING_TYPE, 0, 0, 0u, 0, (char)0, false, "", true);
	v = "HELLO";
	compare(v, Data::STRING_TYPE, 5, 0, 0, 0.0, 'H', true, "HELLO", false);
	assert(v.to_array().empty());
	assert(v.to_table().empty());
}

void test_array()
{
	Data v;
	assert(!v.is_array());
	v = { 1, "HELLO", true };
	assert(v.type() == Data::ARRAY_TYPE);
	assert(v.is_array());
	assert(v.size() == 3);
	assert(v.to_array().size() == 3);
	assert(v.to_table().size() == 3);
	assert(v.to_array()[2].to_string() == v[2].to_string());
	assert(!v.empty());
	check_int(v[0], 1);
	check_string(v[1], "HELLO");
	check_bool(v[2], true);

}

void test_table()
{
	Data v;
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
	v = Data::Table
	({
		{ "label", 3 }
	});
	assert(v.is_table());
	assert(v["label"].is_num());
	assert(v.to_table().size() == 1);
	assert(v.to_table()["label"].is_num());
	assert(v.size() == 1);
	assert(v.to_array().size() == 1);
	assert(v.to_array()[0][0].to_string() == "label");
}

void test_parse_json()
{
	Data v;
	v = Data::parse_json("true");
	check_bool(v, true);
	v = Data::parse_json("false");
	check_bool(v, false);
	v = Data::parse_json("null");
	check_null(v);
	v = Data::parse_json("3");
	check_uint(v, 3);
	v = Data::parse_json("-5");
	check_int(v, -5);
	v = Data::parse_json("-876.02");
	check_float(v, -876.02);
	v = Data::parse_json("5.342");
	check_float(v, 5.342);
	assert_throws(Data::parse_json("5.4."), Data::ParseException);

	v = Data::parse_json("\"hello\"");
	check_string(v, "hello");

	v = Data::parse_json("[3,-5,\"hello\"]");
	check_uint(v[0], 3);
	check_int(v[1], -5);
	check_string(v[2], "hello");

	std::string ex_json = "{\"num\":3,\"str\":\"abcdef\",\"\":-4}";
	v = Data::parse_json(ex_json);
	assert(v.to_json() == ex_json);
	check_table(v, false);
	check_uint(v["num"], 3);
	check_string(v["str"], "abcdef");
	check_int(v[""], -4);

	ex_json = "{\"scooby\":{\"snacks\":{\"flavor\":\"spicy\",\"size\":3}},\"num\":3,\"arr\":[3,-5,2]}";
	v = Data::parse_json(ex_json);
	assert(v.to_json() == ex_json);
	assert(!v["scooby"].is_null());
	assert(v["scooby"].is_table());

	v = Data::parse_json("[]");
	check_array(v, true);
	v = Data::parse_json("{}");
	check_table(v, true);

	std::string serial = v.to_json();
	v = Data::parse_json(pokemon_json);
	assert(v.contains("previous"));
	check_null(v["previous"]);
	assert(v.contains("results"));
	check_array(v["results"], false);
	check_table(v["results"][0], false);
	assert(v["results"][0].contains("name"));
	check_string(v["results"][0]["name"], "bulbasaur");
	assert(v["results"][0].contains("url"));
	check_string(v["results"][0]["url"], "https://pokeapi.co/api/v2/pokemon/1/");

	v = Data::parse_json(colors_json);
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
