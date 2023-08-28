#ifndef HIRZEL_DATA_HPP
#define HIRZEL_DATA_HPP

/**
 * @file data.hpp
 * @brief A universal data type
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

#include <string>
#include <unordered_map>
#include <stdexcept>
#include <vector>
#include <cstdint>

namespace hirzel
{
	enum class DataType
	{
		Null,
		Integer,
		Float,
		Boolean,
		String,
		Array,
		Table
	};

	class Data
	{
	public:
		typedef std::unordered_map<std::string, Data> Table;
		typedef std::vector<Data> Array;


		class TypeException : public std::exception
		{
		private:
			std::string _msg;
		public:
			TypeException(const std::string& msg) : _msg(msg) {}

			static TypeException cast_exception(const char *from, const char *to)
			{
				return TypeException("data of type '"
					+ std::string(from)
					+ "' cannot be cast as "
					+ std::string(to));
			}

			const char *what() const noexcept override
			{
				return _msg.c_str(); 
			}
		};

	private:

		DataType _type = DataType::Null;
		union
		{
			int64_t _integer = 0;
			bool _boolean;
			double _decimal;
			std::string *_string;
			Array *_array;
			Table *_table;
		};

	public:

		Data() : _type(DataType::Null), _integer(0) {}
		Data(const Data& other) { *this = other; }
		Data(Data&& other) { *this = other; }
		Data(DataType t);

		Data(short i) : _type(DataType::Integer), _integer(i) {}
		Data(int i) : _type(DataType::Integer), _integer(i) {}
		Data(long i) : _type(DataType::Integer), _integer(i) {}
		Data(long long i) : _type(DataType::Integer), _integer(i) {}

		Data(unsigned short i) : _type(DataType::Integer), _integer(i) {}
		Data(unsigned int i) : _type(DataType::Integer), _integer(i) {}
		Data(unsigned long i) : _type(DataType::Integer), _integer(i) {}
		Data(unsigned long long i) : _type(DataType::Integer), _integer(i) {}
		
		Data(float d) : _type(DataType::Float), _decimal(d) {}
		Data(double d) : _type(DataType::Float), _decimal(d) {}

		Data(bool b) : _type(DataType::Boolean), _boolean(b) {}

		Data(const std::string& s) : _type(DataType::String), _string(new std::string(s)) {}
		Data(char* s) : _type(DataType::String), _string(new std::string(s)) {}
		Data(const char* s) : _type(DataType::String), _string(new std::string(s)) {}

		Data(const Array& array) : _type(DataType::Array), _array(new Array(array)) {}
		Data(const Table& table) : _type(DataType::Table), _table(new Table(table)) {}

		template <typename T>
		static Data from(const std::vector<T> array)
		{
			Data::Array out;

			out.resize(array.size());

			for (size_t i = 0; i < array.size(); ++i)
				out[i] = array[i];

			return out;
		}

		template <typename T>
		static Data from(const std::unordered_map<std::string, T> table)
		{
			Data::Table out;

			for (const auto& pair : table)
				out[pair.first] = pair.second;

			return out;
		}

		~Data();

		long long integer() const { return _integer; }
		double decimal() const { return _decimal; }
		bool boolean() const { return _boolean; }
		const std::string& string() const { return *_string; }
		const Array& array() const { return *_array; }
		const Table& table() const { return *_table; }

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
			if (_type != DataType::Boolean)
				throw TypeException::cast_exception(typeName(), "boolean");

			return _boolean;
		}	

		long long toI64() const
		{
			if (_type != DataType::Integer)
				throw TypeException::cast_exception(typeName(), "integer");
				
			return _integer;
		}

		double toF64() const 
		{
			if (_type != DataType::Float)
				throw TypeException::cast_exception(typeName(), "decimal");
				
			return _decimal;
		}
		const std::string& toString() const

		{
			if (_type != DataType::String)
				throw TypeException::cast_exception(typeName(), "string");

			return *_string;
		}

		const Array& toArray() const
		{
			if (_type != DataType::Array)
				throw TypeException::cast_exception(typeName(), "array");

			return *_array;
		}

		const Table& toTable() const
		{
			if (_type != DataType::Table)
				throw TypeException::cast_exception(typeName(), "rable");
			
			return *_table;
		}

		bool contains(const std::string& key) const
		{
			return _type == DataType::Table ?
				_table->find(key) != _table->end() :
				false;
		}

		bool isEmpty() const;
		size_t length() const;

		bool isNull() const noexcept { return _type == DataType::Null; }
		bool isInteger() const noexcept { return _type == DataType::Integer; }
		bool isFloat() const noexcept { return _type == DataType::Float; }
		bool isNumber() const noexcept { return _type == DataType::Integer || _type == DataType::Float; }
		bool isBoolean() const noexcept { return _type == DataType::Boolean; }
		bool isString() const noexcept { return _type == DataType::String; }
		bool isArray() const noexcept { return _type == DataType::Array; }
		bool isTable() const noexcept { return _type == DataType::Table; }

		DataType type() const { return _type; }
		const char *typeName() const noexcept;

		Data& operator=(const Data& other);
		Data& operator=(Data&& other);

		Data& at(size_t i)
		{
			if (_type != DataType::Array)
				throw TypeException("data must be array for integer indexing");

			if (i >= _array->size())
			{
				_array->resize(i + 1);
			}

			return (*_array)[i];
		}

		const Data& at(size_t i) const
		{
			if (_type != DataType::Array)
				throw TypeException("data must be array for integer indexing");

			if (i >= _array->size())
				throw std::out_of_range("attempted to access index "
					+ std::to_string(i)
					+ " but size is "
					+ std::to_string(_array->size()));

			return (*_array)[i];
		}

		Data& operator[](size_t i)
		{
			return at(i); 
		}

		const Data& operator[](size_t i) const
		{
			return at(i); 
		}

		Data& at(const std::string& key)
		{
			if (_type != DataType::Table)
				throw TypeException("data must be a table for string indexing");
			
			return (*_table)[key];
		}

		const Data& at(const std::string& key) const
		{
			if (_type != DataType::Table)
				throw TypeException("data must be a table for string indexing");

			if (_table->find(key) == _table->end())
				throw std::out_of_range("attempted to access element at '"
					+ key + "' but it does not exist");

			return (*_table)[key];
		}

		Data& operator[](const std::string& key)
		{
			return at(key); 
		}

		const Data& operator[](const std::string& key) const
		{ 
			return at(key);
		}

		bool operator==(const Data& other) const;
		bool operator!=(const Data& other) const { return !(*this == other); }
	};
}

#endif // HIRZEL_DATA_HPP

#if !defined(HIRZEL_DATA_DATA_I) && defined(HIRZEL_IMPLEMENT)
#define HIRZEL_DATA_DATA_I

namespace hirzel
{
	Data::Data(DataType t)
	{
		_type = t;

		switch (t)
		{
		case DataType::Null:
			break;

		case DataType::Integer:
			_integer = 0;
			break;

		case DataType::Float:
			_decimal = 0.0;
			break;

		case DataType::Boolean:
			_boolean = false;
			break;

		case DataType::String:
			_string = new std::string();
			break;
			
		case DataType::Array:
			_array = new Array();
			break;

		case DataType::Table:
			_table = new Table();
			break;
		}
	}

	Data::~Data()
	{
		switch (_type)
		{
			case DataType::String:
				delete _string;
				break;
			case DataType::Array:
				delete _array;
				break;
			case DataType::Table:
				delete _table;
				break;
			default:
				break;
		}
	}

	int64_t Data::asI64() const
	{
		switch(_type)
		{
			case DataType::Integer:
				return _integer;
			case DataType::Boolean:
				return (long long)_boolean;
			case DataType::Float:
				return (long long)_decimal;
			case DataType::String:
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

	double Data::asF64() const
	{
		switch(_type)
		{
			case DataType::Integer:
				return (double)_integer;
			case DataType::Boolean:
				return (double)_boolean;
			case DataType::Float:
				return _decimal;
			case DataType::String:
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

	bool Data::asBool() const
	{
		switch(_type)
		{
			case DataType::Integer:
				return (bool)_integer;
			case DataType::Boolean:
				return _boolean;
			case DataType::Float:
				return (bool)_decimal;
			case DataType::String:
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

	std::string Data::asString() const
	{
		switch(_type)
		{
			case DataType::Null:
				return "null";

			case DataType::Integer:
				return std::to_string(_integer);

			case DataType::Boolean:
				return (_boolean ? "true" : "false");

			case DataType::Float:
				return std::to_string(_decimal);

			case DataType::String:
				return *_string;

			case DataType::Array:
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
			case DataType::Table:
			{
				std::string out;
				std::vector<std::string> str_reps;

				if (_table->empty()) return "{}";
				str_reps.resize(_table->size());
				int i = 0;
				for (auto iter = _table->begin(); iter != _table->end(); iter++)
				{
					const Data& v = iter->second;
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

	bool Data::isEmpty() const
	{
		switch (_type)
		{
		case DataType::String:
			return _string->empty();
		case DataType::Array:
			return _array->empty();
		case DataType::Table:
			return _table->empty();
		case DataType::Null:
			return true;
		default:
			return false;
		}
	}

	size_t Data::length() const
	{
		switch (_type)
		{
		case DataType::Null:
			return 0;
		case DataType::String:
			return _string->length();
		case DataType::Array:
			return _array->size();
		case DataType::Table:
			return _table->size();
		default:
			return 1;
		}
	}

	Data& Data::operator=(const Data& other)
	{
		_type = other.type();
		
		switch (_type)
		{
			case DataType::Null:
				_integer = 0;
				break;
			case DataType::Integer:
				_integer = other.integer();
				break;
			case DataType::Float:
				_decimal = other.decimal();
				break;
			case DataType::Boolean:
				_boolean = other.boolean();
				break;
			case DataType::String:
				_string = new std::string(other.string());
				break;
			case DataType::Array:
				_array = new Array(other.array());
				break;
			case DataType::Table:
				_table = new Table(other.table());
				break;
			default:
				throw TypeException("attempted copy data of invalid type");
		}
		return *this;
	}

	Data& Data::operator=(Data&& other)
	{
		_type = other._type;
		other._type = DataType::Null;

		switch (_type)
		{
			case DataType::Null:
				break;
			case DataType::Integer:
				_integer = other._integer;
				break;
			case DataType::Float:
				_decimal = other._decimal;
				break;
			case DataType::Boolean:
				_boolean = other._boolean;
				break;
			case DataType::String:
				_string = other._string;
				other._string = nullptr;
				break;
			case DataType::Array:
				_array = other._array;
				other._array = nullptr;
				break;
			case DataType::Table:
				_table = other._table;
				other._table = nullptr;
				break;
			default:
				throw TypeException("attempted copy data of invalid type");
		}

		return *this;
	}

	bool Data::operator==(const Data& other) const
	{
		if (_type != other.type()) return false;

		switch (_type)
		{
			case DataType::Null:
				return true;
			case DataType::Integer:
				return _integer == other.integer();
			case DataType::Float:
				return _decimal == other.decimal();
			case DataType::Boolean:
				return _boolean == other.boolean();
			case DataType::String:
				return *_string == other.string();
			case DataType::Array:
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

			case DataType::Table:
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

	const char * Data::typeName() const noexcept
	{
		switch (_type)
		{
			case DataType::Null:
				return "null";
			case DataType::Integer:
				return "integer";
			case DataType::Float:
				return "floating-point";
			case DataType::Boolean:
				return "boolean";
			case DataType::String:
				return "string";
			case DataType::Array:
				return "array";
			case DataType::Table:
				return "table";
			default:
				return "invalid-type";
		}
	}
}

#endif
