#include <hirzel/data/json.h>

#include <iostream>

#include "assert.h"

using namespace hirzel::data;

#define STRI(x) #x
#define STR(x) STRI(x)
#define LINE STR(__LINE__)
#define LOC __FILE__ " @ " LINE ": "
#define ASSERT_FAIL LOC "Assertion failed! "

#define assert_parse_throws(json) {\
	try {\
		parse_json(json);\
		std::cout << ASSERT_FAIL "Expected to catch JsonException but no exception was thrown.\n";\
		std::abort();\
	} catch (const JsonException& e) {\
	} catch (const std::exception& e) {\
		std::cout << ASSERT_FAIL "Expected no exceptions but caught unhandled exception:\n\t" << e.what() << std::endl;\
		std::abort();\
	} catch (...) {\
		std::cout << ASSERT_FAIL "Expected to catch JsonException but got unknown error." << std::endl;\
		std::abort();\
	}\
}

#define assert_parse_not_throws(json) {\
	try {\
		parse_json(json);\
	} catch (const JsonException& e) {\
		std::cout << ASSERT_FAIL "Expected no exceptions but caught JsonException:\n\t" << e.what() << std::endl;\
		std::abort();\
	} catch (const std::exception& e) {\
		std::cout << ASSERT_FAIL "Expected no exceptions but caught unhandled exception:\n\t" << e.what() << std::endl;\
		std::abort();\
	} catch (...) {\
		std::cout << ASSERT_FAIL "Expected no exceptions but caught unknown error\n";\
		std::abort();\
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

void test_integer()
{
}

void test_decimal()
{
}

void test_boolean()
{
}

void test_string()
{
}

void test_array()
{
}

void test_table()
{
}

int main()
{
	Data v;
	assert_parse_not_throws("true");
	v = parse_json("true");
	assert(v.is_boolean());
	assert(v.as_bool() == true);

	assert_parse_not_throws("false");
	v = parse_json("false");
	assert(v.is_boolean());
	assert(v.as_bool() == false);

	assert_parse_not_throws("null");
	v = parse_json("null");
	assert(v.is_null());

	assert_parse_not_throws("3");
	assert_int(v, 3);
	
	assert_parse_not_throws("-5");
	assert_int(v, -5);
	assert_parse_not_throws("-876.02");
	assert_float(v, -876.02);
	assert_parse_not_throws("5.342");
	assert_float(v, 5.342);
	assert_parse_throws("5.4.");

	assert_parse_not_throws("\"hello\"");
	assert_string(v, 0, 0.0, true, "hello");

	assert_parse_not_throws(R"([3,-5,"hello"])");
	assert_int(v[0], 3);
	assert_int(v[1], -5);
	assert_string(v[2], 0, 0.0, true, "hello");

	std::string ex_json = R"({"num":3,"str":"abcdef","":-4})";
	Data second;
	assert_parse_not_throws(ex_json);
	assert(v != second);
	assert(second != v);
	assert_parse_not_throws(ex_json);
	assert(v == second);
	assert(second == v);
	assert_parse_not_throws(v.as_json());
	assert(second == v);
	assert(v == second);
	assert_parse_not_throws(second.as_json()));
	assert(second == v);
	assert(v == second);
	assert_table(v);
	assert_int(v["num"], 3);
	assert_string(v["str"], 0, 0.0, true, "abcdef");
	assert_int(v[""], -4);

	ex_json = R"({"scooby":{"snacks":{"flavor":"spicy","size":3}},"num":3,"arr":[3,-5,2]})";
	assert_parse_not_throws(ex_json));
	assert(v != second);
	assert(second != v);
	assert_parse_not_throws(second = parse_json(ex_json));
	assert(v == second);
	assert(second == v);
	assert_parse_not_throws(second = parse_json(v.as_json()));
	assert(second == v);
	assert(v == second);
	assert_parse_not_throws(second.as_json()));
	assert(second == v);
	assert(v == second);
	assert(!v["scooby"].is_null());
	assert(v["scooby"].is_table());

	assert_parse_not_throws("[]"));
	assert_array(v);
	assert(v.size() == 0);
	assert(!v.as_bool());

	assert_parse_not_throws("{}"));
	assert_table(v);
	assert(v.size() == 0);

	std::string serial = v.as_json();
	assert_parse_not_throws(pokemon_json));
	assert(v.contains("previous"));
	assert_null(v["previous"]);
	assert(v.contains("results"));
	assert_array(v["results"]);
	assert_table(v["results"][0]);
	assert(v["results"][0].contains("name"));
	assert_string(v["results"][0]["name"], 0, 0.0, true, "bulbasaur");
	assert(v["results"][0].contains("url"));
	assert_string(v["results"][0]["url"], 0, 0.0, true, "https://pokeapi.co/api/v2/pokemon/1/");

	assert_parse_not_throws(colors_json));
	assert(v.contains("colors"));
	assert_array(v["colors"]);
	assert(v["colors"].size() == 6);
	assert_table(v["colors"][0]);
	assert(v["colors"][0].contains("color"));
	assert_string(v["colors"][0]["color"], 0, 0.0, true, "black");

	return 0;
}
