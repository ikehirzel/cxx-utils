#include "hirzel/json.hpp"
#include <hirzel/json/JsonValue.hpp>
#include <cassert>
#include <cstdlib>

namespace hirzel::json
{
	JsonValue::JsonValue() :
		_type(JsonValueType::Null),
		_integer(0)
	{}

	JsonValue::JsonValue(JsonValueType type) :
		_type(type),
		_integer(0)
	{
		switch (type)
		{
			case JsonValueType::Decimal:
				_decimal = 0.0;
				break;

			case JsonValueType::Boolean:
				_boolean = false;
				break;

			case JsonValueType::String:
				_string = new std::string();
				break;

			case JsonValueType::Array:
				_array = new JsonArray();
				break;

			case JsonValueType::Object:
				_object = new JsonObject();
				break;

			default:
				break;
		}
	}

	JsonValue::JsonValue(short i) :
		_type(JsonValueType::Integer),
		_integer(i)
	{}

	JsonValue::JsonValue(int i) :
		_type(JsonValueType::Integer),
		_integer(i)
	{}

	JsonValue::JsonValue(long i) :
		_type(JsonValueType::Integer),
		_integer(i)
	{}

	JsonValue::JsonValue(long long i) :
		_type(JsonValueType::Integer),
		_integer(i)
	{}

	JsonValue::JsonValue(unsigned short i) :
		_type(JsonValueType::Integer),
		_integer(i)
	{}

	JsonValue::JsonValue(unsigned int i) :
		_type(JsonValueType::Integer),
		_integer(i)
	{}

	JsonValue::JsonValue(unsigned long i) :
		_type(JsonValueType::Integer),
		_integer(i)
	{}

	JsonValue::JsonValue(unsigned long long i) :
		_type(JsonValueType::Integer),
		_integer(i)
	{}

	JsonValue::JsonValue(float d) :
		_type(JsonValueType::Decimal),
		_decimal(d)
	{}

	JsonValue::JsonValue(double d) :
		_type(JsonValueType::Decimal),
		_decimal(d)
	{}

	JsonValue::JsonValue(bool b) :
		_type(JsonValueType::Boolean),
		_boolean(b)
	{}

	JsonValue::JsonValue(std::string&& s) :
		_type(JsonValueType::String),
		_string(new std::string(std::move(s)))
	{}

	JsonValue::JsonValue(const std::string& s) :
		_type(JsonValueType::String),
		_string(new std::string(s))
	{}

	JsonValue::JsonValue(char* s) :
		_type(JsonValueType::String),
		_string(new std::string(s))
	{}

	JsonValue::JsonValue(const char* s) :
		_type(JsonValueType::String),
		_string(new std::string(s))
	{}

	JsonValue::JsonValue(JsonArray&& array) :
		_type(JsonValueType::Array),
		_array(new JsonArray(std::move(array)))
	{}

	JsonValue::JsonValue(const JsonArray& array) :
		_type(JsonValueType::Array),
		_array(new JsonArray(array))
	{}

	JsonValue::JsonValue(JsonObject&& object) :
		_type(JsonValueType::Object),
		_object(new JsonObject(std::move(object)))
	{}

	JsonValue::JsonValue(const JsonObject& object) :
		_type(JsonValueType::Object),
		_object(new JsonObject(object))
	{}

	JsonValue::JsonValue(JsonValue&& other) noexcept :
		_type(other._type),
		_integer(0)
	{
		switch (_type)
		{
		case JsonValueType::Null:
			break;

		case JsonValueType::Integer:
			_integer = other._integer;
			break;

		case JsonValueType::Decimal:
			_decimal = other._decimal;
			break;

		case JsonValueType::Boolean:
			_boolean = other._boolean;
			break;

		case JsonValueType::String:
			_string = other._string;
			other._string = nullptr;
			break;

		case JsonValueType::Array:
			_array = other._array;
			other._array = nullptr;
			break;

		case JsonValueType::Object:
			_object = other._object;
			other._object = nullptr;
			break;
		}

		other._type = JsonValueType::Null;
	}

	JsonValue::JsonValue(const JsonValue& other) :
		_type(other._type),
		_integer(0)
	{
		switch (_type)
		{
		case JsonValueType::Null:
			_integer = 0;
			break;

		case JsonValueType::Integer:
			_integer = other._integer;
			break;

		case JsonValueType::Decimal:
			_decimal = other._decimal;
			break;

		case JsonValueType::Boolean:
			_boolean = other._boolean;
			break;

		case JsonValueType::String:
			_string = new std::string(*other._string);
			break;

		case JsonValueType::Array:
			_array = new JsonArray(*other._array);
			break;

		case JsonValueType::Object:
			_object = new JsonObject(*other._object);
			break;
		}
	}

	JsonValue::~JsonValue()
	{
		switch (_type)
		{
		case JsonValueType::String:
			delete _string;
			break;

		case JsonValueType::Array:
			delete _array;
			break;

		case JsonValueType::Object:
			delete _object;
			break;

		default:
			break;
		}
	}

	JsonValue& JsonValue::operator=(JsonValue&& other)
	{
		new (this) auto(std::move(other));

		return *this;
	}

	JsonValue& JsonValue::operator=(const JsonValue& other)
	{
		new (this) auto(other);

		return *this;
	}

	const char* JsonValue::typeName() const noexcept
	{
		switch (_type)
		{
		case JsonValueType::Null:
			return "null";

		case JsonValueType::Integer:
			return "integer";

		case JsonValueType::Decimal:
			return "float";

		case JsonValueType::Boolean:
			return "boolean";

		case JsonValueType::String:
			return "string";

		case JsonValueType::Array:
			return "array";

		case JsonValueType::Object:
			return "table";

		default:
			return "invalid-type";
		}
	}

	JsonValue& JsonValue::at(size_t i)
	{
		assert(_type == JsonValueType::Array);
		assert(i < _array->size());

		return (*_array)[i];
	}

	const JsonValue& JsonValue::at(size_t i) const
	{
		assert(_type == JsonValueType::Array);
		assert(i < _array->size());

		return (*_array)[i];
	}

	JsonValue& JsonValue::at(const std::string& key)
	{
		assert(_type == JsonValueType::Object);

		auto iter = _object->find(key);

		assert(iter != _object->end());

		return iter->second;
	}

	const JsonValue& JsonValue::at(const std::string& key) const
	{
		assert(_type == JsonValueType::Object);

		auto iter = _object->find(key);

		assert(iter != _object->end());

		return iter->second;
	}

	int64_t JsonValue::asInteger() const
	{
		switch (_type)
		{
		case JsonValueType::Integer:
			return _integer;
		case JsonValueType::Boolean:
			return (long long)_boolean;
		case JsonValueType::Decimal:
			return (long long)_decimal;
		case JsonValueType::String:
			try
			{
				return std::stoll(*_string);
			}
			catch (const std::exception&)
			{
				return 0;
			}
		default:
			return 0;
		}
	}

	double JsonValue::asDecimal() const
	{
		switch (_type)
		{
		case JsonValueType::Integer:
			return (double)_integer;
		case JsonValueType::Boolean:
			return (double)_boolean;
		case JsonValueType::Decimal:
			return _decimal;
		case JsonValueType::String:
			try
			{
				return std::stod(*_string);
			}
			catch (const std::exception&)
			{
				return 0.0;
			}
		default:
			return 0.0;
		}
	}

	bool JsonValue::asBoolean() const
	{
		switch (_type)
		{
		case JsonValueType::Integer:
			return (bool)_integer;

		case JsonValueType::Boolean:
			return _boolean;

		case JsonValueType::Decimal:
			return (bool)_decimal;

		case JsonValueType::String:
			return !_string->empty();

		default:
			return false;
		}
	}

	std::string JsonValue::asString() const
	{
		if (_type == JsonValueType::String)
			return *_string;

		return serialize(*this, false);
	}

	bool JsonValue::isEmpty() const
	{
		switch (_type)
		{
		case JsonValueType::String:
			return _string->empty();

		case JsonValueType::Array:
			return _array->empty();

		case JsonValueType::Object:
			return _object->empty();

		case JsonValueType::Null:
			return true;

		default:
			return false;
		}
	}

	size_t JsonValue::length() const
	{
		switch (_type)
		{
		case JsonValueType::String:
			return _string->length();

		case JsonValueType::Array:
			return _array->size();

		case JsonValueType::Object:
			return _object->size();

		default:
			return 0;
		}
	}

	bool JsonValue::operator==(const JsonValue& other) const
	{

		if (_type != other.type())
			return false;

		switch (_type)
		{
		case JsonValueType::Null:
			return true;

		case JsonValueType::Integer:
			return _integer == other.integer();

		case JsonValueType::Decimal:
			return _decimal == other.decimal();

		case JsonValueType::Boolean:
			return _boolean == other.boolean();

		case JsonValueType::String:
			return *_string == other.string();

		case JsonValueType::Array:
		{
			const auto& arr = *_array;
			const auto& oarr = other.array();

			if (arr.size() != oarr.size())
				return false;

			for (size_t i = 0; i < arr.size(); ++i)
			{
				if (arr[i] != oarr[i])
					return false;
			}

			return true;
		}

		case JsonValueType::Object:
		{
			const auto& aTable = *_object;
			const auto& bTable = other.object();

			if (aTable.size() != bTable.size())
				return false;

			for (auto& aPair : aTable)
			{
				auto iter = bTable.find(aPair.first);

				if (iter == bTable.end())
					return false;

				auto& bPair = *iter;

				if (aPair.second != bPair.second)
					return false;
			}

			return true;
		}
		}

		return false;
	}

	std::ostream& operator<<(std::ostream& out, const JsonValue& json)
	{
		serialize(out, json, false);

		return out;
	}
}
