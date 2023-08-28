#ifndef HIRZEL_JSON_HPP
#define HIRZEL_JSON_HPP

/**
 * @file json.hpp
 * @brief Json de/serialization utilities for hirzel::JsonValue
 * @author Isaac Hirzel
 * 
 * Copyright 2023 Isaac Hirzel
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>
#include <cstdint>

namespace hirzel
{
	namespace json
	{
		class SyntaxError : public std::exception
		{
			std::string _msg;

		public:

			SyntaxError(const std::string& msg) : _msg(msg) {}
			const char *what() const noexcept { return _msg.c_str(); }
		};

		class TypeException : public std::exception
		{
			std::string _msg;

		public:

			TypeException(const std::string& msg) : _msg(msg) {}

			static TypeException cast_exception(const char *from, const char *to)
			{
				return TypeException("JsonValue of type '"
					+ std::string(from)
					+ "' cannot be cast as "
					+ std::string(to));
			}

			const char *what() const noexcept override
			{
				return _msg.c_str(); 
			}
		};

		enum class ValueType
		{
			Null,
			Integer,
			Float,
			Boolean,
			String,
			Array,
			Object
		};

		class JsonValue;

		using JsonObject = std::unordered_map<std::string, JsonValue>;
		using JsonArray = std::vector<JsonValue>;

		class JsonValue
		{
			ValueType _type = ValueType::Null;
			
			union
			{
				int64_t _integer = 0;
				bool _boolean;
				double _decimal;
				std::string *_string;
				JsonArray *_array;
				JsonObject *_table;
			};

		public:

			JsonValue() : _type(ValueType::Null), _integer(0) {}
			JsonValue(const JsonValue& other) { *this = other; }
			JsonValue(JsonValue&& other) { *this = other; }
			JsonValue(ValueType t);

			JsonValue(short i) : _type(ValueType::Integer), _integer(i) {}
			JsonValue(int i) : _type(ValueType::Integer), _integer(i) {}
			JsonValue(long i) : _type(ValueType::Integer), _integer(i) {}
			JsonValue(long long i) : _type(ValueType::Integer), _integer(i) {}

			JsonValue(unsigned short i) : _type(ValueType::Integer), _integer(i) {}
			JsonValue(unsigned int i) : _type(ValueType::Integer), _integer(i) {}
			JsonValue(unsigned long i) : _type(ValueType::Integer), _integer(i) {}
			JsonValue(unsigned long long i) : _type(ValueType::Integer), _integer(i) {}
			
			JsonValue(float d) : _type(ValueType::Float), _decimal(d) {}
			JsonValue(double d) : _type(ValueType::Float), _decimal(d) {}

			JsonValue(bool b) : _type(ValueType::Boolean), _boolean(b) {}

			JsonValue(const std::string& s) : _type(ValueType::String), _string(new std::string(s)) {}
			JsonValue(char* s) : _type(ValueType::String), _string(new std::string(s)) {}
			JsonValue(const char* s) : _type(ValueType::String), _string(new std::string(s)) {}

			JsonValue(const JsonArray& array) : _type(ValueType::Array), _array(new JsonArray(array)) {}
			JsonValue(const JsonObject& table) : _type(ValueType::Object), _table(new JsonObject(table)) {}

			template <typename T>
			static JsonValue from(const std::vector<T> array)
			{
				JsonArray out;

				out.resize(array.size());

				for (size_t i = 0; i < array.size(); ++i)
					out[i] = array[i];

				return out;
			}

			template <typename T>
			static JsonValue from(const std::unordered_map<std::string, T> table)
			{
				JsonObject out;

				for (const auto& pair : table)
					out[pair.first] = pair.second;

				return out;
			}

			~JsonValue();

			long long integer() const { return _integer; }
			double decimal() const { return _decimal; }
			bool boolean() const { return _boolean; }
			const std::string& string() const { return *_string; }
			const JsonArray& array() const { return *_array; }
			const JsonObject& table() const { return *_table; }

			int64_t asI64() const;
			int32_t asI32() const { return (int32_t)asI64(); }
			int16_t asI16() const { return (int16_t)asI64(); }
			int8_t asI8() const { return (int8_t)asI64(); }

			double asF64() const;
			float asF32() const { return (float)asF64(); }
			bool asBool() const;
			std::string	asString() const;

			bool toBool() const
			{
				if (_type != ValueType::Boolean)
					throw TypeException::cast_exception(typeName(), "boolean");

				return _boolean;
			}	

			long long toI64() const
			{
				if (_type != ValueType::Integer)
					throw TypeException::cast_exception(typeName(), "integer");
					
				return _integer;
			}

			double toF64() const 
			{
				if (_type != ValueType::Float)
					throw TypeException::cast_exception(typeName(), "decimal");
					
				return _decimal;
			}
			const std::string& toString() const

			{
				if (_type != ValueType::String)
					throw TypeException::cast_exception(typeName(), "string");

				return *_string;
			}

			const JsonArray& toArray() const
			{
				if (_type != ValueType::Array)
					throw TypeException::cast_exception(typeName(), "array");

				return *_array;
			}

			const JsonObject& toTable() const
			{
				if (_type != ValueType::Object)
					throw TypeException::cast_exception(typeName(), "rable");
				
				return *_table;
			}

			bool contains(const std::string& key) const
			{
				return _type == ValueType::Object ?
					_table->find(key) != _table->end() :
					false;
			}

			bool isEmpty() const;
			size_t length() const;

			bool isNull() const noexcept { return _type == ValueType::Null; }
			bool isInteger() const noexcept { return _type == ValueType::Integer; }
			bool isFloat() const noexcept { return _type == ValueType::Float; }
			bool isNumber() const noexcept { return _type == ValueType::Integer || _type == ValueType::Float; }
			bool isBoolean() const noexcept { return _type == ValueType::Boolean; }
			bool isString() const noexcept { return _type == ValueType::String; }
			bool isArray() const noexcept { return _type == ValueType::Array; }
			bool isTable() const noexcept { return _type == ValueType::Object; }

			ValueType type() const { return _type; }
			const char *typeName() const noexcept;

			JsonValue& operator=(const JsonValue& other);
			JsonValue& operator=(JsonValue&& other);

			JsonValue& at(size_t i)
			{
				if (_type != ValueType::Array)
					throw TypeException("JsonValue must be array for integer indexing");

				if (i >= _array->size())
				{
					_array->resize(i + 1);
				}

				return (*_array)[i];
			}

			const JsonValue& at(size_t i) const
			{
				if (_type != ValueType::Array)
					throw TypeException("JsonValue must be array for integer indexing");

				if (i >= _array->size())
					throw std::out_of_range("attempted to access index "
						+ std::to_string(i)
						+ " but size is "
						+ std::to_string(_array->size()));

				return (*_array)[i];
			}

			JsonValue& operator[](size_t i)
			{
				return at(i); 
			}

			const JsonValue& operator[](size_t i) const
			{
				return at(i); 
			}

			JsonValue& at(const std::string& key)
			{
				if (_type != ValueType::Object)
					throw TypeException("JsonValue must be a table for string indexing");
				
				return (*_table)[key];
			}

			const JsonValue& at(const std::string& key) const
			{
				if (_type != ValueType::Object)
					throw TypeException("JsonValue must be a table for string indexing");

				if (_table->find(key) == _table->end())
					throw std::out_of_range("attempted to access element at '"
						+ key + "' but it does not exist");

				return (*_table)[key];
			}

			JsonValue& operator[](const std::string& key)
			{
				return at(key); 
			}

			const JsonValue& operator[](const std::string& key) const
			{ 
				return at(key);
			}

			bool operator==(const JsonValue& other) const;
			bool operator!=(const JsonValue& other) const { return !(*this == other); }
		};

		std::string preprocess(const std::string& src);
		bool parseNumberLiteral(const char *& iter);
		JsonValue parseNumber(const char *& iter);
		JsonValue parseString(const char *& iter);
		JsonValue parseNull(const char *& iter);
		JsonValue parseBool(const char *& iter, bool expected);
		JsonValue parseArray(const char *& iter);
		JsonValue parseObject(const char *& iter);
		JsonValue parseValue(const char *& iter);
		std::string serialize_array(const JsonValue& JsonValue);
		std::string serialize_object(const JsonValue& JsonValue);
		std::string serialize_value(const JsonValue& JsonValue);
		JsonValue deserialize(const std::string& json);
		std::string serialize(const JsonValue& JsonValue)
		{
			return serialize_value(JsonValue);
		}
	}
}

#endif

#if !defined(HIRZEL_Value_JSON_I) && defined(HIRZEL_IMPLEMENT)
#define HIRZEL_Value_JSON_I

namespace hirzel
{
	namespace json
	{
		std::string preprocess(const std::string& src)
		{
			const char *iter = src.c_str();
			std::string out(src.length(), '\0');
			size_t oi = 0;

			while (*iter)
			{
				if (*iter <= ' ')
				{
					iter += 1;
					continue;
				}
			
				if (*iter == '\"')
				{
					const char *start_of_string = iter;

					out[oi++] = *iter;
					iter += 1;

					while (true)
					{
						if (*iter == '\0')
						{
							size_t size_of_preview = (iter - start_of_string > 15 ? 15 : iter - start_of_string);
							throw SyntaxError("unterminated string starting with: "
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

		bool parseNumberLiteral(const char *& iter)
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
							throw SyntaxError("extra '.' in number literal");

						iter += 1;
						isFloat = true;
						expecting_digit = true;
						break;

					default:
						if (expecting_digit)
							throw SyntaxError("number literals must end in a digit");

						return isFloat;
				}
			}
		}

		JsonValue parseNumber(const char *& iter)
		{
			const char * const start_of_number = iter;

			try
			{
				bool is_base_decimal = parseNumberLiteral(iter);

				if (*iter == 'e' || *iter == 'E')
				{
					iter += 1;

					if (parseNumberLiteral(iter))
						throw SyntaxError("exponents must be integers");
				}

				std::string number_literal(start_of_number, iter - start_of_number);

				return is_base_decimal
					? JsonValue(std::stod(number_literal))
					: JsonValue(std::stoll(number_literal));
			}
			catch (const std::exception& e)
			{
				throw SyntaxError("failed to parse number '"
					+ std::string(start_of_number, iter - start_of_number)
					+ "': "
					+ std::string(e.what()));
			}
		}

		JsonValue parseString(const char *& iter)
		{
			iter += 1;

			const char * const start_of_string = iter;

			while (*iter != '\"')
			{
				if (*iter == '\0')
					throw SyntaxError("unterminated string: "
						+ std::string(start_of_string, iter - start_of_string));

				iter += 1;
			}

			JsonValue out = std::string(start_of_string, iter - start_of_string);

			iter += 1;

			return out;
		}

		JsonValue parseNull(const char *& iter)
		{
			iter += 1;

			for (const char *match = "ull"; *match; ++match)
			{
				if (*iter != *match)
					throw SyntaxError("unexpected character '"
						+ std::string(1, *iter)
						+ "' found in null literal");

				iter += 1;
			}

			return JsonValue();
		}

		JsonValue parseBool(const char *&iter, bool expected)
		{
			iter += 1;

			const char *match;

			if (expected)
				match = "rue";
			else
				match = "alse";

			while (*match)
			{
				if (*iter != *match)
					throw SyntaxError("unexpected character '"
						+ std::string(1, *iter)
						+ "' found in bool literal");

				iter += 1;
				match += 1;
			}
			
			return JsonValue(expected);
		}

		JsonValue parseArray(const char *& iter)
		{
			JsonValue arr(ValueType::Array);

			iter += 1;

			if (*iter == ']')
			{
				iter += 1;
				return arr;
			}

			size_t curr_array_index = 0;

			while (true)
			{
				arr[curr_array_index++] = parseValue(iter);

				if (*iter != ',')
					break;
					
				iter += 1;
			}

			if (*iter != ']')
				throw SyntaxError("unterminated array");

			iter += 1;

			return arr;
		}

		JsonValue parseObject(const char *& iter)
		{
			JsonValue obj(ValueType::Object);

			iter += 1;

			if (*iter == '}')
			{
				iter += 1;
				return obj;
			}
			
			while (true)
			{
				if (*iter != '\"')
					throw SyntaxError("invalid label");

				iter += 1;

				const char *end_of_label = iter;
				const char *start_of_label = iter;

				while (*end_of_label != '\"')
				{
					if (!end_of_label[0])
						throw SyntaxError("unterminated label: '"
							+ std::string(start_of_label, end_of_label - start_of_label)
							+ "'");

					end_of_label += 1;
				}

				std::string label(start_of_label, end_of_label - start_of_label);
				iter = end_of_label + 1;

				if (*iter != ':')
					throw SyntaxError("expected ':' but found: "
						+ std::string(iter, 1));

				iter += 1;
				
				obj[label] = parseValue(iter);
				
				if (*iter != ',')
				{
					if (*iter != '}')
						throw SyntaxError("unterminated object found in JSON");
					iter += 1;
					break;
				}

				iter += 1;
			}

			return obj;
		}

		JsonValue parseValue(const char *& iter)
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
					throw SyntaxError("expected value but reached end of input");
				default:
					throw SyntaxError("invalid value starting with '"
						+ std::string(1, *iter)
						+ "'");
			}
		}

		JsonValue deserialize(const std::string& src)
		{
			std::string preprocessed_src = preprocess(src);

			if (preprocessed_src.empty())
				throw SyntaxError("source string was empty");

			const char *iter = preprocessed_src.c_str();

			auto out = parseValue(iter);

			if (*iter != '\0')
				throw SyntaxError("unexpected characters at end of JSON: '" + std::string(iter) + "'");

			return out;
		}

		std::string serialize_array(const JsonValue& JsonValue)
		{
			std::string out = "[";

			const auto& array = JsonValue.array();

			bool is_first_element = true;
			for (const auto& element : array)
			{
				if (!is_first_element)
					out += ',';

				out += serialize_value(element);
				is_first_element = false;
			}

			return out + "]";
		}

		std::string serialize_object(const JsonValue& JsonValue)
		{
			std::string out = "{";
			
			const auto& table = JsonValue.table();

			bool is_first_element = true;
			for (const auto& pair : table)
			{
				if (!is_first_element)
					out += ',';

				out += "\"" + pair.first + "\":" + serialize_value(pair.second);
				is_first_element = false;
			}

			return out + "}";
		}

		std::string serialize_value(const JsonValue& JsonValue)
		{
			switch (JsonValue.type())
			{
				case ValueType::Null:
					return "null";
				case ValueType::Integer:
					return std::to_string(JsonValue.integer());
				case ValueType::Float:
					return std::to_string(JsonValue.decimal());
				case ValueType::Boolean:
					return JsonValue.boolean() ? "true" : "false";
				case ValueType::String:
					return "\"" + JsonValue.string() + "\"";
				case ValueType::Array:
					return serialize_array(JsonValue);
				case ValueType::Object:
					return serialize_object(JsonValue);
				default:
					throw std::runtime_error("Invalid type!");
			}
		}

		JsonValue::JsonValue(ValueType t)
		{
			_type = t;

			switch (t)
			{
			case ValueType::Null:
				break;

			case ValueType::Integer:
				_integer = 0;
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

		int64_t JsonValue::asI64() const
		{
			switch(_type)
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
					catch (const std::exception& e)
					{
						return 0;
					}
				default:
					return 0;
			}
		}

		double JsonValue::asF64() const
		{
			switch(_type)
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
					catch(const std::exception& e)
					{
						return 0.0;
					}
				default:
					return 0.0;
			}
		}

		bool JsonValue::asBool() const
		{
			switch(_type)
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

		namespace details
		{
			void indent(std::string& s)
			{
				for (size_t i = 1; i < s.length(); i++)
				{
					if (s[i] == '\n')
					{
						i++;
						s.insert(i, 1, '\t');
					}
				}
			}
		}

		std::string JsonValue::asString() const
		{
			switch(_type)
			{
				case ValueType::Null:
					return "null";

				case ValueType::Integer:
					return std::to_string(_integer);

				case ValueType::Boolean:
					return (_boolean ? "true" : "false");

				case ValueType::Float:
					return std::to_string(_decimal);

				case ValueType::String:
					return *_string;

				case ValueType::Array:
				{
					std::vector<std::string> str_reps;

					str_reps.reserve(_array->size());

					bool vert = false;

					for (const auto& elem : *_array)
					{
						std::string str = elem.isString()
							? "\"" + elem.string() + "\""
							: elem.asString();

						if (elem.isTable() || elem.isArray())
							vert = true;

						str_reps.push_back(str);
					}

					std::string out = "[";

					auto is_first_elem = true;

					for (auto& str : str_reps)
					{
						if (is_first_elem)
							is_first_elem = false;
						else
							out += ", ";

						if (vert)
							out += "\n\t";

						details::indent(str);
						out += str; 
					}

					if (vert)
						out += '\n';

					out += ']';

					return out;
				}
				case ValueType::Object:
				{
					std::string out;
					std::vector<std::string> str_reps;

					if (_table->empty()) return "{}";
					str_reps.resize(_table->size());
					int i = 0;
					for (auto iter = _table->begin(); iter != _table->end(); iter++)
					{
						const JsonValue& v = iter->second;
						str_reps[i] = "\n\t" + iter->first + ":\t";
						std::string tmp = v.asString();
						if (v.isString())
						{
							tmp.insert(0, 1, '\"');
							tmp.push_back('\"');
						}
						details::indent(tmp);
						str_reps[i++] += tmp;
					}

					out = "{";
					for (i = str_reps.size() - 1; i >= 0; i--)
					{
						out += str_reps[i];
					}
					out += "\n}";

					return out;
				}
				default:
					return "";
			}
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

		size_t JsonValue::length() const
		{
			switch (_type)
			{
			case ValueType::Null:
				return 0;
			case ValueType::String:
				return _string->length();
			case ValueType::Array:
				return _array->size();
			case ValueType::Object:
				return _table->size();
			default:
				return 1;
			}
		}

		JsonValue& JsonValue::operator=(const JsonValue& other)
		{
			_type = other.type();
			
			switch (_type)
			{
				case ValueType::Null:
					_integer = 0;
					break;
				case ValueType::Integer:
					_integer = other.integer();
					break;
				case ValueType::Float:
					_decimal = other.decimal();
					break;
				case ValueType::Boolean:
					_boolean = other.boolean();
					break;
				case ValueType::String:
					_string = new std::string(other.string());
					break;
				case ValueType::Array:
					_array = new JsonArray(other.array());
					break;
				case ValueType::Object:
					_table = new JsonObject(other.table());
					break;
				default:
					throw TypeException("attempted copy JsonValue of invalid type");
			}
			return *this;
		}

		JsonValue& JsonValue::operator=(JsonValue&& other)
		{
			_type = other._type;
			other._type = ValueType::Null;

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
				default:
					throw TypeException("attempted copy JsonValue of invalid type");
			}

			return *this;
		}

		bool JsonValue::operator==(const JsonValue& other) const
		{
			if (_type != other.type()) return false;

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
					// for every child element
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

		const char * JsonValue::typeName() const noexcept
		{
			switch (_type)
			{
				case ValueType::Null:
					return "null";
				case ValueType::Integer:
					return "integer";
				case ValueType::Float:
					return "floating-point";
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
	}
}


#endif
