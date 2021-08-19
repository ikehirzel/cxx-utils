#ifndef HIRZEL_DATA_DATA_H
#define HIRZEL_DATA_DATA_H

/**
 * @file data.h
 * @brief A universal data type
 * @author Ike Hirzel
 * 
 * Copyright 2021 Ike Hirzel
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
#include <vector>

namespace hirzel::data
{
	class Data
	{
	public:
		typedef std::unordered_map<std::string, Data> Table;
		typedef std::vector<Data> Array;

		enum Type
		{
			NULL_TYPE,
			INTEGER_TYPE,
			DECIMAL_TYPE,
			BOOLEAN_TYPE,
			STRING_TYPE,
			ARRAY_TYPE,
			TABLE_TYPE
		};

		class ParseException : public std::exception
		{
		private:
			std::string _msg;
		public:
			ParseException(const std::string& msg) : _msg(msg) {}
			const char *what() const noexcept override
			{
				return _msg.c_str();
			}
		};

		class TypeException : public std::exception
		{
		private:
			std::string _msg;
		public:
			TypeException(const std::string& msg) : _msg(msg) {}

			inline static TypeException cast_exception(const char *from, const char *to)
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

	private: // data members

		Type _type = NULL_TYPE;
		union
		{
			long long _integer = 0;
			bool _boolean;
			double _decimal;
			std::string *_string;
			Array *_array;
			Table *_table;
		};

	public:

		Data() : _type(NULL_TYPE), _integer(0) {}
		Data(const Data& other) { *this = other; }
		Data(Data&& other) { *this = other; }
		Data(Type t);

		inline Data(short i) : _type(INTEGER_TYPE), _integer(i) {}
		inline Data(int i) : _type(INTEGER_TYPE), _integer(i) {}
		inline Data(long i) : _type(INTEGER_TYPE), _integer(i) {}
		inline Data(long long i) : _type(INTEGER_TYPE), _integer(i) {}

		inline Data(unsigned short i) : _type(INTEGER_TYPE), _integer(i) {}
		inline Data(unsigned int i) : _type(INTEGER_TYPE), _integer(i) {}
		inline Data(unsigned long i) : _type(INTEGER_TYPE), _integer(i) {}
		inline Data(unsigned long long i) : _type(INTEGER_TYPE), _integer(i) {}
		
		inline Data(float d) : _type(DECIMAL_TYPE), _decimal(d) {}
		inline Data(double d) : _type(DECIMAL_TYPE), _decimal(d) {}

		inline Data(bool b) : _type(BOOLEAN_TYPE), _boolean(b) {}

		inline Data(const std::string& s) : _type(STRING_TYPE), _string(new std::string(s)) {}
		inline Data(char* s) : _type(STRING_TYPE), _string(new std::string(s)) {}
		inline Data(const char* s) : _type(STRING_TYPE), _string(new std::string(s)) {}

		inline Data(const Array& array) : _type(ARRAY_TYPE), _array(new Array(array)) {}
		inline Data(const Table& table) : _type(TABLE_TYPE), _table(new Table(table)) {}

		~Data();

		inline long long integer() const { return _integer; }
		inline double decimal() const { return _decimal; }
		inline bool boolean() const { return _boolean; }
		inline const std::string& string() const { return *_string; }
		inline const Array& array() const { return *_array; }
		inline const Table& table() const { return *_table; }

		long long as_long_long() const;
		inline long as_long() const { return (long)as_long_long(); }
		inline int as_int() const { return (int)as_long_long(); }
		inline short as_short() const { return (short)as_long_long(); }

		double as_double() const;
		inline float as_float() const { return (float)as_double(); }
		bool as_bool() const;
		std::string	as_string() const;

		inline bool get_boolean() const
		{
			if (_type != BOOLEAN_TYPE)
				throw TypeException::cast_exception(type_name(), "boolean");

			return _boolean;
		}	

		inline long long get_integer() const
		{
			if (_type != BOOLEAN_TYPE)
				throw TypeException::cast_exception(type_name(), "integer");
				
			return _boolean;
		}

		inline double get_decimal() const 
		{
			if (_type != DECIMAL_TYPE)
				throw TypeException::cast_exception(type_name(), "decimal");
				
			return _decimal;
		}

		inline const std::string& get_string() const
		{
			if (_type != STRING_TYPE)
				throw TypeException::cast_exception(type_name(), "string");

			return *_string;
		}

		inline const Array& get_array() const
		{
			if (_type != ARRAY_TYPE)
				throw TypeException::cast_exception(type_name(), "array");

			return *_array;
		}

		inline const Table& get_table() const
		{
			if (_type != TABLE_TYPE)
				throw TypeException::cast_exception(type_name(), "rable");
			
			return *_table;
		}
		
		std::string as_json() const;

		inline bool contains(const std::string& key) const
		{
			return _type == TABLE_TYPE ?
				_table->find(key) != _table->end() :
				false;
		}

		bool is_empty() const;
		size_t size() const;

		inline bool is_null() const noexcept { return _type == NULL_TYPE; }
		inline bool is_integer() const noexcept { return _type == INTEGER_TYPE; }
		inline bool is_decimal() const noexcept { return _type == DECIMAL_TYPE; }
		inline bool is_number() const noexcept { return _type == INTEGER_TYPE || _type == DECIMAL_TYPE; }
		inline bool is_boolean() const noexcept { return _type == BOOLEAN_TYPE; }
		inline bool is_string() const noexcept { return _type == STRING_TYPE; }
		inline bool is_array() const noexcept { return _type == ARRAY_TYPE; }
		inline bool is_table() const noexcept { return _type == TABLE_TYPE; }

		inline Type type() const { return _type; }
		const char *type_name() const noexcept;

		Data& operator=(const Data& other);
		Data& operator=(Data&& other);

		inline Data& at(size_t i)
		{
			if (_type != ARRAY_TYPE)
				throw TypeException("data must be array for integer indexing");

			if (i >= _array->size())
			{
				_array->resize(i + 1);
			}

			return (*_array)[i];
		}

		inline const Data& at(size_t i) const
		{
			if (_type != ARRAY_TYPE)
				throw TypeException("data must be array for integer indexing");

			if (i >= _array->size())
				throw std::out_of_range("attempted to access index "
					+ std::to_string(i)
					+ " but size is "
					+ std::to_string(_array->size()));

			return (*_array)[i];
		}

		inline Data& operator[](size_t i)
		{
			return at(i); 
		}

		inline const Data& operator[](size_t i) const
		{
			return at(i); 
		}

		inline Data& at(const std::string& key)
		{
			if (_type != TABLE_TYPE)
				throw TypeException("data must be a table for string indexing");
			
			return (*_table)[key];
		}

		inline const Data& at(const std::string& key) const
		{
			if (_type != TABLE_TYPE)
				throw TypeException("data must be a table for string indexing");

			if (_table->find(key) == _table->end())
				throw std::out_of_range("attempted to access element at '"
					+ key + "' but it does not exist");

			return (*_table)[key];
		}

		inline Data& operator[](const std::string& key)
		{
			return at(key); 
		}

		inline const Data& operator[](const std::string& key) const
		{ 
			return at(key);
		}

		bool operator==(const Data& other) const;
		inline bool operator!=(const Data& other) const { return !(*this == other); }
	};
}

#endif // HIRZEL_DATA_H

#if !defined(HIRZEL_DATA_DATA_I) && defined(HIRZEL_IMPLEMENT)
#define HIRZEL_DATA_DATA_I

namespace hirzel::data
{
	Data::Data(Type t)
	{
		_type = t;

		switch (t)
		{
		case NULL_TYPE:
			break;

		case INTEGER_TYPE:
			_integer = 0;
			break;

		case DECIMAL_TYPE:
			_decimal = 0.0;
			break;

		case BOOLEAN_TYPE:
			_boolean = false;
			break;

		case STRING_TYPE:
			_string = new std::string();
			break;
			
		case ARRAY_TYPE:
			_array = new Array();
			break;

		case TABLE_TYPE:
			_table = new Table();
			break;
		}
	}

	Data::~Data()
	{
		switch (_type)
		{
			case STRING_TYPE:
				delete _string;
				break;
			case ARRAY_TYPE:
				delete _array;
				break;
			case TABLE_TYPE:
				delete _table;
				break;
			default:
				break;
		}
	}

	long long Data::as_long_long() const
	{
		switch(_type)
		{
			case INTEGER_TYPE:
				return _integer;
			case BOOLEAN_TYPE:
				return (long long)_boolean;
			case DECIMAL_TYPE:
				return (long long)_decimal;
			case STRING_TYPE:
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

	double Data::as_double() const
	{
		switch(_type)
		{
			case INTEGER_TYPE:
				return (double)_integer;
			case BOOLEAN_TYPE:
				return (double)_boolean;
			case DECIMAL_TYPE:
				return _decimal;
			case STRING_TYPE:
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

	bool Data::as_bool() const
	{
		switch(_type)
		{
			case INTEGER_TYPE:
				return (bool)_integer;
			case BOOLEAN_TYPE:
				return _boolean;
			case DECIMAL_TYPE:
				return (bool)_decimal;
			case STRING_TYPE:
				return !_string->empty();
			default:
				return false;
		}
	}

	std::string Data::as_json() const
	{
		switch(_type)
		{
			case INTEGER_TYPE:
				return std::to_string(_integer);
			case BOOLEAN_TYPE:
				return (_boolean ? "true" : "false");
			case DECIMAL_TYPE:
				return std::to_string(_decimal);
			case STRING_TYPE:
				return "\"" + *_string + "\"";
			case ARRAY_TYPE:
			{
				std::string out;

				out = "[";
				for (auto iter = _array->begin(); iter != _array->end(); iter++)
				{
					if (iter != _array->begin()) out += ',';
					out += iter->as_json();
				}
				out += ']';

				return out;
			}
			case TABLE_TYPE:
			{
				std::string out;

				std::vector<std::string> str_reps(_table->size());
				int i = 0;
				for (auto iter = _table->begin(); iter != _table->end(); iter++)
				{
					str_reps[i++] = "\"" + iter->first + "\":" + iter->second.as_json();
				}

				out = "{";
				for (i = i - 1; i >= 0; i--)
				{
					out += str_reps[i];
					if (i > 0) out += ',';
				}
				out += '}';

				return out;
			}
			default:
				return "null";
		}
	}

	namespace details
	{
		void indent(std::string& s)
		{
			for (size_t i = 1; i < s.size(); i++)
			{
				if (s[i] == '\n')
				{
					i++;
					s.insert(i, 1, '\t');
				}
			}
		}
	}

	std::string Data::as_string() const
	{
		switch(_type)
		{
			case NULL_TYPE:
				return "null";

			case INTEGER_TYPE:
				return std::to_string(_integer);

			case BOOLEAN_TYPE:
				return (_boolean ? "true" : "false");

			case DECIMAL_TYPE:
				return std::to_string(_decimal);

			case STRING_TYPE:
				return *_string;

			case ARRAY_TYPE:
			{
				std::string out;
				std::vector<std::string> str_reps(_array->size());

				bool vert = false;

				for (size_t i = 0; i < _array->size(); i++)
				{
					const Data& v = (_array)[i];
					std::string tmp = v.as_string();
					if (v.is_string())
					{
						tmp.insert(0, 1, '\"');
						tmp.push_back('\"');
					}
					else if (v.is_table())
					{
						vert = true;
					}
					str_reps[i] = tmp;
				}


				out = "[";

				for (size_t i = 0; i < str_reps.size(); ++i)
				{
					if (vert)
					{
						out += "\n\t";
					}
					else
					{
						if (i > 0) out += ' ';
					}
					
					details::indent(str_reps[i]);
					out += str_reps[i];
					if (i < str_reps.size() - 1) out += ',';
				}
				if (vert) out += '\n';
				out += ']';

				return out;
			}
			case TABLE_TYPE:
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
					std::string tmp = v.as_string();
					if (v.is_string())
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

	bool Data::is_empty() const
	{
		switch (_type)
		{
		case STRING_TYPE:
			return _string->empty();
		case ARRAY_TYPE:
			return _array->empty();
		case TABLE_TYPE:
			return _table->empty();
		case NULL_TYPE:
			return true;
		default:
			return false;
		}
	}

	size_t Data::size() const
	{
		switch (_type)
		{
		case NULL_TYPE:
			return 0;
		case STRING_TYPE:
			return _string->size();
		case ARRAY_TYPE:
			return _array->size();
		case TABLE_TYPE:
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
			case NULL_TYPE:
				_integer = 0;
				break;
			case INTEGER_TYPE:
				_integer = other.integer();
				break;
			case DECIMAL_TYPE:
				_decimal = other.decimal();
				break;
			case BOOLEAN_TYPE:
				_boolean = other.boolean();
				break;
			case STRING_TYPE:
				_string = new std::string(other.string());
				break;
			case ARRAY_TYPE:
				_array = new Array(other.array());
				break;
			case TABLE_TYPE:
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
		other._type = NULL_TYPE;

		switch (_type)
		{
			case NULL_TYPE:
				break;
			case INTEGER_TYPE:
				_integer = other._integer;
				break;
			case DECIMAL_TYPE:
				_decimal = other._decimal;
				break;
			case BOOLEAN_TYPE:
				_boolean = other._boolean;
				break;
			case STRING_TYPE:
				_string = other._string;
				other._string = nullptr;
				break;
			case ARRAY_TYPE:
				_array = other._array;
				other._array = nullptr;
				break;
			case TABLE_TYPE:
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
			case NULL_TYPE:
				return true;
			case INTEGER_TYPE:
				return _integer == other.integer();
			case DECIMAL_TYPE:
				return _decimal == other.decimal();
			case BOOLEAN_TYPE:
				return _boolean == other.boolean();
			case STRING_TYPE:
				return *_string == other.string();
			case ARRAY_TYPE:
			{
				// for every child element
				const auto& arr = *_array;
				const auto& oarr = other.array();

				for (size_t i = 0; i < arr.size(); ++i)
				{
					if (arr[i] != oarr[i]) return false;
				}

				return true;
			}

			case TABLE_TYPE:
			{
				const auto& table = *_table;
				const auto& otable = other.table();

				for (auto p : table)
				{
					auto iter = otable.find(p.first);
					if (iter == otable.end()) return false;
					auto op = *iter;
					if (p.first != op.first) return false;
					if (p.second != op.second) return false;
				}
				return true;
			}
		}

		return false;
	}

	const char * Data::type_name() const noexcept
	{
		switch (_type)
		{
			case NULL_TYPE:
				return "null";
			case INTEGER_TYPE:
				return "integer";
			case DECIMAL_TYPE:
				return "floating-point";
			case BOOLEAN_TYPE:
				return "boolean";
			case STRING_TYPE:
				return "string";
			case ARRAY_TYPE:
				return "array";
			case TABLE_TYPE:
				return "table";
			default:
				return "invalid-type";
		}
	}
} // namespace hirzel

#endif
