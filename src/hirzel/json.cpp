#include <hirzel/json.hpp>
#include <hirzel/file.hpp>
#include <hirzel/print.hpp>
#include <utility>

namespace hirzel::json
{
	static int xIndex = std::ios::xalloc();

	JsonValue::JsonValue():
	_type(ValueType::Null),
	_integer(0)
	{}

	JsonValue::JsonValue(ValueType type) :
	_type(type),
	_integer(0)
	{
		switch (type)
		{
			case ValueType::Null:
			case ValueType::Integer:
				break;

			case ValueType::Float:
				_decimal = 0.0;
				break;

			case ValueType::Boolean:
				_boolean = false;
				break;

			case ValueType::String:
				_string = new std::string();
				break;

			case ValueType::Array:
				_array = new JsonArray();
				break;

			case ValueType::Object:
				_table = new JsonObject();
				break;
		}
	}

	JsonValue::JsonValue(short i):
	_type(ValueType::Integer),
	_integer(i)
	{}

	JsonValue::JsonValue(int i):
	_type(ValueType::Integer),
	_integer(i)
	{}

	JsonValue::JsonValue(long i):
	_type(ValueType::Integer),
	_integer(i)
	{}

	JsonValue::JsonValue(long long i):
	_type(ValueType::Integer),
	_integer(i)
	{}

	JsonValue::JsonValue(unsigned short i):
	_type(ValueType::Integer),
	_integer(i)
	{}

	JsonValue::JsonValue(unsigned int i):
	_type(ValueType::Integer),
	_integer(i)
	{}

	JsonValue::JsonValue(unsigned long i):
	_type(ValueType::Integer),
	_integer(i)
	{}

	JsonValue::JsonValue(unsigned long long i):
	_type(ValueType::Integer),
	_integer(i)
	{}

	JsonValue::JsonValue(float d):
	_type(ValueType::Float),
	_decimal(d)
	{}

	JsonValue::JsonValue(double d):
	_type(ValueType::Float),
	_decimal(d)
	{}

	JsonValue::JsonValue(bool b):
	_type(ValueType::Boolean),
	_boolean(b)
	{}

	JsonValue::JsonValue(const std::string& s):
	_type(ValueType::String),
	_string(new std::string(s))
	{}

	JsonValue::JsonValue(char* s):
	_type(ValueType::String),
	_string(new std::string(s))
	{}

	JsonValue::JsonValue(const char* s):
	_type(ValueType::String),
	_string(new std::string(s))
	{}

	JsonValue::JsonValue(JsonArray&& array) :
	_type(ValueType::Array),
	_array(new JsonArray(std::move(array)))
	{}

	JsonValue::JsonValue(const JsonArray& array):
	_type(ValueType::Array),
	_array(new JsonArray(array))
	{}

	JsonValue::JsonValue(JsonObject&& table) :
	_type(ValueType::Object),
	_table(new JsonObject(std::move(table)))
	{}

	JsonValue::JsonValue(const JsonObject& table):
	_type(ValueType::Object),
	_table(new JsonObject(table))
	{}

	JsonValue::JsonValue(JsonValue&& other) noexcept:
	_type(other._type),
	_integer(0)
	{
		switch (_type)
		{
			case ValueType::Null:
				break;

			case ValueType::Integer:
				_integer = other._integer;
				break;

			case ValueType::Float:
				_decimal = other._decimal;
				break;

			case ValueType::Boolean:
				_boolean = other._boolean;
				break;

			case ValueType::String:
				_string = other._string;
				other._string = nullptr;
				break;

			case ValueType::Array:
				_array = other._array;
				other._array = nullptr;
				break;

			case ValueType::Object:
				_table = other._table;
				other._table = nullptr;
				break;
		}

		other._type = ValueType::Null;
	}

	JsonValue::JsonValue(const JsonValue& other):
	_type(other._type),
	_integer(0)
	{
		switch (_type)
		{
			case ValueType::Null:
				_integer = 0;
				break;

			case ValueType::Integer:
				_integer = other._integer;
				break;

			case ValueType::Float:
				_decimal = other._decimal;
				break;

			case ValueType::Boolean:
				_boolean = other._boolean;
				break;

			case ValueType::String:
				_string = new std::string(*other._string);
				break;

			case ValueType::Array:
				_array = new JsonArray(*other._array);
				break;

			case ValueType::Object:
				_table = new JsonObject(*other._table);
				break;
		}
	}

	JsonValue::~JsonValue()
	{
		switch (_type)
		{
			case ValueType::String:
				delete _string;
				break;

			case ValueType::Array:
				delete _array;
				break;

			case ValueType::Object:
				delete _table;
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
			case ValueType::Null:
				return "null";

			case ValueType::Integer:
				return "integer";

			case ValueType::Float:
				return "float";

			case ValueType::Boolean:
				return "boolean";

			case ValueType::String:
				return "string";

			case ValueType::Array:
				return "array";

			case ValueType::Object:
				return "table";

			default:
				return "invalid-type";
		}
	}

	JsonValue& JsonValue::at(usize i)
	{
		assert(_type == ValueType::Array);
		assert(i < _array->size());

		return (*_array)[i];
	}

	const JsonValue& JsonValue::at(usize i) const
	{
		assert(_type == ValueType::Array);
		assert(i < _array->size());

		return (*_array)[i];
	}

	JsonValue& JsonValue::at(const std::string& key)
	{
		assert(_type == ValueType::Object);

		auto iter = _table->find(key);

		assert(iter != _table->end());

		return iter->second;
	}

	const JsonValue& JsonValue::at(const std::string& key) const
	{
		assert(_type == ValueType::Object);

		auto iter = _table->find(key);

		assert(iter != _table->end());

		return iter->second;
	}

	JsonValue read(const std::string& filepath)
	{
		auto src = file::read(filepath);

		return parse(src);
	}

	std::string preprocess(const std::string& src)
	{
		const char* iter = src.c_str();
		std::string out(src.length(), '\0');
		usize oi = 0;

		while (*iter)
		{
			if (*iter <= ' ')
			{
				iter += 1;
				continue;
			}

			if (*iter == '\"')
			{
				const char* start_of_string = iter;

				out[oi++] = *iter;
				iter += 1;

				while (true)
				{
					if (*iter == '\0')
					{
						usize size_of_preview = (iter - start_of_string > 15 ? 15 : iter - start_of_string);
						throw std::runtime_error("unterminated string starting with: "
							+ std::string(start_of_string, size_of_preview));
					}

					if (*iter == '\"' && iter[-1] != '\\')
						break;

					out[oi++] = *iter;
					iter += 1;
				}
			}

			out[oi++] = *iter;
			iter += 1;
		}

		out.resize(oi);

		return out;
	}
	
	JsonValue parse(const std::string& src)
	{
		std::string preprocessedSrc = preprocess(src);

		if (preprocessedSrc.empty())
			throw std::runtime_error("Source string is empty.");

		const auto* iter = preprocessedSrc.c_str();
		auto out = parseValue(iter);

		if (*iter != '\0')
			throw std::runtime_error("Unexpected characters at end of JSON: '" + std::string(iter) + "'.");

		return out;
	}

	static bool isFloatLiteral(const char*& iter)
	{
		bool isFloat = false;
		bool expecting_digit = true;

		if (*iter == '-' || *iter == '+')
			iter += 1;

		while (true)
		{
			switch (*iter)
			{
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				expecting_digit = false;
				iter += 1;
				break;

			case '.':
				if (isFloat)
					throw std::runtime_error("extra '.' in number literal");

				iter += 1;
				isFloat = true;
				expecting_digit = true;
				break;

			default:
				if (expecting_digit)
					throw std::runtime_error("number literals must end in a digit");

				return isFloat;
			}
		}
	}

	JsonValue parseNumber(const char*& iter)
	{
		const char* const start_of_number = iter;

		try
		{
			bool isFloat = isFloatLiteral(iter);

			if (*iter == 'e' || *iter == 'E')
			{
				iter += 1;

				if (isFloatLiteral(iter))
					throw std::runtime_error("exponents must be integers");
			}

			std::string number_literal(start_of_number, iter - start_of_number);

			return isFloat
				? JsonValue(std::stod(number_literal))
				: JsonValue(std::stoll(number_literal));
		}
		catch (const std::exception& e)
		{
			throw std::runtime_error("failed to parse number '"
				+ std::string(start_of_number, iter - start_of_number)
				+ "': "
				+ std::string(e.what()));
		}
	}

	JsonValue parseString(const char*& iter)
	{
		iter += 1;

		const char* const start_of_string = iter;

		while (*iter != '\"')
		{
			if (*iter == '\0')
				throw std::runtime_error("unterminated string: "
					+ std::string(start_of_string, iter - start_of_string));

			iter += 1;
		}

		JsonValue out = std::string(start_of_string, iter - start_of_string);

		iter += 1;

		return out;
	}

	JsonValue parseNull(const char*& iter)
	{
		iter += 1;

		for (const char* match = "ull"; *match; ++match)
		{
			if (*iter != *match)
				throw std::runtime_error("unexpected character '"
					+ std::string(1, *iter)
					+ "' found in null literal");

			iter += 1;
		}

		return JsonValue();
	}

	JsonValue parseBool(const char*& iter, bool expected)
	{
		iter += 1;

		const char* match;

		if (expected)
			match = "rue";
		else
			match = "alse";

		while (*match)
		{
			if (*iter != *match)
				throw std::runtime_error("unexpected character '"
					+ std::string(1, *iter)
					+ "' found in bool literal");

			iter += 1;
			match += 1;
		}

		return JsonValue(expected);
	}

	JsonValue parseArray(const char*& iter)
	{
		auto arr = JsonArray();

		iter += 1;

		if (*iter == ']')
		{
			iter += 1;
			return arr;
		}

		while (true)
		{
			auto value = parseValue(iter);
			
			arr.emplace_back(std::move(value));

			if (*iter != ',')
				break;

			iter += 1;
		}

		if (*iter != ']')
			throw std::runtime_error("unterminated array");

		iter += 1;

		return arr;
	}

	JsonValue parseObject(const char*& iter)
	{
		auto obj = JsonObject();

		iter += 1;

		if (*iter == '}')
		{
			iter += 1;
			return obj;
		}

		while (true)
		{
			if (*iter != '\"')
				throw std::runtime_error("Invalid label.");

			iter += 1;

			const char* endOfLabel = iter;
			const char* startOfLabel = iter;

			while (*endOfLabel != '\"')
			{
				if (!endOfLabel[0])
					throw std::runtime_error("Unterminated label: '"
						+ std::string(startOfLabel, endOfLabel - startOfLabel)
						+ "'.");

				endOfLabel += 1;
			}

			std::string label(startOfLabel, endOfLabel - startOfLabel);
			iter = endOfLabel + 1;

			if (*iter != ':')
				throw std::runtime_error("Expected ':', got: "
					+ std::string(iter, 1));

			iter += 1;

			auto value = parseValue(iter);
			
			obj.emplace(label, std::move(value));

			if (*iter != ',')
			{
				if (*iter != '}')
					throw std::runtime_error("Unterminated object.");
				iter += 1;
				break;
			}

			iter += 1;
		}

		return obj;
	}

	JsonValue parseValue(const char*& iter)
	{
		switch (*iter)
		{
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case '-':
			return parseNumber(iter);

		case '\"':
			return parseString(iter);

		case '{':
			return parseObject(iter);

		case '[':
			return parseArray(iter);

		case 't':
			return parseBool(iter, true);

		case 'f':
			return parseBool(iter, false);

		case 'n':
			return parseNull(iter);

		case 0:
			throw std::runtime_error("Expected value but reached end of input.");

		default:
			throw std::runtime_error("Invalid value starting with '"
				+ std::string(1, *iter)
				+ "'");
		}
	}

	std::string serialize(const JsonValue& json)
	{
		return serializeValue(json);
	}

	std::string serializeArray(const JsonValue& json)
	{
		std::string out = "[";

		const auto& array = json.array();

		bool is_first_element = true;
		for (const auto& element : array)
		{
			if (!is_first_element)
				out += ',';

			out += serializeValue(element);
			is_first_element = false;
		}

		return out + "]";
	}

	std::string serializeObject(const JsonValue& json)
	{
		std::string out = "{";

		const auto& table = json.table();

		bool is_first_element = true;
		for (const auto& pair : table)
		{
			if (!is_first_element)
				out += ',';

			out += "\"" + pair.first + "\":" + serializeValue(pair.second);
			is_first_element = false;
		}

		return out + "}";
	}

	std::string serializeValue(const JsonValue& json)
	{
		switch (json.type())
		{
		case ValueType::Null:
			return "null";
		case ValueType::Integer:
			return std::to_string(json.integer());
		case ValueType::Float:
			return std::to_string(json.decimal());
		case ValueType::Boolean:
			return json.boolean() ? "true" : "false";
		case ValueType::String:
			return "\"" + json.string() + "\"";
		case ValueType::Array:
			return serializeArray(json);
		case ValueType::Object:
			return serializeObject(json);
		default:
			throw std::runtime_error("Invalid type!");
		}
	}

	int64_t JsonValue::asI64() const
	{
		switch (_type)
		{
		case ValueType::Integer:
			return _integer;
		case ValueType::Boolean:
			return (long long)_boolean;
		case ValueType::Float:
			return (long long)_decimal;
		case ValueType::String:
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

	double JsonValue::asF64() const
	{
		switch (_type)
		{
		case ValueType::Integer:
			return (double)_integer;
		case ValueType::Boolean:
			return (double)_boolean;
		case ValueType::Float:
			return _decimal;
		case ValueType::String:
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

	bool JsonValue::asBool() const
	{
		switch (_type)
		{
		case ValueType::Integer:
			return (bool)_integer;
		case ValueType::Boolean:
			return _boolean;
		case ValueType::Float:
			return (bool)_decimal;
		case ValueType::String:
			return !_string->empty();
		default:
			return false;
		}
	}

	

	std::string JsonValue::asString() const
	{
		if (_type == ValueType::String)
			return *_string;

		std::stringstream out;

		out << *this;

		return out.str();
	}

	bool JsonValue::isEmpty() const
	{
		switch (_type)
		{
			case ValueType::String:
				return _string->empty();

			case ValueType::Array:
				return _array->empty();

			case ValueType::Object:
				return _table->empty();

			case ValueType::Null:
				return true;

			default:
				return false;
			}
	}

	usize JsonValue::length() const
	{
		switch (_type)
		{
			case ValueType::String:
				return _string->length();

			case ValueType::Array:
				return _array->size();

			case ValueType::Object:
				return _table->size();

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
			case ValueType::Null:
				return true;

			case ValueType::Integer:
				return _integer == other.integer();

			case ValueType::Float:
				return _decimal == other.decimal();

			case ValueType::Boolean:
				return _boolean == other.boolean();

			case ValueType::String:
				return *_string == other.string();

			case ValueType::Array:
			{
				const auto& arr = *_array;
				const auto& oarr = other.array();

				if (arr.size() != oarr.size())
					return false;

				for (usize i = 0; i < arr.size(); ++i)
				{
					if (arr[i] != oarr[i])
						return false;
				}

				return true;
			}

			case ValueType::Object:
			{
				const auto& table = *_table;
				const auto& otable = other.table();

				if (table.size() != otable.size())
					return false;

				for (auto p : table)
				{
					auto iter = otable.find(p.first);
					if (iter == otable.end())
						return false;
					auto op = *iter;
					if (p.first != op.first || p.second != op.second)
						return false;
				}
				return true;
			}
		}

		return false;
	}

	static void indent(std::ostream& stream)
	{
		auto index = stream.iword(xIndex);

		for (usize i = 0; i < index; ++i)
			stream << "\t";
	}

	std::ostream& operator<<(std::ostream& out, const JsonValue& json)
	{
		switch (json._type)
		{
			case ValueType::Null:
				out << "null";
				break;

			case ValueType::Integer:
				out << json._integer;
				break;

			case ValueType::Boolean:
				out << json._boolean;
				break;

			case ValueType::Float:
				out << json._decimal;
				break;

			case ValueType::String:
				out << "\"" <<  *json._string << "\"";
				break;

			case ValueType::Array:
			{
				if (json._array->empty())
				{
					out << "[]";
					break;
				}

				auto isVertical = false;

				for (const auto& elem : *json._array)
				{
					if (elem.isObject() || elem.isArray())
					{
						isVertical = true;
						break;
					}
				}

				out << "[";

				auto isFirst = true;
				auto& depth = out.iword(xIndex);

				depth += 1;

				for (const auto& item : *json._array)
				{
					if (isFirst)
					{
						isFirst = false;

						if (isVertical)
						{
							indent(out);
							out << "\n";
						}
					}
					else
					{
						if (isVertical)
						{
							out << ",\n";
							indent(out);
						}
						else
						{
							out << ", ";
						}
					}

					out << item;
				}

				depth -= 1;

				if (isVertical)
					out << "\n";

				out << "]";

				return out;
			}

			case ValueType::Object:
			{
				if (json._table->empty())
				{
					out << "{}";
					break;
				}

				out << "{";
				
				auto& depth = out.iword(xIndex);

				depth += 1;

				auto isFirst = true;

				for (const auto& pair : *json._table)
				{
					if (isFirst)
					{
						isFirst = false;
					}
					else
					{
						out << ",";
					}

					out << "\n";

					indent(out);

					out << "\"" << pair.first << "\": " << pair.second;
				}

				depth -= 1;

				out << "\n}";

				return out;
			}

			default:
				break;
		}

		return out;
	}
}
