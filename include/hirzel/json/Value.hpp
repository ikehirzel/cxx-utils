#ifndef HIRZEL_JSON_JSON_VALUE_HPP
#define HIRZEL_JSON_JSON_VALUE_HPP

#include <hirzel/json/ValueType.hpp>
#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <cstdint>
#include <cassert>

namespace hirzel::json
{
	class Value;

	using Object = std::unordered_map<std::string, Value>;
	using Array = std::vector<Value>;

	class Value
	{
		ValueType _type;
		union
		{
			bool _boolean;
			double _number;
			std::string* _string;
			Array* _array;
			Object* _object;
		};

	public:

		Value();
		Value(ValueType type);
		Value(short i);
		Value(int i);
		Value(long i);
		Value(long long i);
		Value(unsigned short i);
		Value(unsigned int i);
		Value(unsigned long i);
		Value(unsigned long long i);
		Value(float d);
		Value(double d);
		Value(bool b);
		Value(std::string&& s);
		Value(const std::string& s);
		Value(char* s);
		Value(const char* s);
		Value(Array&& array);
		Value(const Array& array);
		Value(Object&& object);
		Value(const Object& object);
		Value(Value&& other) noexcept;
		Value(const Value& other);
		~Value();

		template <typename T>
		static Value from(const std::vector<T>& array)
		{
			Array out;

			out.resize(array.size());

			for (size_t i = 0; i < array.size(); ++i)
				out[i] = array[i];

			return out;
		}

		template <typename T>
		static Value from(const std::unordered_map<std::string, T>& object)
		{
			Object out;

			for (const auto& pair : object)
				out[pair.first] = pair.second;

			return out;
		}

		double& number() { assert(_type == ValueType::Number); return _number; }
		const double& number() const { assert(_type == ValueType::Number); return _number; }

		bool& boolean() { assert(_type == ValueType::Boolean); return _boolean; }
		const bool& boolean() const { assert(_type == ValueType::Boolean); return _boolean; }

		std::string& string() { assert(_type == ValueType::String); return *_string; }
		const std::string& string() const { assert(_type == ValueType::String); return *_string; }

		auto& array() { assert(_type == ValueType::Array); return *_array; }
		const auto& array() const { assert(_type == ValueType::Array); return *_array; }

		auto& object() { assert(_type == ValueType::Object); return *_object; }
		const auto& object() const { assert(_type == ValueType::Object); return *_object; }

		int64_t asInteger() const;
		double asDecimal() const;
		bool asBoolean() const;
		std::string	asString() const;

		bool contains(const std::string& key) const
		{
			return _type == ValueType::Object ?
				_object->find(key) != _object->end() :
				false;
		}

		bool isEmpty() const;
		bool isNull() const { return _type == ValueType::Null; }
		bool isDecimal() const { return _type == ValueType::Number; }
		bool isNumber() const { return _type == ValueType::Number; }
		bool isBoolean() const { return _type == ValueType::Boolean; }
		bool isString() const { return _type == ValueType::String; }
		bool isArray() const { return _type == ValueType::Array; }
		bool isObject() const { return _type == ValueType::Object; }

		size_t length() const;
		const auto& type() const { return _type; }
		const char* typeName() const noexcept;

		Value& operator=(Value&& other);
		Value& operator=(const Value& other);

		Value *at(size_t i);
		const Value *at(size_t i) const;
		Value& operator[](size_t i);
		const Value& operator[](size_t i) const;

		Value *at(const std::string& key);
		const Value *at(const std::string& key) const;
		Value& operator[](const std::string& key);
		const Value& operator[](const std::string& key) const;

		bool operator==(const Value& other) const;
		bool operator!=(const Value& other) const { return !(*this == other); }

		friend std::ostream& operator<<(std::ostream& out, const Value& json);
	};
}

#endif
