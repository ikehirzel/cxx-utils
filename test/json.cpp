#define HIRZEL_IMPLEMENT
#include <hirzel/data/json.h>

#include <iostream>

#include "assert.h"

using namespace hirzel;

#define assert_equals(a, b) {\
	assert_true(a == b);\
	assert_true(b == a);\
	assert_false(a != b);\
	assert_false(b != a);\
}

#define assert_parse_throws(json) assert_throws(Json::deserialize(json), Json::SyntaxError)
#define assert_parse_not_throws(json) assert_no_throw(Json::deserialize(json))

#define assert_type(var, type) assert_true_msg(var.is_##type(), "Expected type '" #type "' but got '" + std::string(var.type_name()) + "'")
#define assert_value(var, func, value) assert_true_msg(var.as_##func() == value, "Expected value of " #value " but got " + var.as_string())

#define assert_json(json, value) {\
	assert_parse_not_throws(json);\
	auto data = Json::deserialize(json);\
	auto expected = Data(value);\
	assert_true(data == expected);\
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

	Data null = Json::deserialize("null");
	assert_type(null, null);
	assert_true(null == Data());

	// invalid
	assert_parse_throws("nub");
	assert_parse_throws("nul");
	assert_parse_throws("nulll");
	assert_parse_throws("nll");
}

void test_integer()
{
	// valid
	assert_json("1", 1);
	assert_json("-1", -1);
	assert_json("1526227", 1526227);
	assert_json("-254", -254);
	assert_json("-0", 0);
	assert_json("0", 0);

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
	assert_json("1.234", 1.234);
	assert_json("-11.234", -11.234);
	assert_json("185.22334", 185.22334);
	assert_json("-1.2e1", -1.2e1);
	assert_json("1.2e3", 1.2e3);
	assert_json("23.1e12", 23.1e12);

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
	assert_json("true", true);
	assert_json("false", false);

	// invalid
	assert_parse_throws("falsey");
	assert_parse_throws("truey");
	assert_parse_throws("tabc");
	assert_parse_throws("fabdc");
}

void test_string()
{
	// valid
	assert_json("\"\"", "");
	assert_json("\"hello\"", "hello");
	assert_json("\"123\"", "123");
	assert_json("\"   \"", "   ");

	// invalid
	assert_parse_throws("\"");
	assert_parse_throws("\"hello");
	assert_parse_throws("\"hello\"\"");
	assert_parse_throws("this is a string\"");
} 

void test_array()
{
	using Array = Data::Array;

	assert_json("[]", Array());
	assert_json("[1,2,3]", Array({ 1, 2, 3 }));
	assert_json("[234, \"hello\", null, true, {}]", Array({ 234, "hello", Data(), true, Data::Table() }));
	assert_json("[[{}]]", Array({ Array({ Data::Table() }) }) );
	assert_json("[[[]]]", Array({ Array({ Array() }) }) );
	assert_json("[[1,2,3], [2,3,4], {\"key\":true}]", Array({ Array({ 1, 2, 3 }), Array({ 2, 3, 4 }), Data::Table({ { "key", true } }) }));

	assert_parse_throws("[");
	assert_parse_throws("]");
	assert_parse_throws("[1");
	assert_parse_throws("[h");
	assert_parse_throws("1]");
	assert_parse_throws("[1,]");
	assert_parse_throws("[1,h]");
	assert_parse_throws("[1,true");
	assert_parse_throws("[1,true,");
}

void test_table()
{
	using Table = Data::Table;

	assert_json("{}", Table());
	assert_json("{\"key\":1}", Table({ {"key", 1} }));
	assert_json("{\"abc\":[true, false]}", Table({ { "abc", Data::Array({ true, false }) } }));
	assert_json("{\"key\":\"value\",\"number\":298}", Table({ { "key", "value" }, { "number", 298 } }));
	assert_json("{\"table\":{\"subtable\":567}}", Table({ { "table", Table({ { "subtable", 567 } }) } }));

	assert_parse_throws("{");
	assert_parse_throws("}");
	assert_parse_throws("{a}");
	assert_parse_throws("{\"a\"}");
	assert_parse_throws("{\"a\":}");
	assert_parse_throws("{\"a\":1,}");
	assert_parse_throws("{a");
	assert_parse_throws("a}");
	assert_parse_throws("{\"a\"");
	assert_parse_throws("{\"a\":");
	assert_parse_throws("{\"a\":1");
	assert_parse_throws("{\"a\":1,");
	assert_parse_throws("{\"a\":1,}");
}

void test_json()
{
	using Table = Data::Table;
	using Array = Data::Array;

	assert_parse_not_throws(colors_json);
	auto colors_data = Table{
		{ "colors", Array{
				Table {
					{ "color", "black" },
					{ "category", "hue" },
					{ "type", "primary" },
					{ "code", Table {
							{ "rgba", Array{ 255, 255, 255, 1} },
							{ "hex", "#000" }
						}
					}
				},
				Table {
					{ "color", "white" },
					{ "category", "value" },
					{ "code", Table {
							{ "rgba", Array { 0,0,0,1} },
							{ "hex", "#FFF" }
						}
					}
				},
				Table {
					{ "color", "red" },
					{ "category", "hue" },
					{ "type", "primary" },
					{ "code", Table {
							{ "rgba", Array { 255,0,0,1 } },
							{ "hex", "#FF0" }
						}
					}
				},
				Table {
					{ "color", "blue" },
					{ "category", "hue" },
					{ "type", "primary" },
					{ "code", Table {
							{ "rgba", Array { 0,0,255,1} },
							{ "hex", "#00F" }
						}
					}
				},
				Table {
					{ "color", "yellow" },
					{ "category", "hue" },
					{ "type", "primary" },
					{ "code", Table {
							{ "rgba", Array { 255,255,0,1 } },
							{ "hex", "#FF0" }
						}
					}
				},
				Table {
					{ "color", "green" },
					{ "category", "hue" },
					{ "type", "secondary" },
					{ "code", Table {
							{ "rgba", Array { 0,255,0,1 } },
							{ "hex", "#0F0" }
						}
					}
				}
			}
		}
	};
	assert_json(colors_json, colors_data);

	assert_parse_not_throws(pokemon_json);
	auto pokemon_data = Table {
		{ "count", 1118 },
		{ "next", "https://pokeapi.co/api/v2/pokemon?offset=20&limit=20"},
		{ "previous", Data() },
		{ "results", Array {
				Table {
					{ "name", "bulbasaur"},
					{ "url", "https://pokeapi.co/api/v2/pokemon/1/"}
				},
				Table {
					{ "name", "ivysaur" },
					{ "url", "https://pokeapi.co/api/v2/pokemon/2/" }
				},
				Table {
					{ "name", "venusaur" },
					{ "url", "https://pokeapi.co/api/v2/pokemon/3/" }
				},
				Table {
					{ "name", "charmander" },
					{ "url", "https://pokeapi.co/api/v2/pokemon/4/" }
				},
				Table {
					{ "name", "charmeleon" },
					{ "url", "https://pokeapi.co/api/v2/pokemon/5/" }
				},
				Table {
					{ "name", "charizard" },
					{ "url", "https://pokeapi.co/api/v2/pokemon/6/" }
				},
				Table {
					{ "name", "squirtle" },
					{ "url", "https://pokeapi.co/api/v2/pokemon/7/" }
				},
				Table {
					{ "name", "wartortle" },
					{ "url", "https://pokeapi.co/api/v2/pokemon/8/" }
				},
				Table {
					{ "name", "blastoise" },
					{ "url", "https://pokeapi.co/api/v2/pokemon/9/" }
				},
				Table {
					{ "name", "caterpie" },
					{ "url", "https://pokeapi.co/api/v2/pokemon/10/" }
				},
				Table {
					{ "name", "metapod" },
					{ "url", "https://pokeapi.co/api/v2/pokemon/11/" }
				},
				Table {
					{ "name", "butterfree" },
					{ "url", "https://pokeapi.co/api/v2/pokemon/12/" }
				},
				Table {
					{ "name", "weedle" },
					{ "url", "https://pokeapi.co/api/v2/pokemon/13/" }
				},
				Table {
					{ "name", "kakuna" },
					{ "url", "https://pokeapi.co/api/v2/pokemon/14/" }
				},
				Table {
					{ "name", "beedrill" },
					{ "url", "https://pokeapi.co/api/v2/pokemon/15/" }
				},
				Table {
					{ "name", "pidgey" },
					{ "url", "https://pokeapi.co/api/v2/pokemon/16/" }
				},
				Table {
					{ "name", "pidgeotto" },
					{ "url", "https://pokeapi.co/api/v2/pokemon/17/" }
				},
				Table {
					{ "name", "pidgeot" },
					{ "url", "https://pokeapi.co/api/v2/pokemon/18/" }
				},
				Table {
					{ "name", "rattata" },
					{ "url", "https://pokeapi.co/api/v2/pokemon/19/" }
				},
				Table {
					{ "name", "raticate" },
					{ "url", "https://pokeapi.co/api/v2/pokemon/20/" }
				}
			}
		}
	};

	assert_json(pokemon_json, pokemon_data);

	// testing serialization
	auto pokemon_expected = Data(pokemon_data);
	auto from_json = Json::deserialize(pokemon_json);
	auto serialized_json = Json::serialize(pokemon_expected);
	auto from_json_clone = Json::deserialize(serialized_json);

	assert_true(from_json == pokemon_expected);
	assert_true(from_json_clone == from_json);
	assert_true(from_json_clone == pokemon_expected);

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
	test(json);

	return 0;
}
