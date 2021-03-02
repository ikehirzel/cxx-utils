
/**
 * @file var.h
 * @brief A universal type much like a JavaScript object
 * @author Ike Hirzel
 * 
 * Copyright 2021 Ike Hirzel
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in the
 * Software without restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so, subject to the
 * following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef VAR_H
#define VAR_H

#include <typeinfo>
#include <stdlib.h>
#include <string>
#include <initializer_list>

namespace hirzel
{
	struct bad_var_cast : public std::exception 
	{
		const char *what() const throw()
		{
			return "Invalid cast of hirzel::var";
		}
	};

	class var
	{
	private:
		union Data
		{
			bool _boolean;
			char _character;
			double _float;
			char* _string;
			uintmax_t _unsigned;
			intmax_t _integer = 0;
			var* _array;
		};
		Data _data;
		// type of data
		char _type = 0;
		// size of data in bytes
		unsigned _size = 0;
		// universal constructor
	public:

		var() = default;
		var(const var& other);
		var(var&& other);

		var(short i);
		var(int i);
		var(long i);
		var(long long i);

		var(unsigned short i);
		var(unsigned int i);
		var(unsigned long i);
		var(unsigned long long i);
		
		var(float f);
		var(double d);

		var(bool b);

		var(char c);
		var(char* c);
		var(const char* c);
		var(const std::string& s);

		var(const std::initializer_list<var>& list);

		~var();
		
		intmax_t to_int() const;
		uintmax_t to_uint() const;
		float to_float() const;
		double to_double() const;
		char to_char() const;
		std::string to_string() const;
		bool to_bool() const;

		inline Data data() const { return _data; }
		inline unsigned size() const { return _size; }
		inline int type() const { return (int)_type; }

		var& operator=(const var& other);
		var& operator[](size_t i);
		const var& operator[](size_t i) const;
	};
}

#endif

#ifdef HIRZEL_VAR_IMPLEMENTATION
#undef HIRZEL_VAR_IMPLEMENTATION

#include <string>
#include <cstdlib>
#include <cstring>
#include <iostream>

namespace hirzel
{
	enum {
		NULL_TYPE,
		INT_TYPE,
		UINT_TYPE,
		FLOAT_TYPE,
		CHAR_TYPE,
		STR_TYPE,
		BOOL_TYPE,
		ARRAY_TYPE,
		MAP_TYPE
	};

	var::var(const var& other)
	{
		std::cout << "Copy constructor\n";
		*this = other;	
	}

	var::var(var&& other)
	{
		std::cout << "Move constructor\n";
		_size = other._size;
		_type = other._type;
		_data = other._data;

		other._type = NULL_TYPE;
	}

#define VAR_CONSTRUCTOR_IMPL(valtype, member, type) var::var(valtype val)\
{ _data.member = val; _type = type; _size = sizeof(_data.member); }

	VAR_CONSTRUCTOR_IMPL(short, _integer, INT_TYPE);
	VAR_CONSTRUCTOR_IMPL(int, _integer, INT_TYPE);
	VAR_CONSTRUCTOR_IMPL(long, _integer, INT_TYPE);
	VAR_CONSTRUCTOR_IMPL(long long, _integer, INT_TYPE);

	VAR_CONSTRUCTOR_IMPL(unsigned short, _unsigned, UINT_TYPE);
	VAR_CONSTRUCTOR_IMPL(unsigned int, _unsigned, UINT_TYPE);
	VAR_CONSTRUCTOR_IMPL(unsigned long, _unsigned, UINT_TYPE);
	VAR_CONSTRUCTOR_IMPL(unsigned long long, _unsigned, UINT_TYPE);

	VAR_CONSTRUCTOR_IMPL(char, _character, CHAR_TYPE);
	
	VAR_CONSTRUCTOR_IMPL(float, _float, FLOAT_TYPE);
	VAR_CONSTRUCTOR_IMPL(double, _float, FLOAT_TYPE);

	VAR_CONSTRUCTOR_IMPL(bool, _boolean, BOOL_TYPE);

	var::var(const char *c)
	{
		_type = STR_TYPE;
		_size = std::strlen(c);
		_data._string = new char[_size + 1];
		for (int i = 0; i < _size; i++)
		{
			_data._string[i] = c[i];
		}
		_data._string[_size] = 0;
	}
	var::var(char *c) : var((const char*)c) {}
	var::var(const std::string& s) : var(s.c_str()) {}

	var::var(const std::initializer_list<var>& list)
	{
		_size = list.size();
		_type = ARRAY_TYPE;
		_data._array = new var[_size];
		int i = 0;
		for (const var& v : list)
		{
			_data._array[i++] = v;
		}
	}

	var::~var()
	{
		switch (_type)
		{
		case STR_TYPE:
			delete[] _data._string;
			break;

		case ARRAY_TYPE:
			delete[] _data._array;
			break;
		}
	}

	intmax_t var::to_int() const
	{
		switch(_type)
		{
			case NULL_TYPE:
				return 0;
			case INT_TYPE:
				return _data._integer;
			case UINT_TYPE:
				return (intmax_t)_data._unsigned;
			case BOOL_TYPE:
				return (intmax_t)_data._boolean;
			case CHAR_TYPE:
				return (intmax_t)_data._character;
			case FLOAT_TYPE:
				return (intmax_t)_data._float;

			default:
				throw bad_var_cast();
				break;
		}
		return 0;
	}

	uintmax_t var::to_uint() const
	{
		switch(_type)
		{
			case NULL_TYPE:
				return 0;
			case INT_TYPE:
				return (uintmax_t)_data._integer;
			case UINT_TYPE:
				return (uintmax_t)_data._unsigned;
			case BOOL_TYPE:
				return (uintmax_t)_data._boolean;
			case CHAR_TYPE:
				return (uintmax_t)_data._character;
			case FLOAT_TYPE:
				return (uintmax_t)_data._float;

			default:
				throw bad_var_cast();
		}
		return 0;
	}

	double var::to_double() const
	{
		switch(_type)
		{
			case NULL_TYPE:
				return 0.0;
			case INT_TYPE:
				return (double)_data._integer;
			case UINT_TYPE:
				return (double)_data._unsigned;
			case BOOL_TYPE:
				return (double)_data._boolean;
			case CHAR_TYPE:
				return (double)_data._character;
			case FLOAT_TYPE:
				return _data._float;

			default:
				throw bad_var_cast();
		}
		return 0.0;
	}

	char var::to_char() const
	{
		switch(_type)
		{
			case NULL_TYPE:
				return 0;
			case INT_TYPE:
				return (char)_data._integer;
			case UINT_TYPE:
				return (char)_data._unsigned;
			case BOOL_TYPE:
				return (char)_data._boolean;
			case CHAR_TYPE:
				return (char)_data._character;

			case FLOAT_TYPE:
				return (char)_data._float;

			case STR_TYPE:
				return _data._string[0];
		}
		return 0;
	}

	bool var::to_bool() const
	{
		switch(_type)
		{
			case NULL_TYPE:
				return false;
			case INT_TYPE:
				return (bool)_data._integer;
			case UINT_TYPE:
				return (bool)_data._unsigned;
			case BOOL_TYPE:
				return _data._boolean;
			case CHAR_TYPE:
				return (bool)_data._character;
			case FLOAT_TYPE:
				return (bool)_data._float;
			case STR_TYPE:
				return _size > 1;
		}
		return false;
	}

	std::string var::to_string() const
	{
		switch(_type)
		{
			case NULL_TYPE:
				return "";
			case INT_TYPE:
				return std::to_string(_data._integer);
			case UINT_TYPE:
				return std::to_string(_data._unsigned);
			case BOOL_TYPE:
				return (_data._boolean ? "true" : "false");
			case CHAR_TYPE:
				return std::string(1, _data._character);
			case FLOAT_TYPE:
				return std::to_string(_data._float);
			case STR_TYPE:
				return std::string(_data._string);
		}
		return "";
	}

	var& var::operator=(const var& other)
	{
		_type = other.type();
		_size = other.size();
		if (_type == STR_TYPE)
		{
			Data d = other.data();
			// copy string
			_data._string = new char[_size + 1];
			const char* str = d._string;
			for (int i = 0; i < _size; i++)
			{
				_data._string[i] = str[i];
			}
			_data._string[_size] = 0;
		}
		else
		{
			_data = other.data();
		}

		return *this;
	}

	var& var::operator[](size_t i)
	{
		switch (_type)
		{
		case ARRAY_TYPE:
			return _data._array[i];
		}
		// this is placeholder code
		return *this;
	}

	//const var& var::operator[](size_t i) const {}
}
#endif