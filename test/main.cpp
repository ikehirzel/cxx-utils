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
	assert(v.as_int() == i);\
	assert(v.as_double() == d);\
	assert(v.as_char() == c);\
	assert(v.as_bool() == b);\
	assert(v.as_string() == s);\
	assert(v.is_empty() == e);\

#define check_prim(v, val, t, func)\
	assert(v.type() == t);\
	assert(v.is_##func ());\
	assert(v.as_##func() == val);

// #define check_null(v)			assert(v.type() == Data::NULL_TYPE); assert(v.is_null());
// #define check_char(v, val)		check_prim(v, val, Data::CHAR_TYPE, char)
// #define check_bool(v, val)		check_prim(v, val, Data::BOOL_TYPE, bool)
// #define check_int(v, val)		check_prim(v, val, Data::INT_TYPE, int)
// #define check_float(v, val)		assert(v.type() == Data::FLOAT_TYPE); assert(v.is_float()); assert(v.as_double() == val);
// #define check_string(v, val)	check_prim(v, val, Data::STRING_TYPE, string);
// #define check_table(v, val)		assert(v.type() == Data::TABLE_TYPE); assert(v.is_table()); assert(v.is_empty() == val);
// #define check_array(v, val)		assert(v.type() == Data::ARRAY_TYPE); assert(v.is_array()); assert(v.is_empty() == val);

// #define assert_throws(x, type) { bool throws = false; try { x; } catch (const type& e) { throws = true; } assert(throws); }
// #define assert_not_throws(x, type) { bool does_not_throw = true; try { x; } catch (const type& e) { does_not_throw = false; std::cout << "\033[31merror:\033[0m " << e.what() << std::endl; } assert(does_not_throw); }

// void test_null()
// {
// 	Data v;
// 	assert(v.is_null());
// 	compare(v, Data::NULL_TYPE, 1, 0, 0u, 0.0, (char)0, false, "null", true);
// 	assert(v.as_array().empty());
// 	assert(v.as_table().empty());
// }

// void test_int()
// {
// 	Data v;
// 	assert(!v.is_int());
// 	v = 62;
// 	assert(v.is_int() && v.is_num());
// 	compare(v, Data::INT_TYPE, 1, 62, 62u, 62.0, (char)62, true, "62", false);
// 	v = -1023;
// 	compare(v, Data::INT_TYPE, 1, -1023, (intmax_t)(-1023), -1023.0, (char)-1023, true, "-1023", false);
// 	assert(v.as_array().empty());
// 	assert(v.as_table().empty());
// }

// void test_float()
// {
// 	Data v;
// 	assert(!v.is_float());
// 	v = 12.4;
// 	assert(v.is_float() && v.is_num());
// 	compare(v, Data::FLOAT_TYPE, 1, 12, 12u, 12.4, (char)12, true, std::to_string(12.4), false);
// 	assert(v.as_array().empty());
// 	assert(v.as_table().empty());
// }

// void test_char()
// {
// 	Data v;
// 	assert(!v.is_char());
// 	v = ' ';
// 	assert(v.is_char());
// 	compare(v, Data::CHAR_TYPE, 1, 32, 32u, 32.0, ' ', true, " ", false);
// 	assert(v.as_array().empty());
// 	assert(v.as_table().empty());
// }

// void test_bool()
// {
// 	Data v;
// 	assert(!v.is_bool());
// 	v = true;
// 	assert(v.is_bool());
// 	compare(v, Data::BOOL_TYPE, 1, 1, 1u, 1.0, 1, true, "true", false);
// 	v = false;
// 	compare(v, Data::BOOL_TYPE, 1, 0, 0u, 0.0, 0, false, "false", false);
// 	assert(v.as_array().empty());
// 	assert(v.as_table().empty());
// }

// void test_string()
// {
// 	Data v;
// 	assert(!v.is_string());
// 	v = "";
// 	assert(v.is_string());
// 	compare(v, Data::STRING_TYPE, 0, 0, 0u, 0, (char)0, false, "", true);
// 	v = "HELLO";
// 	compare(v, Data::STRING_TYPE, 5, 0, 0, 0.0, 'H', true, "HELLO", false);
// 	assert(v.as_array().empty());
// 	assert(v.as_table().empty());
// }

// void test_array()
// {
// 	Data v;
// 	assert(!v.is_array());
// 	v = { 1, "HELLO", true };
// 	assert(v.type() == Data::ARRAY_TYPE);
// 	assert(v.is_array());
// 	assert(v.size() == 3);
// 	assert(v.as_array().size() == 3);
// 	assert(v.as_table().size() == 3);
// 	assert(v.as_array()[2].as_string() == v[2].as_string());
// 	assert(!v.is_empty());
// 	check_int(v[0], 1);
// 	check_string(v[1], "HELLO");
// 	check_bool(v[2], true);

// }

// void test_table()
// {
// 	Data v;
// 	assert(!v.is_table());
// 	v["val"] = 2;
// 	assert(v.is_table());
// 	assert(v.contains("val"));
// 	assert(v.size() == 1);
// 	check_int(v["val"], 2);
// 	v[0] = "Hello";
// 	assert(v.contains("0"));
// 	assert(v.size() == 2);
// 	check_string(v[0], "Hello");
// 	v = Data::Table
// 	({
// 		{ "label", 3 }
// 	});
// 	assert(v.is_table());
// 	assert(v["label"].is_num());
// 	assert(v.as_table().size() == 1);
// 	assert(v.as_table()["label"].is_num());
// 	assert(v.size() == 1);
// 	assert(v.as_array().size() == 1);
// 	assert(v.as_array()[0][0].as_string() == "label");
// }

// void test_parse_json()
// {
// 	Data v;
// 	assert_not_throws(v = Data::parse_json("true"), Data::ParseException);
// 	check_bool(v, true);
// 	assert_not_throws(v = Data::parse_json("false"), Data::ParseException);
// 	check_bool(v, false);
// 	assert_not_throws(v = Data::parse_json("null"), Data::ParseException);
// 	check_null(v);
// 	assert_not_throws(v = Data::parse_json("3"), Data::ParseException);
// 	check_int(v, 3);
// 	assert_not_throws(v = Data::parse_json("-5"), Data::ParseException);
// 	check_int(v, -5);
// 	assert_not_throws(v = Data::parse_json("-876.02"), Data::ParseException);
// 	check_float(v, -876.02);
// 	assert_not_throws(v = Data::parse_json("5.342"), Data::ParseException);
// 	check_float(v, 5.342);
// 	assert_throws(Data::parse_json("5.4."), Data::ParseException);

// 	assert_not_throws(v = Data::parse_json("\"hello\""), Data::ParseException);
// 	check_string(v, "hello");

// 	assert_not_throws(v = Data::parse_json("[3,-5,\"hello\"]"), Data::ParseException);
// 	check_int(v[0], 3);
// 	check_int(v[1], -5);
// 	check_string(v[2], "hello");

// 	std::string ex_json = "{\"num\":3,\"str\":\"abcdef\",\"\":-4}";
// 	Data second;
// 	assert_not_throws(v = Data::parse_json(ex_json), Data::ParseException);
// 	assert(v != second);
// 	assert(second != v);
// 	assert_not_throws(second = Data::parse_json(ex_json), Data::ParseException);
// 	assert(v == second);
// 	assert(second == v);
// 	assert_not_throws(second = Data::parse_json(v.as_json()), Data::ParseException);
// 	assert(second == v);
// 	assert(v == second);
// 	assert_not_throws(v = Data::parse_json(second.as_json()), Data::ParseException);
// 	assert(second == v);
// 	assert(v == second);
// 	check_table(v, false);
// 	check_int(v["num"], 3);
// 	check_string(v["str"], "abcdef");
// 	check_int(v[""], -4);

// 	ex_json = "{\"scooby\":{\"snacks\":{\"flavor\":\"spicy\",\"size\":3}},\"num\":3,\"arr\":[3,-5,2]}";
// 	assert_not_throws(v = Data::parse_json(ex_json), Data::ParseException);
// 	assert(v != second);
// 	assert(second != v);
// 	assert_not_throws(second = Data::parse_json(ex_json), Data::ParseException);
// 	assert(v == second);
// 	assert(second == v);
// 	assert_not_throws(second = Data::parse_json(v.as_json()), Data::ParseException);
// 	assert(second == v);
// 	assert(v == second);
// 	assert_not_throws(v = Data::parse_json(second.as_json()), Data::ParseException);
// 	assert(second == v);
// 	assert(v == second);
// 	assert(!v["scooby"].is_null());
// 	assert(v["scooby"].is_table());

// 	assert_not_throws(v = Data::parse_json("[]"), Data::ParseException);
// 	check_array(v, true);

// 	assert_not_throws(v = Data::parse_json("{}"), Data::ParseException);
// 	check_table(v, true);

// 	std::string serial = v.as_json();
// 	assert_not_throws(v = Data::parse_json(pokemon_json), Data::ParseException);
// 	assert(v.contains("previous"));
// 	check_null(v["previous"]);
// 	assert(v.contains("results"));
// 	check_array(v["results"], false);
// 	check_table(v["results"][0], false);
// 	assert(v["results"][0].contains("name"));
// 	check_string(v["results"][0]["name"], "bulbasaur");
// 	assert(v["results"][0].contains("url"));
// 	check_string(v["results"][0]["url"], "https://pokeapi.co/api/v2/pokemon/1/");

// 	assert_not_throws(v = Data::parse_json(colors_json), Data::ParseException);
// 	assert(v.contains("colors"));
// 	check_array(v["colors"], false);
// 	assert(v["colors"].size() == 6);
// 	check_table(v["colors"][0], false);
// 	assert(v["colors"][0].contains("color"));
// 	check_string(v["colors"][0]["color"], "black");
// }

#define TEST(name) std::cout << "Testing " #name "...\n"; test_##name(); std::cout << "\t\tAll tests passed\n";


void test_new_data_format()
{
	Data data;
	assert(data.is_null());
	std::cout << "Data: " << data << std::endl;
	std::cout << "Data['key']: " << data["key"] << std::endl;
	std::cout << "made data a table\n";
	data = Data::Table();
	std::cout << "Data['key']: " << data["key"] << std::endl;
	

	std::cout << data.type() << std::endl;
}

int main()
{
	test_new_data_format();
	// TEST(null);
	// TEST(int);
	// TEST(float);
	// TEST(char);
	// TEST(bool);
	// TEST(string);
	// TEST(array);
	// TEST(table);
	// TEST(parse_json);
	return 0;
}
