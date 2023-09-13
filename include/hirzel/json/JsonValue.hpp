#ifndef HIRZEL_JSON_JSON_VALUE_HPP
#define HIRZEL_JSON_JSON_VALUE_HPP

#include <hirzel/json/JsonValueType.hpp>
#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <cstdint>
#include <cassert>

namespace hirzel::json
{
	class JsonValue;

	using JsonObject = std::unordered_map<std::string, JsonValue>;
	using JsonArray = std::vector<JsonValue>;

	class JsonValue
	{
		JsonValueType _type;
		union
		{
			bool _boolean;
			double _number;
			std::string* _string;
			JsonArray* _array;
			JsonObject* _object;
		};

	public:

		JsonValue();
		JsonValue(JsonValueType type);
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
		JsonValue(std::string&& s);
		JsonValue(const std::string& s);
		JsonValue(char* s);
		JsonValue(const char* s);
		JsonValue(JsonArray&& array);
		JsonValue(const JsonArray& array);
		JsonValue(JsonObject&& object);
		JsonValue(const JsonObject& object);
		JsonValue(JsonValue&& other) noexcept;
		JsonValue(const JsonValue& other);
		~JsonValue();

		template <typename T>
		static JsonValue from(const std::vector<T>& array)
		{
			JsonArray out;

			out.resize(array.size());

			for (size_t i = 0; i < array.size(); ++i)
				out[i] = array[i];

			return out;
		}

		template <typename T>
		static JsonValue from(const std::unordered_map<std::string, T>& object)
		{
			JsonObject out;

			for (const auto& pair : object)
				out[pair.first] = pair.second;

			return out;
		}

		double& number() { assert(_type == JsonValueType::Number); return _number; }
		const double& number() const { assert(_type == JsonValueType::Number); return _number; }

		bool& boolean() { assert(_type == JsonValueType::Boolean); return _boolean; }
		const bool& boolean() const { assert(_type == JsonValueType::Boolean); return _boolean; }

		std::string& string() { assert(_type == JsonValueType::String); return *_string; }
		const std::string& string() const { assert(_type == JsonValueType::String); return *_string; }

		auto& array() { assert(_type == JsonValueType::Array); return *_array; }
		const auto& array() const { assert(_type == JsonValueType::Array); return *_array; }

		auto& object() { assert(_type == JsonValueType::Object); return *_object; }
		const auto& object() const { assert(_type == JsonValueType::Object); return *_object; }

		int64_t asInteger() const;
		double asDecimal() const;
		bool asBoolean() const;
		std::string	asString() const;

		bool contains(const std::string& key) const
		{
			return _type == JsonValueType::Object ?
				_object->find(key) != _object->end() :
				false;
		}

		bool isEmpty() const;
		bool isNull() const { return _type == JsonValueType::Null; }
		bool isDecimal() const { return _type == JsonValueType::Number; }
		bool isNumber() const { return _type == JsonValueType::Number; }
		bool isBoolean() const { return _type == JsonValueType::Boolean; }
		bool isString() const { return _type == JsonValueType::String; }
		bool isArray() const { return _type == JsonValueType::Array; }
		bool isObject() const { return _type == JsonValueType::Object; }

		size_t length() const;
		const auto& type() const { return _type; }
		const char* typeName() const noexcept;

		JsonValue& operator=(JsonValue&& other);
		JsonValue& operator=(const JsonValue& other);

		JsonValue& at(size_t i);
		JsonValue& operator[](size_t i) { return at(i); }
		const JsonValue& at(size_t i) const;
		const JsonValue& operator[](size_t i) const { return at(i); }

		JsonValue& at(const std::string& key);
		JsonValue& operator[](const std::string& key) { return at(key); }
		const JsonValue& at(const std::string& key) const;
		const JsonValue& operator[](const std::string& key) const { return at(key); }

		bool operator==(const JsonValue& other) const;
		bool operator!=(const JsonValue& other) const { return !(*this == other); }

		friend std::ostream& operator<<(std::ostream& out, const JsonValue& json);
	};
}

#endif
