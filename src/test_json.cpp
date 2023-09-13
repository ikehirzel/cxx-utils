#include <hirzel/json.hpp>
#include <cassert>

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

void assert_cast_values(const Value& obj, int64_t intValue, double floatValue, bool boolValue, const std::string&)
{
	assert(obj.asInteger() == intValue);
	assert(obj.asDecimal() == floatValue);
	assert(obj.asBoolean() == boolValue);
	// TODO: Trim zeroes and spaces to make this comparison work
	//assert(obj.asString() == stringValue);
}

void assert_primitive(const Value& obj, ValueType type)
{
	// TODO: Check value?
	assert(obj.type() == type);
	//assert(obj.is##funcname());
	assert(obj.length() == 0);
}

void assert_not_primitive(const Value& obj, ValueType type)
{
	assert(obj.type() != type);
	//assert(!obj.is##funcname())
}

void assert_not_null(const Value& obj)
{
	assert(obj.type() != ValueType::Null);
	assert(!obj.isNull());
}

void assert_not_bool(const Value& obj)
{
	assert(obj.type() != ValueType::Boolean);
	assert(!obj.isBoolean());
}

void assert_not_number(const Value& obj)
{
	assert(obj.type() != ValueType::Number);
	assert(!obj.isDecimal());
}

void assert_not_string(const Value& obj)
{
	assert(obj.type() != ValueType::String);
	assert(!obj.isString());
}

void assert_not_array(const Value& obj)
{
	assert(obj.type() != ValueType::Array);
	assert(!obj.isArray());
}

void assert_not_object(const Value& obj)
{
	assert(obj.type() != ValueType::Object);
	assert(!obj.isObject());
}

void assert_equals(const Value& a, const Value& b)
{
	assert(a == b);
	assert(b == a);
	assert(!(a != b));
	assert(!(b != a));
}

void assert_not_equals(const Value& a, const Value& b)
{
	assert(!(a == b));
	assert(!(b == a));
	assert(a != b);
	assert(b != a);
}

void assert_null(const Value& obj)
{
	assert(obj.type() == ValueType::Null);
	assert(obj.isNull());
	assert(obj.length() == 0);
	assert_cast_values(obj, 0, 0.0, false, "null");
	assert_not_number(obj);
	assert_not_bool(obj);
	assert_not_string(obj);
	assert_not_array(obj);
	assert_not_object(obj);
	assert_equals(obj, Value());
	assert_equals(obj, Value(ValueType::Null));
}

template <typename V>
void assert_boolean(const Value& obj, const V& value)
{
	assert_primitive(obj, ValueType::Boolean);
	assert_cast_values(obj, (int)value, (double)value, value, (value ? "true" : "false"));
	assert_not_null(obj);
	assert_not_number(obj);
	assert_not_string(obj);
	assert_not_array(obj);
	assert_not_object(obj);
	assert_equals(obj, Value(value));
}

void assert_number(const Value& obj, double value)
{
	assert_primitive(obj, ValueType::Number);
	assert(obj.number() == value);
	assert_cast_values(obj, (int)value, (double)value, (bool)value, std::to_string(value));
	assert(obj.isNumber());
	assert_not_null(obj);
	assert_not_bool(obj);
	assert_not_string(obj);
	assert_not_array(obj);
	assert_not_object(obj);
	assert_equals(obj, Value(value));
}

void assert_string(const Value& obj, int64_t intValue, double floatValue, bool boolValue, const char* stringValue)
{
	assert(obj.type() == ValueType::String);
	assert(std::string(obj.typeName()) == "string");
	assert(obj.isString());
	assert(obj.length() == obj.asString().length());
	assert_cast_values(obj, intValue, floatValue, boolValue, stringValue);
	assert_not_null(obj);
	assert_not_number(obj);
	assert_not_bool(obj);
	assert_not_array(obj);
	assert_not_object(obj);
	assert_equals(obj, Value(stringValue));
}

void assert_array(const Value& obj, const Array& value)
{
	assert(obj.type() == ValueType::Array);
	assert(obj.isArray());
	assert_not_null(obj);
	assert_not_number(obj);
	assert_not_bool(obj);
	assert_not_string(obj);
	assert_not_object(obj);
	assert_equals(obj, Value(value));
}

void assert_object(const Value& obj, const Object& value)
{
	assert(obj.type() == ValueType::Object);
	assert(obj.isObject());
	assert_not_null(obj);
	assert_not_bool(obj);
	assert_not_number(obj);
	assert_not_string(obj);
	assert_not_array(obj);
	assert_equals(obj, Value(value));
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
	auto expected = Value(value);

	assert(jsonValue == expected);
}

void test_null()
{
	auto init = Value();
	assert_null(init);

	auto copy = Value(init);
	assert_null(copy);
	assert_equals(copy, init);
	copy = init;
	assert_null(copy);
	assert_equals(copy, init);

	Value move(std::move(init));
	assert_null(move);
	assert_equals(move, copy);
	Value clone = copy;
	move = std::move(copy);
	assert_null(move);
	assert_equals(move, clone);

	assert_null(Value());
	assert_null(Value(ValueType::Null));

	assert_not_equals(move, Value(143));
	assert_not_equals(move, Value(234.82));
	assert_not_equals(move, Value("hello"));
	assert_not_equals(move, Value(true));
	assert_not_equals(move, Value(Array()));
	assert_not_equals(move, Value(Object()));

	// valid
	assert_parse_not_throws("null");

	auto null = deserialize("null");
	assert(null.isNull());
	assert(null == Value());

	// invalid
	assert_parse_throws("n");
	assert_parse_throws("nub");
	assert_parse_throws("nul");
	assert_parse_throws("nullb");
	assert_parse_throws("nulll");
	assert_parse_throws("nll");
}

void test_number()
{
	Value init = 234;
	assert_number(init, 234.0);

	Value copy(init);
	assert_number(copy, 234.0);
	assert_equals(copy, init);
	copy = init;
	assert_number(copy, 234.0);
	assert_equals(copy, init);

	Value move(std::move(init));
	assert_number(move, 234.0);
	assert_equals(move, copy);
	Value clone = copy;
	move = std::move(copy);
	assert_number(move, 234);
	assert_equals(move, clone);

	assert_number(Value(0), 0);
	assert_number(Value(-0), -0);
	assert_number(Value(1), 1);
	assert_number(Value(-1), -1);
	assert_number(Value(4L), 4L);
	assert_number(Value(4U), 4U);
	assert_number(Value(-1023), -1023);
	assert_number(Value(62), 62);

	assert_not_equals(move, Value());
	assert_not_equals(move, Value(130));
	assert_not_equals(move, Value(-130));
	assert_not_equals(move, Value(-130.52));
	assert_not_equals(move, Value("hello"));
	assert_not_equals(move, Value(false));
	assert_not_equals(move, Value(ValueType::Array));
	assert_not_equals(move, Value(ValueType::Object));

	init = 2.34;
	assert_number(init, 2.34);

	new(&copy) auto(init);
	assert_number(copy, 2.34);
	assert_equals(copy, init);
	copy = init;
	assert_number(copy, 2.34);
	assert_equals(copy, init);

	new (&move) auto(std::move(init));
	assert_number(move, 2.34);
	assert_equals(move, copy);
	clone = copy;
	move = std::move(copy);
	assert_number(move, 2.34);
	assert_equals(move, clone);

	assert_number(Value(ValueType::Number), 0.0);
	assert_number(Value(1.34), 1.34);
	assert_number(Value(0.0f), 0.0f);
	assert_number(Value(-0.0), -0.0);
	assert_number(Value(-123592.235), -123592.235);
	assert_number(Value(2.0f), 2.0f);

	assert_not_equals(move, Value());
	assert_not_equals(move, Value(130));
	assert_not_equals(move, Value(-130.52));
	assert_not_equals(move, Value("hello"));
	assert_not_equals(move, Value(false));
	assert_not_equals(move, Value(ValueType::Array));
	assert_not_equals(move, Value(ValueType::Object));

	// valid
	assert_json("1.234", 1.234);
	assert_json("-11.234", -11.234);
	assert_json("185.22334", 185.22334);
	assert_json("-1.2e1", -1.2e1);
	assert_json("1.2e3", 1.2e3);
	assert_json("23.1e12", 23.1e12);
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
	assert_parse_throws(".234");
	assert_parse_throws("1.");
	assert_parse_throws("1.2.3");
	assert_parse_throws(".123e1");
	assert_parse_throws("23.e1");
	assert_parse_throws("23.1e1.2");
}

void test_boolean()
{
	Value init = true;
	assert_boolean(init, true);

	Value copy(init);
	assert_boolean(copy, true);
	assert_equals(copy, init);
	copy = init;
	assert_boolean(copy, true);
	assert_equals(copy, init);

	Value move(std::move(init));
	assert_boolean(move, true);
	assert_equals(move, copy);
	Value clone = copy;
	move = std::move(copy);
	assert_boolean(move, true);
	assert_equals(move, clone);

	assert_boolean(Value(ValueType::Boolean), false);
	assert_boolean(Value(true), true);
	assert_boolean(Value(false), false);

	assert_not_equals(move, Value());
	assert_not_equals(move, Value(130));
	assert_not_equals(move, Value(-130.52));
	assert_not_equals(move, Value("hello"));
	assert_not_equals(move, Value(false));
	assert_not_equals(move, Value(ValueType::Array));
	assert_not_equals(move, Value(ValueType::Object));

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
	Value init = "this is obj string";
	assert_string(init, 0, 0.0, true, "this is obj string");

	Value copy(init);
	assert_string(copy, 0, 0.0, true, "this is obj string");
	assert_equals(copy, init);
	copy = init;
	assert_string(copy, 0, 0.0, true, "this is obj string");
	assert_equals(copy, init);

	Value move(std::move(init));
	assert_string(move, 0, 0.0, true, "this is obj string");
	assert_equals(move, copy);
	Value clone = copy;
	move = std::move(copy);
	assert_string(move, 0, 0.0, true, "this is obj string");
	assert_equals(move, clone);

	assert_string(Value(ValueType::String), 0, 0.0, false, "");
	assert_string(Value(""), 0, 0.0, false, "");
	assert_string(Value("hello"), 0, 0.0, true, "hello");
	assert_string(Value("1.2"), 1, 1.2, true, "1.2");
	assert_string(Value("123"), 123, 123.0, true, "123");
	assert_string(Value("0"), 0, 0.0, true, "0");
	assert_string(Value("0000"), 0, 0.0, true, "0000");

	assert_not_equals(move, Value());
	assert_not_equals(move, Value(130));
	assert_not_equals(move, Value(-130.52));
	assert_not_equals(move, Value("hello"));
	assert_not_equals(move, Value(false));
	assert_not_equals(move, Value(ValueType::Array));
	assert_not_equals(move, Value(ValueType::Object));

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
		Value empty = Array{};
		assert_array(empty, Array());
		assert(empty.length() == 0);
		assert(empty.isEmpty());

		Value copy(empty);
		assert_array(copy, Array());
		assert(copy.length() == 0);
		assert(copy.isEmpty());
		assert_equals(copy, empty);
		copy = empty;
		assert_array(copy, Array());
		assert(copy.length() == 0);
		assert(copy.isEmpty());
		assert_equals(copy, empty);

		Value move(std::move(empty));
		assert_array(move, Array());
		assert(move.length() == 0);
		assert(move.isEmpty());
		assert_equals(move, copy);
		Value clone = copy;
		move = std::move(copy);
		assert_array(move, Array());
		assert(move.length() == 0);
		assert(move.isEmpty());
		assert_equals(move, clone);

		assert_not_equals(move, Value());
		assert_not_equals(move, Value(130));
		assert_not_equals(move, Value(-130));
		assert_not_equals(move, Value(-130.52));
		assert_not_equals(move, Value("hello"));
		assert_not_equals(move, Value(false));
		assert_equals(move, Value(ValueType::Array));
		assert_not_equals(move, Value(Array{ 1, 2, 3 }));
		assert_not_equals(move, Value(ValueType::Object));
	}

	{
		Value init = Array{ "hello", 1, true, 0.4 };
		assert_array(init, Array({ "hello", 1, true, 0.4 }));
		assert(init.length() == 4);
		assert(!init.isEmpty());
		assert_string(init[0], 0, 0.0, true, "hello");
		assert_boolean(init[2], true);
		assert_number(init[3], 0.4);

		Value copy(init);
		assert_array(copy, Array({ "hello", 1, true, 0.4 }));
		assert(copy.length() == 4);
		assert(!copy.isEmpty());
		assert_string(copy[0], 0, 0.0, true, "hello");
		assert_number(copy[1], 1);
		assert_boolean(copy[2], true);
		assert_number(copy[3], 0.4);
		assert_equals(copy, init);
		copy = init;
		assert_array(copy, Array({ "hello", 1, true, 0.4 }));
		assert(copy.length() == 4);
		assert(!copy.isEmpty());
		assert_string(copy[0], 0, 0.0, true, "hello");
		assert_number(copy[1], 1);
		assert_boolean(copy[2], true);
		assert_number(copy[3], 0.4);
		assert_equals(copy, init);

		Value move(std::move(init));
		assert_array(move, Array({ "hello", 1, true, 0.4 }));
		assert(move.length() == 4);
		assert(!move.isEmpty());
		assert_string(move[0], 0, 0.0, true, "hello");
		assert_number(move[1], 1);
		assert_boolean(move[2], true);
		assert_number(move[3], 0.4);
		assert_equals(move, copy);
		Value clone = copy;
		move = std::move(copy);
		assert_array(move, Array({ "hello", 1, true, 0.4 }));
		assert(move.length() == 4);
		assert(!move.isEmpty());
		assert_string(move[0], 0, 0.0, true, "hello");
		assert_number(move[1], 1);
		assert_boolean(move[2], true);
		assert_number(move[3], 0.4);
		assert_equals(move, clone);

		assert_not_equals(move, Value());
		assert_not_equals(move, Value(130));
		assert_not_equals(move, Value(-130));
		assert_not_equals(move, Value(-130.52));
		assert_not_equals(move, Value("hello"));
		assert_not_equals(move, Value(false));
		assert_not_equals(move, Value(ValueType::Array));
		assert_not_equals(move, Value(ValueType::Object));
	}

	Value array = Array{ 1, 2.0f, "3", false, Value() };
	assert_array(array, Array({ 1, 2.0f, "3", false, Value() }));
	assert(array.length() == 5);
	assert(!array.isEmpty());
	assert_number(array[0], 1);
	assert_number(array[1], 2.0f);
	assert_string(array[2], 3, 3.0, true, "3");
	assert_boolean(array[3], false);
	assert_null(array[4]);

	std::vector<std::string> array_of_strings = { "this", "is", "obj", "sentence" };

	array = Value::from(array_of_strings);

	for (size_t i = 0; i < array_of_strings.size(); ++i)
		assert(array[i] == array_of_strings[i] && "got unexpected element value");

	std::vector<int> array_of_ints = { 6, 6345, 26, 34, -1231 };

	array = Value::from(array_of_ints);

	for (size_t i = 0; i < array_of_ints.size(); ++i)
		assert(array[i] == array_of_ints[i] && "got unexpected element value");

	assert_json("[]", Array());
	assert_json("[1,2,3]", Array({ 1, 2, 3 }));
	assert_json("[234, \"hello\", null, true, {}]", Array({ 234, "hello", Value(), true, Object() }));
	assert_json("[[{}]]", Value(Array({ Value(Array({ Value(Object()) })) })) );
	assert_json("[[[]]]", Value(Array({ Value(Array({ Value(Array()) })) })) );
	assert_json("[[1,2,3], [2,3,4], {\"key\":true}]", Array({ Array({ 1, 2, 3 }), Array({ 2, 3, 4 }), Object({ { "key", true } }) }));

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
		Value empty = Object();
		assert_object(empty, Object());
		assert(empty.length() == 0);
		assert(empty.isEmpty());

		Value copy(empty);
		assert_object(copy, Object());
		assert(copy.length() == 0);
		assert(copy.isEmpty());
		assert_equals(copy, empty);
		copy = empty;
		assert_object(copy, Object());
		assert(copy.length() == 0);
		assert(copy.isEmpty());
		assert_equals(copy, empty);

		Value move(std::move(empty));
		assert_object(move, Object());
		assert(move.length() == 0);
		assert(move.isEmpty());
		assert_equals(move, copy);
		Value clone = copy;
		move = std::move(copy);
		assert_object(move, Object());
		assert(move.length() == 0);
		assert(move.isEmpty());
		assert_equals(move, clone);

		assert_not_equals(move, Value());
		assert_not_equals(move, Value(130));
		assert_not_equals(move, Value(-130));
		assert_not_equals(move, Value(-130.52));
		assert_not_equals(move, Value("hello"));
		assert_not_equals(move, Value(false));
		assert_not_equals(move, Value(ValueType::Array));
		assert_equals(move, Value(ValueType::Object));
		assert_not_equals(move, Value(Object { { "key", "value" } }));
	}
	{
		auto object = Object
		{
			{ "boolean", true },
			{ "integer", 1362 },
			{ "number", 235.125 },
			{ "string", "this is text" },
			{ "null", Value() }
		};
		Value init = object;
		assert_object(init, object);
		assert(init.length() == 5);
		assert(!init.isEmpty());
		assert_boolean(init["boolean"], true);
		assert_number(init["integer"], 1362);
		assert_number(init["number"], 235.125);
		assert_string(init["string"], 0, 0.0, true, "this is text");
		assert_null(init["null"]);

		Value copy(init);
		assert_object(copy, object);
		assert(copy.length() == 5);
		assert(!(copy.isEmpty()));
		assert_equals(copy, init);
		copy = init;
		assert_object(copy, object);
		assert(copy.length() == 5);
		assert(!(copy.isEmpty()));
		assert_equals(copy, init);

		Value move(std::move(init));
		assert_object(move, object);
		assert(move.length() == 5);
		assert(!(move.isEmpty()));
		assert_equals(move, copy);
		Value clone = copy;
		move = std::move(copy);
		assert_object(move, object);
		assert(move.length() == 5);
		assert(!(move.isEmpty()));
		assert_equals(move, clone);

		assert_not_equals(move, Value());
		assert_not_equals(move, Value(130));
		assert_not_equals(move, Value(-130));
		assert_not_equals(move, Value(-130.52));
		assert_not_equals(move, Value("hello"));
		assert_not_equals(move, Value(false));
		assert_not_equals(move, Value(ValueType::Array));
		assert_not_equals(move, Value(ValueType::Object));
	}

	std::unordered_map<std::string, std::string> object_of_strings = {
		{ "abc", "def" },
		{ "hij", "klm" },
		{ "nop", "qrs" },
		{ "tuv", "wxyz" }
	};

	Value object = Value::from(object_of_strings);

	for (const auto& pair : object_of_strings)
		assert(object[pair.first] == pair.second && "got unexpected object value");

	std::unordered_map<std::string, double> object_of_doubles = {
		{ "123", 123.0 },
		{ "194.65", 194.65 },
		{ "987.125", 987.125 }
	};

	object = Value::from(object_of_doubles);

	for (const auto& pair : object_of_doubles)
		assert(object[pair.first] == pair.second && "did not get expected object value");

	assert_json("{}", Object());
	assert_json("{\"key\":1}", Object({ {"key", 1} }));
	assert_json("{\"abc\":[true, false]}", Object({ { "abc", Array({ true, false }) } }));
	assert_json("{\"key\":\"value\",\"number\":298}", Object({ { "key", "value" }, { "number", 298 } }));
	assert_json("{\"table\":{\"subtable\":567}}", Object({ { "table", Object({ { "subtable", 567 } }) } }));

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
	auto colors_Value = Object
	{
		{ "colors", Array
			{
				Object
				{
					{ "color", "black" },
					{ "category", "hue" },
					{ "type", "primary" },
					{ "code", Object
						{
							{ "rgba", Array { 255, 255, 255, 1 } },
							{ "hex", "#000" }
						}
					}
				},
				Object
				{
					{ "color", "white" },
					{ "category", "value" },
					{ "code", Object {
							{ "rgba", Array { 0, 0, 0, 1 } },
							{ "hex", "#FFF" }
						}
					}
				},
				Object {
					{ "color", "red" },
					{ "category", "hue" },
					{ "type", "primary" },
					{ "code", Object {
							{ "rgba", Array { 255, 0, 0, 1 } },
							{ "hex", "#FF0" }
						}
					}
				},
				Object {
					{ "color", "blue" },
					{ "category", "hue" },
					{ "type", "primary" },
					{ "code", Object {
							{ "rgba", Array { 0, 0, 255, 1 } },
							{ "hex", "#00F" }
						}
					}
				},
				Object {
					{ "color", "yellow" },
					{ "category", "hue" },
					{ "type", "primary" },
					{ "code", Object {
							{ "rgba", Array { 255, 255, 0, 1 } },
							{ "hex", "#FF0" }
						}
					}
				},
				Object {
					{ "color", "green" },
					{ "category", "hue" },
					{ "type", "secondary" },
					{ "code", Object {
							{ "rgba", Array { 0, 255, 0, 1 } },
							{ "hex", "#0F0" }
						}
					}
				}
			}
		}
	};
	assert_json(colorsJson, colors_Value);

	assert_parse_not_throws(pokemonJson);
	auto pokemon_Value = Object {
		{ "count", 1118 },
		{ "next", "https://pokeapi.co/api/v2/pokemon?offset=20&limit=20"},
		{ "previous", Value() },
		{ "results", Array {
				Object {
					{ "name", "bulbasaur"},
					{ "url", "https://pokeapi.co/api/v2/pokemon/1/"}
				},
				Object {
					{ "name", "ivysaur" },
					{ "url", "https://pokeapi.co/api/v2/pokemon/2/" }
				},
				Object {
					{ "name", "venusaur" },
					{ "url", "https://pokeapi.co/api/v2/pokemon/3/" }
				},
				Object {
					{ "name", "charmander" },
					{ "url", "https://pokeapi.co/api/v2/pokemon/4/" }
				},
				Object {
					{ "name", "charmeleon" },
					{ "url", "https://pokeapi.co/api/v2/pokemon/5/" }
				},
				Object {
					{ "name", "charizard" },
					{ "url", "https://pokeapi.co/api/v2/pokemon/6/" }
				},
				Object {
					{ "name", "squirtle" },
					{ "url", "https://pokeapi.co/api/v2/pokemon/7/" }
				},
				Object {
					{ "name", "wartortle" },
					{ "url", "https://pokeapi.co/api/v2/pokemon/8/" }
				},
				Object {
					{ "name", "blastoise" },
					{ "url", "https://pokeapi.co/api/v2/pokemon/9/" }
				},
				Object {
					{ "name", "caterpie" },
					{ "url", "https://pokeapi.co/api/v2/pokemon/10/" }
				},
				Object {
					{ "name", "metapod" },
					{ "url", "https://pokeapi.co/api/v2/pokemon/11/" }
				},
				Object {
					{ "name", "butterfree" },
					{ "url", "https://pokeapi.co/api/v2/pokemon/12/" }
				},
				Object {
					{ "name", "weedle" },
					{ "url", "https://pokeapi.co/api/v2/pokemon/13/" }
				},
				Object {
					{ "name", "kakuna" },
					{ "url", "https://pokeapi.co/api/v2/pokemon/14/" }
				},
				Object {
					{ "name", "beedrill" },
					{ "url", "https://pokeapi.co/api/v2/pokemon/15/" }
				},
				Object {
					{ "name", "pidgey" },
					{ "url", "https://pokeapi.co/api/v2/pokemon/16/" }
				},
				Object {
					{ "name", "pidgeotto" },
					{ "url", "https://pokeapi.co/api/v2/pokemon/17/" }
				},
				Object {
					{ "name", "pidgeot" },
					{ "url", "https://pokeapi.co/api/v2/pokemon/18/" }
				},
				Object {
					{ "name", "rattata" },
					{ "url", "https://pokeapi.co/api/v2/pokemon/19/" }
				},
				Object {
					{ "name", "raticate" },
					{ "url", "https://pokeapi.co/api/v2/pokemon/20/" }
				}
			}
		}
	};

	assert_json(pokemonJson, pokemon_Value);

	// testing serialization
	auto pokemon_expected = Value(pokemon_Value);
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
	test_number();
	test_boolean();
	test_string();
	test_array();
	test_object();
	test_parse();

	return 0;
}
