#ifndef HIRZEL_JSON_HPP
#define HIRZEL_JSON_HPP

/**
 * @file json.hpp
 * @brief De/serialization utilities for JSON
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
#include <cassert>
#include <hirzel/primitives.hpp>

namespace hirzel::json
{
	using namespace hirzel::primitives;

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
		union
		{
			i64 _integer;
			bool _boolean;
			f64 _decimal;
			std::string *_string;
			JsonArray *_array;
			JsonObject *_table;
		};

		ValueType _type;

	public:

		JsonValue();
		JsonValue(ValueType type);
		JsonValue(short i);
		JsonValue(int i);
		JsonValue(long i);
		JsonValue(long long i);
		JsonValue(unsigned short i);
		JsonValue(unsigned int i);
		JsonValue(unsigned long i);
		JsonValue(unsigned long long i);
		JsonValue(float d);
		JsonValue(double d);
		JsonValue(bool b);
		JsonValue(const std::string& s);
		JsonValue(char* s);
		JsonValue(const char* s);
		JsonValue(JsonArray&& array);
		JsonValue(const JsonArray& array);
		JsonValue(JsonObject&& table);
		JsonValue(const JsonObject& table);
		JsonValue(JsonValue&& other) noexcept;
		JsonValue(const JsonValue& other);
		~JsonValue();

		template <typename T>
		static JsonValue from(const std::vector<T>& array)
		{
			JsonArray out;

			out.resize(array.size());

			for (usize i = 0; i < array.size(); ++i)
				out[i] = array[i];

			return out;
		}

		template <typename T>
		static JsonValue from(const std::unordered_map<std::string, T>& table)
		{
			JsonObject out;

			for (const auto& pair : table)
				out[pair.first] = pair.second;

			return out;
		}

		auto& integer() { return _integer; }
		const auto& integer() const { return _integer; }

		auto& decimal() { return _decimal; }
		const auto& decimal() const { return _decimal; }

		auto& boolean() { return _boolean; }
		const auto& boolean() const { return _boolean; }

		auto& string() { return *_string; }
		const auto& string() const { return *_string; }

		auto& array() { return *_array; }
		const auto& array() const { return *_array; }

		auto& table() { return *_table; }
		const auto& table() const { return *_table; }

		i64 asI64() const;
		i32 asI32() const { return (int32_t)asI64(); }
		i16 asI16() const { return (int16_t)asI64(); }
		i8 asI8() const { return (int8_t)asI64(); }
		double asF64() const;
		float asF32() const { return (float)asF64(); }
		bool asBool() const;
		std::string	asString() const;

		bool contains(const std::string& key) const
		{
			return _type == ValueType::Object ?
				_table->find(key) != _table->end() :
				false;
		}

		bool isEmpty() const;
		bool isNull() const { return _type == ValueType::Null; }
		bool isInteger() const { return _type == ValueType::Integer; }
		bool isFloat() const { return _type == ValueType::Float; }
		bool isNumber() const { return _type == ValueType::Integer || _type == ValueType::Float; }
		bool isBoolean() const { return _type == ValueType::Boolean; }
		bool isString() const { return _type == ValueType::String; }
		bool isArray() const { return _type == ValueType::Array; }
		bool isObject() const { return _type == ValueType::Object; }

		usize length() const;
		const auto& type() const { return _type; }
		const char* typeName() const noexcept;

		JsonValue& operator=(JsonValue&& other);
		JsonValue& operator=(const JsonValue& other);

		JsonValue& at(usize i);
		JsonValue& operator[](usize i) { return at(i); }
		const JsonValue& at(usize i) const;
		const JsonValue& operator[](usize i) const { return at(i); }

		JsonValue& at(const std::string& key);
		JsonValue& operator[](const std::string& key) { return at(key); }
		const JsonValue& at(const std::string& key) const;
		const JsonValue& operator[](const std::string& key) const { return at(key); }

		bool operator==(const JsonValue& other) const;
		bool operator!=(const JsonValue& other) const { return !(*this == other); }

		friend std::ostream& operator<<(std::ostream& out, const JsonValue& json);
	};

	JsonValue read(const std::string& filepath);
	std::string preprocess(const std::string& src);

	JsonValue parse(const std::string& json);
	JsonValue parseNumber(const char *& iter);
	JsonValue parseString(const char *& iter);
	JsonValue parseNull(const char *& iter);
	JsonValue parseBool(const char *& iter, bool expected);
	JsonValue parseArray(const char *& iter);
	JsonValue parseObject(const char *& iter);
	JsonValue parseValue(const char *& iter);

	std::string serialize(const JsonValue& JsonValue);
	std::string serializeArray(const JsonValue& JsonValue);
	std::string serializeObject(const JsonValue& JsonValue);
	std::string serializeValue(const JsonValue& JsonValue);
}

#endif
