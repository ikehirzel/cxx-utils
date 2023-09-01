#define HIRZEL_IMPLEMENT

#include <hirzel/json.hpp>
#include <iostream>
#include <cassert>
#include <hirzel/print.hpp>

using namespace hirzel;
using namespace hirzel::json;

const char* colorsJson =
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

const char* pokemonJson= R"====(
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

void assert_cast_values(const JsonValue& obj, i64 intValue, f64 floatValue, bool boolValue, const std::string&)
{
	assert(obj.asInteger() == intValue);
	assert(obj.asFloat() == floatValue);
	assert(obj.asBoolean() == boolValue);
	// TODO: Trim zeroes and spaces to make this comparison work
	//assert(obj.asString() == stringValue);
}

void assert_primitive(const JsonValue& obj, JsonValueType type)
{
	// TODO: Check value?
	assert(obj.type() == type);
	//assert(obj.is##funcname());
	assert(obj.length() == 0);
}

void assert_not_primitive(const JsonValue& obj, JsonValueType type)
{
	assert(obj.type() != type);
	//assert(!obj.is##funcname())
}

void assert_not_null(const JsonValue& obj)
{
	assert(obj.type() != JsonValueType::Null);
	assert(!obj.isNull());
}

void assert_not_bool(const JsonValue& obj)
{
	assert(obj.type() != JsonValueType::Boolean);
	assert(!obj.isBoolean());
}

void assert_not_int(const JsonValue& obj)
{
	assert(obj.type() != JsonValueType::Integer);
	assert(!obj.isInteger());
}

void assert_not_float(const JsonValue& obj)
{
	assert(obj.type() != JsonValueType::Float);
	assert(!obj.isFloat());
}

void assert_not_string(const JsonValue& obj)
{
	assert(obj.type() != JsonValueType::String);
	assert(!obj.isString());
}

void assert_not_array(const JsonValue& obj)
{
	assert(obj.type() != JsonValueType::Array);
	assert(!obj.isArray());
}

void assert_not_object(const JsonValue& obj)
{
	assert(obj.type() != JsonValueType::Object);
	assert(!obj.isObject());
}

void assert_equals(const JsonValue& a, const JsonValue& b)
{
	assert(a == b);
	assert(b == a);
	assert(!(a != b));
	assert(!(b != a));
}

void assert_not_equals(const JsonValue& a, const JsonValue& b)
{
	assert(!(a == b));
	assert(!(b == a));
	assert(a != b);
	assert(b != a);
}

void assert_null(const JsonValue& obj)
{
	assert(obj.type() == JsonValueType::Null);
	assert(obj.isNull());
	assert(obj.length() == 0);
	assert_cast_values(obj, 0, 0.0, false, "null");
	assert_not_int(obj);
	assert_not_float(obj);
	assert_not_bool(obj);
	assert_not_string(obj);
	assert_not_array(obj);
	assert_not_object(obj);
	assert_equals(obj, JsonValue());
	assert_equals(obj, JsonValue(JsonValueType::Null));
}

template <typename V>
void assert_boolean(const JsonValue& obj, const V& value)
{
	assert_primitive(obj, JsonValueType::Boolean);
	assert_cast_values(obj, (int)value, (double)value, value, (value ? "true" : "false"));
	assert_not_null(obj);
	assert_not_int(obj);
	assert_not_float(obj);
	assert_not_string(obj);
	assert_not_array(obj);
	assert_not_object(obj);
	assert_equals(obj, JsonValue(value));
}

void assert_integer(const JsonValue& obj, i64 value)
{
	assert_primitive(obj, JsonValueType::Integer);
	assert(obj.integer() == value);
	assert(obj.isNumber());
	assert_cast_values(obj, value, (double)value, (bool)value, std::to_string(value));
	assert_not_null(obj);
	assert_not_bool(obj);
	assert_not_float(obj);
	assert_not_string(obj);
	assert_not_array(obj);
	assert_not_object(obj);
	assert_equals(obj, JsonValue(value));
}

void assert_decimal(const JsonValue& obj, f64 value)
{
	assert_primitive(obj, JsonValueType::Float);
	assert(obj.decimal() == value);
	assert_cast_values(obj, (int)value, (double)value, (bool)value, std::to_string(value));
	assert(obj.isNumber());
	assert_not_null(obj);
	assert_not_int(obj);
	assert_not_bool(obj);
	assert_not_string(obj);
	assert_not_array(obj);
	assert_not_object(obj);
	assert_equals(obj, JsonValue(value));
}

void assert_string(const JsonValue& obj, i64 intValue, f64 floatValue, bool boolValue, const char* stringValue)
{
	assert(obj.type() == JsonValueType::String);
	assert(std::string(obj.typeName()) == "string");
	assert(obj.isString());
	assert(obj.length() == obj.asString().length());
	assert_cast_values(obj, intValue, floatValue, boolValue, stringValue);
	assert_not_null(obj);
	assert_not_int(obj);
	assert_not_float(obj);
	assert_not_bool(obj);
	assert_not_array(obj);
	assert_not_object(obj);
	assert_equals(obj, JsonValue(stringValue));
}

void assert_array(const JsonValue& obj, const JsonArray& value)
{
	assert(obj.type() == JsonValueType::Array);
	assert(obj.isArray());
	assert_not_null(obj);
	assert_not_int(obj);
	assert_not_float(obj);
	assert_not_bool(obj);
	assert_not_string(obj);
	assert_not_object(obj);
	assert_equals(obj, JsonValue(value));
}

void assert_object(const JsonValue& obj, const JsonObject& value)
{
	assert(obj.type() == JsonValueType::Object);
	assert(obj.isObject());
	assert_not_null(obj);
	assert_not_bool(obj);
	assert_not_int(obj);
	assert_not_float(obj);
	assert_not_string(obj);
	assert_not_array(obj);
	assert_equals(obj, JsonValue(value));
}

void assert_parse_throws(const char *json)
{
	try
	{
		deserialize(json);
	}
	catch (const std::exception&)
	{
		return;
	}

	throw std::runtime_error("Expected parsing '" + std::string(json) + "' to throw exception.");
}

void assert_parse_not_throws(const char *json)
{
	deserialize(json);
}

template <typename V>
void assert_json(const char* json, const V& value)
{
	auto jsonValue = deserialize(json);
	auto expected = JsonValue(value);

	assert(jsonValue == expected);
}

void test_null()
{
	auto init = JsonValue();
	assert_null(init);

	auto copy = JsonValue(init);
	assert_null(copy);
	assert_equals(copy, init);
	copy = init;
	assert_null(copy);
	assert_equals(copy, init);

	JsonValue move(std::move(init));
	assert_null(move);
	assert_equals(move, copy);
	JsonValue clone = copy;
	move = std::move(copy);
	assert_null(move);
	assert_equals(move, clone);

	assert_null(JsonValue());
	assert_null(JsonValue(JsonValueType::Null));

	assert_not_equals(move, JsonValue(143));
	assert_not_equals(move, JsonValue(234.82));
	assert_not_equals(move, JsonValue("hello"));
	assert_not_equals(move, JsonValue(true));
	assert_not_equals(move, JsonValue(JsonArray()));
	assert_not_equals(move, JsonValue(JsonObject()));

	// valid
	assert_parse_not_throws("null");

	auto null = deserialize("null");
	assert(null.isNull());
	assert(null == JsonValue());

	// invalid
	assert_parse_throws("n");
	assert_parse_throws("nub");
	assert_parse_throws("nul");
	assert_parse_throws("nullb");
	assert_parse_throws("nulll");
	assert_parse_throws("nll");
}

void test_integer()
{
	JsonValue init = 234;
	assert_integer(init, 234);

	JsonValue copy(init);
	assert_integer(copy, 234);
	assert_equals(copy, init);
	copy = init;
	assert_integer(copy, 234);
	assert_equals(copy, init);

	JsonValue move(std::move(init));
	assert_integer(move, 234);
	assert_equals(move, copy);
	JsonValue clone = copy;
	move = std::move(copy);
	assert_integer(move, 234);
	assert_equals(move, clone);

	assert_integer(JsonValue(0), 0);
	assert_integer(JsonValue(-0), -0);
	assert_integer(JsonValue(1), 1);
	assert_integer(JsonValue(-1), -1);
	assert_integer(JsonValue(4L), 4L);
	assert_integer(JsonValue(4U), 4U);
	assert_integer(JsonValue(-1023), -1023);
	assert_integer(JsonValue(62), 62);

	assert_not_equals(move, JsonValue());
	assert_not_equals(move, JsonValue(130));
	assert_not_equals(move, JsonValue(-130));
	assert_not_equals(move, JsonValue(-130.52));
	assert_not_equals(move, JsonValue("hello"));
	assert_not_equals(move, JsonValue(false));
	assert_not_equals(move, JsonValue(JsonValueType::Array));
	assert_not_equals(move, JsonValue(JsonValueType::Object));

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

void test_float()
{
	JsonValue init = 2.34;
	assert_decimal(init, 2.34);

	JsonValue copy(init);
	assert_decimal(copy, 2.34);
	assert_equals(copy, init);
	copy = init;
	assert_decimal(copy, 2.34);
	assert_equals(copy, init);

	JsonValue move(std::move(init));
	assert_decimal(move, 2.34);
	assert_equals(move, copy);
	JsonValue clone = copy;
	move = std::move(copy);
	assert_decimal(move, 2.34);
	assert_equals(move, clone);

	assert_decimal(JsonValue(JsonValueType::Float), 0.0);
	assert_decimal(JsonValue(1.34), 1.34);
	assert_decimal(JsonValue(0.0f), 0.0f);
	assert_decimal(JsonValue(-0.0), -0.0);
	assert_decimal(JsonValue(-123592.235), -123592.235);
	assert_decimal(JsonValue(2.0f), 2.0f);

	assert_not_equals(move, JsonValue());
	assert_not_equals(move, JsonValue(130));
	assert_not_equals(move, JsonValue(-130.52));
	assert_not_equals(move, JsonValue("hello"));
	assert_not_equals(move, JsonValue(false));
	assert_not_equals(move, JsonValue(JsonValueType::Array));
	assert_not_equals(move, JsonValue(JsonValueType::Object));

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
	JsonValue init = true;
	assert_boolean(init, true);

	JsonValue copy(init);
	assert_boolean(copy, true);
	assert_equals(copy, init);
	copy = init;
	assert_boolean(copy, true);
	assert_equals(copy, init);

	JsonValue move(std::move(init));
	assert_boolean(move, true);
	assert_equals(move, copy);
	JsonValue clone = copy;
	move = std::move(copy);
	assert_boolean(move, true);
	assert_equals(move, clone);

	assert_boolean(JsonValue(JsonValueType::Boolean), false);
	assert_boolean(JsonValue(true), true);
	assert_boolean(JsonValue(false), false);

	assert_not_equals(move, JsonValue());
	assert_not_equals(move, JsonValue(130));
	assert_not_equals(move, JsonValue(-130.52));
	assert_not_equals(move, JsonValue("hello"));
	assert_not_equals(move, JsonValue(false));
	assert_not_equals(move, JsonValue(JsonValueType::Array));
	assert_not_equals(move, JsonValue(JsonValueType::Object));

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
	JsonValue init = "this is obj string";
	assert_string(init, 0, 0.0, true, "this is obj string");

	JsonValue copy(init);
	assert_string(copy, 0, 0.0, true, "this is obj string");
	assert_equals(copy, init);
	copy = init;
	assert_string(copy, 0, 0.0, true, "this is obj string");
	assert_equals(copy, init);

	JsonValue move(std::move(init));
	assert_string(move, 0, 0.0, true, "this is obj string");
	assert_equals(move, copy);
	JsonValue clone = copy;
	move = std::move(copy);
	assert_string(move, 0, 0.0, true, "this is obj string");
	assert_equals(move, clone);

	assert_string(JsonValue(JsonValueType::String), 0, 0.0, false, "");
	assert_string(JsonValue(""), 0, 0.0, false, "");
	assert_string(JsonValue("hello"), 0, 0.0, true, "hello");
	assert_string(JsonValue("1.2"), 1, 1.2, true, "1.2");
	assert_string(JsonValue("123"), 123, 123.0, true, "123");
	assert_string(JsonValue("0"), 0, 0.0, true, "0");
	assert_string(JsonValue("0000"), 0, 0.0, true, "0000");

	assert_not_equals(move, JsonValue());
	assert_not_equals(move, JsonValue(130));
	assert_not_equals(move, JsonValue(-130.52));
	assert_not_equals(move, JsonValue("hello"));
	assert_not_equals(move, JsonValue(false));
	assert_not_equals(move, JsonValue(JsonValueType::Array));
	assert_not_equals(move, JsonValue(JsonValueType::Object));

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
	{
		JsonValue empty = JsonArray{};
		assert_array(empty, JsonArray());
		assert(empty.length() == 0);
		assert(empty.isEmpty());

		JsonValue copy(empty);
		assert_array(copy, JsonArray());
		assert(copy.length() == 0);
		assert(copy.isEmpty());
		assert_equals(copy, empty);
		copy = empty;
		assert_array(copy, JsonArray());
		assert(copy.length() == 0);
		assert(copy.isEmpty());
		assert_equals(copy, empty);

		JsonValue move(std::move(empty));
		assert_array(move, JsonArray());
		assert(move.length() == 0);
		assert(move.isEmpty());
		assert_equals(move, copy);
		JsonValue clone = copy;
		move = std::move(copy);
		assert_array(move, JsonArray());
		assert(move.length() == 0);
		assert(move.isEmpty());
		assert_equals(move, clone);

		assert_not_equals(move, JsonValue());
		assert_not_equals(move, JsonValue(130));
		assert_not_equals(move, JsonValue(-130));
		assert_not_equals(move, JsonValue(-130.52));
		assert_not_equals(move, JsonValue("hello"));
		assert_not_equals(move, JsonValue(false));
		assert_equals(move, JsonValue(JsonValueType::Array));
		assert_not_equals(move, JsonValue(JsonArray{ 1, 2, 3 }));
		assert_not_equals(move, JsonValue(JsonValueType::Object));
	}

	{
		JsonValue init = JsonArray{ "hello", 1, true, 0.4 };
		assert_array(init, JsonArray({ "hello", 1, true, 0.4 }));
		assert(init.length() == 4);
		assert(!init.isEmpty());
		assert_string(init[0], 0, 0.0, true, "hello");
		assert_integer(init[1], 1);
		assert_boolean(init[2], true);
		assert_decimal(init[3], 0.4);

		JsonValue copy(init);
		assert_array(copy, JsonArray({ "hello", 1, true, 0.4 }));
		assert(copy.length() == 4);
		assert(!copy.isEmpty());
		assert_string(copy[0], 0, 0.0, true, "hello");
		assert_integer(copy[1], 1);
		assert_boolean(copy[2], true);
		assert_decimal(copy[3], 0.4);
		assert_equals(copy, init);
		copy = init;
		assert_array(copy, JsonArray({ "hello", 1, true, 0.4 }));
		assert(copy.length() == 4);
		assert(!copy.isEmpty());
		assert_string(copy[0], 0, 0.0, true, "hello");
		assert_integer(copy[1], 1);
		assert_boolean(copy[2], true);
		assert_decimal(copy[3], 0.4);
		assert_equals(copy, init);

		JsonValue move(std::move(init));
		assert_array(move, JsonArray({ "hello", 1, true, 0.4 }));
		assert(move.length() == 4);
		assert(!move.isEmpty());
		assert_string(move[0], 0, 0.0, true, "hello");
		assert_integer(move[1], 1);
		assert_boolean(move[2], true);
		assert_decimal(move[3], 0.4);
		assert_equals(move, copy);
		JsonValue clone = copy;
		move = std::move(copy);
		assert_array(move, JsonArray({ "hello", 1, true, 0.4 }));
		assert(move.length() == 4);
		assert(!move.isEmpty());
		assert_string(move[0], 0, 0.0, true, "hello");
		assert_integer(move[1], 1);
		assert_boolean(move[2], true);
		assert_decimal(move[3], 0.4);
		assert_equals(move, clone);

		assert_not_equals(move, JsonValue());
		assert_not_equals(move, JsonValue(130));
		assert_not_equals(move, JsonValue(-130));
		assert_not_equals(move, JsonValue(-130.52));
		assert_not_equals(move, JsonValue("hello"));
		assert_not_equals(move, JsonValue(false));
		assert_not_equals(move, JsonValue(JsonValueType::Array));
		assert_not_equals(move, JsonValue(JsonValueType::Object));
	}

	JsonValue array = JsonArray{ 1, 2.0f, "3", false, JsonValue() };
	assert_array(array, JsonArray({ 1, 2.0f, "3", false, JsonValue() }));
	assert(array.length() == 5);
	assert(!array.isEmpty());
	assert_integer(array[0], 1);
	assert_decimal(array[1], 2.0f);
	assert_string(array[2], 3, 3.0, true, "3");
	assert_boolean(array[3], false);
	assert_null(array[4]);

	std::vector<std::string> array_of_strings = { "this", "is", "obj", "sentence" };

	array = JsonValue::from(array_of_strings);

	for (size_t i = 0; i < array_of_strings.size(); ++i)
		assert(array[i] == array_of_strings[i] && "got unexpected element value");

	std::vector<int> array_of_ints = { 6, 6345, 26, 34, -1231 };

	array = JsonValue::from(array_of_ints);

	for (size_t i = 0; i < array_of_ints.size(); ++i)
		assert(array[i] == array_of_ints[i] && "got unexpected element value");

	assert_json("[]", JsonArray());
	assert_json("[1,2,3]", JsonArray({ 1, 2, 3 }));
	assert_json("[234, \"hello\", null, true, {}]", JsonArray({ 234, "hello", JsonValue(), true, JsonObject() }));
	assert_json("[[{}]]", JsonValue(JsonArray({ JsonValue(JsonArray({ JsonValue(JsonObject()) })) })) );
	assert_json("[[[]]]", JsonValue(JsonArray({ JsonValue(JsonArray({ JsonValue(JsonArray()) })) })) );
	assert_json("[[1,2,3], [2,3,4], {\"key\":true}]", JsonArray({ JsonArray({ 1, 2, 3 }), JsonArray({ 2, 3, 4 }), JsonObject({ { "key", true } }) }));

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

void test_object()
{
	{
		JsonValue empty = JsonObject();
		assert_object(empty, JsonObject());
		assert(empty.length() == 0);
		assert(empty.isEmpty());

		JsonValue copy(empty);
		assert_object(copy, JsonObject());
		assert(copy.length() == 0);
		assert(copy.isEmpty());
		assert_equals(copy, empty);
		copy = empty;
		assert_object(copy, JsonObject());
		assert(copy.length() == 0);
		assert(copy.isEmpty());
		assert_equals(copy, empty);

		JsonValue move(std::move(empty));
		assert_object(move, JsonObject());
		assert(move.length() == 0);
		assert(move.isEmpty());
		assert_equals(move, copy);
		JsonValue clone = copy;
		move = std::move(copy);
		assert_object(move, JsonObject());
		assert(move.length() == 0);
		assert(move.isEmpty());
		assert_equals(move, clone);

		assert_not_equals(move, JsonValue());
		assert_not_equals(move, JsonValue(130));
		assert_not_equals(move, JsonValue(-130));
		assert_not_equals(move, JsonValue(-130.52));
		assert_not_equals(move, JsonValue("hello"));
		assert_not_equals(move, JsonValue(false));
		assert_not_equals(move, JsonValue(JsonValueType::Array));
		assert_equals(move, JsonValue(JsonValueType::Object));
		assert_not_equals(move, JsonValue(JsonObject { { "key", "value" } }));
	}
	{
		auto object = JsonObject
		{
			{ "boolean", true },
			{ "integer", 1362 },
			{ "decimal", 235.125 },
			{ "string", "this is text" },
			{ "null", JsonValue() }
		};
		JsonValue init = object;
		assert_object(init, object);
		assert(init.length() == 5);
		assert(!init.isEmpty());
		assert_boolean(init["boolean"], true);
		assert_integer(init["integer"], 1362);
		assert_decimal(init["decimal"], 235.125);
		assert_string(init["string"], 0, 0.0, true, "this is text");
		assert_null(init["null"]);

		JsonValue copy(init);
		assert_object(copy, object);
		assert(copy.length() == 5);
		assert(!(copy.isEmpty()));
		assert_equals(copy, init);
		copy = init;
		assert_object(copy, object);
		assert(copy.length() == 5);
		assert(!(copy.isEmpty()));
		assert_equals(copy, init);

		JsonValue move(std::move(init));
		assert_object(move, object);
		assert(move.length() == 5);
		assert(!(move.isEmpty()));
		assert_equals(move, copy);
		JsonValue clone = copy;
		move = std::move(copy);
		assert_object(move, object);
		assert(move.length() == 5);
		assert(!(move.isEmpty()));
		assert_equals(move, clone);

		assert_not_equals(move, JsonValue());
		assert_not_equals(move, JsonValue(130));
		assert_not_equals(move, JsonValue(-130));
		assert_not_equals(move, JsonValue(-130.52));
		assert_not_equals(move, JsonValue("hello"));
		assert_not_equals(move, JsonValue(false));
		assert_not_equals(move, JsonValue(JsonValueType::Array));
		assert_not_equals(move, JsonValue(JsonValueType::Object));
	}

	std::unordered_map<std::string, std::string> object_of_strings = {
		{ "abc", "def" },
		{ "hij", "klm" },
		{ "nop", "qrs" },
		{ "tuv", "wxyz" }
	};

	JsonValue object = JsonValue::from(object_of_strings);

	for (const auto& pair : object_of_strings)
		assert(object[pair.first] == pair.second && "got unexpected object value");

	std::unordered_map<std::string, double> object_of_doubles = {
		{ "123", 123.0 },
		{ "194.65", 194.65 },
		{ "987.125", 987.125 }
	};

	object = JsonValue::from(object_of_doubles);

	for (const auto& pair : object_of_doubles)
		assert(object[pair.first] == pair.second && "did not get expected object value");

	assert_json("{}", JsonObject());
	assert_json("{\"key\":1}", JsonObject({ {"key", 1} }));
	assert_json("{\"abc\":[true, false]}", JsonObject({ { "abc", JsonArray({ true, false }) } }));
	assert_json("{\"key\":\"value\",\"number\":298}", JsonObject({ { "key", "value" }, { "number", 298 } }));
	assert_json("{\"table\":{\"subtable\":567}}", JsonObject({ { "table", JsonObject({ { "subtable", 567 } }) } }));

	assert_parse_throws("{");
	assert_parse_throws("}");
	assert_parse_throws("{a");
	assert_parse_throws("a}");
	assert_parse_throws("{a}");
	assert_parse_throws("{\"a\"");
	assert_parse_throws("{\"a\":");
	assert_parse_throws("{\"a\"}");
	assert_parse_throws("{\"a\":1");
	assert_parse_throws("{\"a\":}");
	assert_parse_throws("{\"a\":1,");
	assert_parse_throws("{\"a\":1,}");
	assert_parse_throws("{\"a\":1}asdf");
	assert_parse_throws("{\"a\":1}true");
	assert_parse_throws("{\"a\":1}{");
}

void test_parse()
{
	assert_parse_not_throws(colorsJson);
	auto colors_Value = JsonObject
	{
		{ "colors", JsonArray
			{
				JsonObject
				{
					{ "color", "black" },
					{ "category", "hue" },
					{ "type", "primary" },
					{ "code", JsonObject
						{
							{ "rgba", JsonArray { 255, 255, 255, 1 } },
							{ "hex", "#000" }
						}
					}
				},
				JsonObject
				{
					{ "color", "white" },
					{ "category", "value" },
					{ "code", JsonObject {
							{ "rgba", JsonArray { 0, 0, 0, 1 } },
							{ "hex", "#FFF" }
						}
					}
				},
				JsonObject {
					{ "color", "red" },
					{ "category", "hue" },
					{ "type", "primary" },
					{ "code", JsonObject {
							{ "rgba", JsonArray { 255, 0, 0, 1 } },
							{ "hex", "#FF0" }
						}
					}
				},
				JsonObject {
					{ "color", "blue" },
					{ "category", "hue" },
					{ "type", "primary" },
					{ "code", JsonObject {
							{ "rgba", JsonArray { 0, 0, 255, 1 } },
							{ "hex", "#00F" }
						}
					}
				},
				JsonObject {
					{ "color", "yellow" },
					{ "category", "hue" },
					{ "type", "primary" },
					{ "code", JsonObject {
							{ "rgba", JsonArray { 255, 255, 0, 1 } },
							{ "hex", "#FF0" }
						}
					}
				},
				JsonObject {
					{ "color", "green" },
					{ "category", "hue" },
					{ "type", "secondary" },
					{ "code", JsonObject {
							{ "rgba", JsonArray { 0, 255, 0, 1 } },
							{ "hex", "#0F0" }
						}
					}
				}
			}
		}
	};
	assert_json(colorsJson, colors_Value);

	assert_parse_not_throws(pokemonJson);
	auto pokemon_Value = JsonObject {
		{ "count", 1118 },
		{ "next", "https://pokeapi.co/api/v2/pokemon?offset=20&limit=20"},
		{ "previous", JsonValue() },
		{ "results", JsonArray {
				JsonObject {
					{ "name", "bulbasaur"},
					{ "url", "https://pokeapi.co/api/v2/pokemon/1/"}
				},
				JsonObject {
					{ "name", "ivysaur" },
					{ "url", "https://pokeapi.co/api/v2/pokemon/2/" }
				},
				JsonObject {
					{ "name", "venusaur" },
					{ "url", "https://pokeapi.co/api/v2/pokemon/3/" }
				},
				JsonObject {
					{ "name", "charmander" },
					{ "url", "https://pokeapi.co/api/v2/pokemon/4/" }
				},
				JsonObject {
					{ "name", "charmeleon" },
					{ "url", "https://pokeapi.co/api/v2/pokemon/5/" }
				},
				JsonObject {
					{ "name", "charizard" },
					{ "url", "https://pokeapi.co/api/v2/pokemon/6/" }
				},
				JsonObject {
					{ "name", "squirtle" },
					{ "url", "https://pokeapi.co/api/v2/pokemon/7/" }
				},
				JsonObject {
					{ "name", "wartortle" },
					{ "url", "https://pokeapi.co/api/v2/pokemon/8/" }
				},
				JsonObject {
					{ "name", "blastoise" },
					{ "url", "https://pokeapi.co/api/v2/pokemon/9/" }
				},
				JsonObject {
					{ "name", "caterpie" },
					{ "url", "https://pokeapi.co/api/v2/pokemon/10/" }
				},
				JsonObject {
					{ "name", "metapod" },
					{ "url", "https://pokeapi.co/api/v2/pokemon/11/" }
				},
				JsonObject {
					{ "name", "butterfree" },
					{ "url", "https://pokeapi.co/api/v2/pokemon/12/" }
				},
				JsonObject {
					{ "name", "weedle" },
					{ "url", "https://pokeapi.co/api/v2/pokemon/13/" }
				},
				JsonObject {
					{ "name", "kakuna" },
					{ "url", "https://pokeapi.co/api/v2/pokemon/14/" }
				},
				JsonObject {
					{ "name", "beedrill" },
					{ "url", "https://pokeapi.co/api/v2/pokemon/15/" }
				},
				JsonObject {
					{ "name", "pidgey" },
					{ "url", "https://pokeapi.co/api/v2/pokemon/16/" }
				},
				JsonObject {
					{ "name", "pidgeotto" },
					{ "url", "https://pokeapi.co/api/v2/pokemon/17/" }
				},
				JsonObject {
					{ "name", "pidgeot" },
					{ "url", "https://pokeapi.co/api/v2/pokemon/18/" }
				},
				JsonObject {
					{ "name", "rattata" },
					{ "url", "https://pokeapi.co/api/v2/pokemon/19/" }
				},
				JsonObject {
					{ "name", "raticate" },
					{ "url", "https://pokeapi.co/api/v2/pokemon/20/" }
				}
			}
		}
	};

	assert_json(pokemonJson, pokemon_Value);

	// testing serialization
	auto pokemon_expected = JsonValue(pokemon_Value);
	auto from_json = deserialize(pokemonJson);
	auto serialized_json = serialize(pokemon_expected);
	auto from_json_clone = deserialize(serialized_json);

	assert(from_json == pokemon_expected);
	assert(from_json_clone == from_json);
	assert(from_json_clone == pokemon_expected);

}

int main()
{
	test_null();
	test_integer();
	test_float();
	test_boolean();
	test_string();
	test_array();
	test_object();
	test_parse();

	return 0;
}
