#define HIRZEL_IMPLEMENT
#include <hirzel/data/json.h>

#include <iostream>

#include "assert.h"

using namespace hirzel::data;

#define assert_parse_throws(json) assert_throws(parse_json(json), JsonException)
#define assert_parse_not_throws(json) assert_no_throw(parse_json(json))

#define assert_type(var, type) assert_true(var.is_##type(), "Expected type '" #type "' but got '" + std::string(var.type_name()) + "'")
#define assert_value(var, func, value) assert_true(var.as_##func() == value, "Expected value of " #value " but got " + var.as_string())

#define assert_json(json, func, value) {\
	assert_parse_not_throws(json);\
	auto data = parse_json(json);\
	assert_true(data.get_##func() == value, "Expected value '" #value "' but got '" + data.as_string() + "'");\
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

void test_null()
{
	// valid
	assert_parse_not_throws("null");
	assert_true(parse_json("null") == Data(), "Expected data of type 'null' but got " + parse_json("null").as_string());

	// invalid
	assert_parse_throws("nub");
	assert_parse_throws("nul");
	assert_parse_throws("nulll");
	assert_parse_throws("nll");
}

void test_integer()
{
	// valid
	assert_json("1", integer, 1);
	assert_json("-1", integer, -1);
	assert_json("1526227", integer, 1526227);
	assert_json("-254", integer, -254);
	assert_json("-0", integer, 0);
	assert_json("0", integer, 0);

	// invalid
	assert_parse_throws("1526a");
	assert_parse_throws("15a26");
	assert_parse_not_throws("1e1");
	assert_parse_not_throws("2e7");
	assert_parse_not_throws("214e7");
}

void test_decimal()
{
	// valid
	assert_json("1.234", decimal, 1.234);
	assert_json("185.22334", decimal, 185.22334);
	assert_json("1.2e1", decimal, 1.2e1);
	assert_json("1.2e3", decimal, 1.2e3);
	assert_json("23.1e12", decimal, 23.1e12);

	// invalid
	assert_parse_throws(".234");
	assert_parse_throws("1.");
	assert_parse_throws("1.2.3");
	assert_parse_throws(".123e1");
	assert_parse_throws("23.e1");
	assert_parse_throws("23.1e1.2");
}

void test_boolean()
{
	// valid
	assert_json("true", boolean, true);
	assert_json("false", boolean, false);

	// invalid
	assert_parse_throws("falsey");
	assert_parse_throws("truey");
	assert_parse_throws("tabc");
	assert_parse_throws("fabdc");
}

void test_string()
{
	// valid
	assert_json("\"\"", string, "");
	assert_json("\"hello\"", string, "hello");

	// invalid
	assert_parse_throws("\"");
	assert_parse_throws("\"hello");
	assert_parse_throws("\"hello\"\"");
	assert_parse_throws("this is a string\"");
} 

void test_array()
{
}

void test_table()
{
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
	
	// Data v;
	// assert_parse_not_throws("null");
	// v = parse_json("null");
	// assert_true(v.is_null(), "");

	// assert_parse_not_throws("3");
	// assert_true(v.is_int(), "");
	
	// assert_parse_not_throws("-5");
	// assert_int(v, -5);
	// assert_parse_not_throws("-876.02");
	// assert_float(v, -876.02);
	// assert_parse_not_throws("5.342");
	// assert_float(v, 5.342);
	// assert_parse_throws("5.4.");

	

	// assert_parse_not_throws(R"([3,-5,"hello"])");
	// assert_int(v[0], 3);
	// assert_int(v[1], -5);
	// assert_string(v[2], 0, 0.0, true, "hello");

	// std::string ex_json = R"({"num":3,"str":"abcdef","":-4})";
	// Data second;
	// assert_parse_not_throws(ex_json);
	// assert(v != second);
	// assert(second != v);
	// assert_parse_not_throws(ex_json);
	// assert(v == second);
	// assert(second == v);
	// assert_parse_not_throws(v.as_json());
	// assert(second == v);
	// assert(v == second);
	// assert_parse_not_throws(second.as_json()));
	// assert(second == v);
	// assert(v == second);
	// assert_table(v);
	// assert_int(v["num"], 3);
	// assert_string(v["str"], 0, 0.0, true, "abcdef");
	// assert_int(v[""], -4);

	// ex_json = R"({"scooby":{"snacks":{"flavor":"spicy","size":3}},"num":3,"arr":[3,-5,2]})";
	// assert_parse_not_throws(ex_json));
	// assert_true(v != second, "V should not be equivalent to second);
	// assert_true(second != v, "Second should not be equivalent to v");
	// assert_parse_not_throws(second = parse_json(ex_json));
	// assert_true(v == second, "");
	// assert_true(second == v, "");
	// assert_parse_not_throws(second = parse_json(v.as_json()));
	// assert(second == v);
	// assert(v == second);
	// assert_parse_not_throws(second.as_json()));
	// assert(second == v);
	// assert(v == second);
	// assert(!v["scooby"].is_null());
	// assert(v["scooby"].is_table());

	// assert_parse_not_throws("[]"));
	// assert_array(v);
	// assert(v.size() == 0);
	// assert(!v.as_bool());

	// assert_parse_not_throws("{}"));
	// assert_table(v);
	// assert(v.size() == 0);

	// std::string serial = v.as_json();
	// assert_parse_not_throws(pokemon_json));
	// assert(v.contains("previous"));
	// assert_null(v["previous"]);
	// assert(v.contains("results"));
	// assert_array(v["results"]);
	// assert_table(v["results"][0]);
	// assert(v["results"][0].contains("name"));
	// assert_string(v["results"][0]["name"], 0, 0.0, true, "bulbasaur");
	// assert(v["results"][0].contains("url"));
	// assert_string(v["results"][0]["url"], 0, 0.0, true, "https://pokeapi.co/api/v2/pokemon/1/");

	// assert_parse_not_throws(colors_json));
	// assert(v.contains("colors"));
	// assert_array(v["colors"]);
	// assert(v["colors"].size() == 6);
	// assert_table(v["colors"][0]);
	// assert(v["colors"][0].contains("color"));
	// assert_string(v["colors"][0]["color"], 0, 0.0, true, "black");

	return 0;
}
