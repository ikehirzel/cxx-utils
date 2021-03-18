
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

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

namespace hirzel
{
	class var
	{
	private:

		union Data
		{
			bool _boolean;
			char _character;
			double _float;
			std::string* _string;
			unsigned long long _unsigned;
			long long _integer = 0;
			std::vector<var>* _array;
			std::unordered_map<std::string, var>* _table;
		};
		Data _data;
		// type of data
		char _type = 0;

		static var parse_json_object(const std::string& src, size_t& i);
		static var parse_json_array(const std::string& src, size_t& i);
		static var parse_json_value(const std::string& src, size_t& i);

	public:

		enum Type
		{
			NULL_TYPE,
			ERROR_TYPE,
			INT_TYPE,
			UINT_TYPE,
			FLOAT_TYPE,
			CHAR_TYPE,
			BOOL_TYPE,
			STRING_TYPE,
			ARRAY_TYPE,
			TABLE_TYPE
		};

		var() = default;

		var(const var& other);
		var(var&& other);

		var(Type t);

		var(long long i);
		inline var(short i) : var((long long)i) {}
		inline var(int i) : var((long long)i) {}
		inline var(long i) : var((long long)i) {}

		var(unsigned long long u);
		inline var(unsigned short u): var((unsigned long long)u) {}
		inline var(unsigned int u) : var((unsigned long long )u) {}
		inline var(unsigned long u) : var((unsigned long long)u) {}
		
		var(double d);
		inline var(float f) : var((double)f) {}

		var(bool b);

		var(char c);

		var(const std::string& s, bool error = false);
		inline var(char* c) : var(std::string(c)) {}
		inline var(const char* c) : var(std::string(c)) {}

		var(const std::initializer_list<var>& list);

		~var();

		static var parse_json(const std::string& src);
		inline static var error(const std::string& msg) { return var(msg, true); }
		
		intmax_t to_int() const;
		uintmax_t to_uint() const;
		double to_double() const;
		char to_char() const;
		bool to_bool() const;
		std::string to_string() const;
		std::string to_json() const;

		inline bool contains(const std::string& key) const
		{
			return _type == TABLE_TYPE ?
				_data._table->find(key) != _data._table->end() :
				false;
		}
		bool empty() const;
		size_t size() const;

		inline bool is_null() const { return _type == NULL_TYPE; }
		inline bool is_error() const { return _type == ERROR_TYPE; }
		inline bool is_int() const { return _type == INT_TYPE; }
		inline bool is_uint() const { return _type == UINT_TYPE; }
		inline bool is_float() const { return _type == FLOAT_TYPE; }
		inline bool is_num() const { return _type == INT_TYPE || _type == UINT_TYPE || _type == FLOAT_TYPE; }
		inline bool is_bool() const { return _type == BOOL_TYPE; }
		inline bool is_char() const { return _type == CHAR_TYPE; }
		inline bool is_string() const { return _type == STRING_TYPE; }
		inline bool is_table() const { return _type == TABLE_TYPE; }
		inline bool is_array() const { return _type == ARRAY_TYPE; }

		inline Data data() const { return _data; }
		inline int type() const { return (int)_type; }

		var& operator=(const var& other);

		var& operator[](size_t i);
		inline const var& operator[](size_t i) const
		{
			return _type == ARRAY_TYPE ?
				(*_data._array)[i] :
				*this;
		}

		var& operator[](const std::string& key);
		inline const var& operator[](const std::string& key) const
		{
			return _type == TABLE_TYPE ?
				(*_data._table)[key] :
				*this;
		}

		friend std::ostream& operator<<(std::ostream& out, const var& v);
	};
}

#endif // VAR_H

#ifdef HIRZEL_VAR_IMPLEMENTATION
#undef HIRZEL_VAR_IMPLEMENTATION

namespace hirzel
{
	var::var(const var& other)
	{
		*this = other;
	}

	var::var(var&& other)
	{
		_type = other._type;
		_data = other._data;
		other._type = NULL_TYPE;
	}

	var::var(Type t)
	{
		_type = t;
		switch (t)
		{
		case NULL_TYPE:
			_data._integer = 0;
			break;
		case INT_TYPE:
			_data._integer = 0;
			break;
		case UINT_TYPE:
			_data._unsigned = 0;
			break;
		case FLOAT_TYPE:
			_data._float = 0.0;
			break;
		case CHAR_TYPE:
			_data._character = 0;
			break;
		case BOOL_TYPE:
			_data._boolean = false;
			break;
		case ERROR_TYPE:
		case STRING_TYPE:
			_data._string = new std::string();
			break;
		case ARRAY_TYPE:
			_data._array = new std::vector<var>();
			break;
		case TABLE_TYPE:
			_data._table = new std::unordered_map<std::string,var>();
			break;
		}
	}

	var::var(long long i)
	{
		_data._integer = i;
		_type = var::INT_TYPE;
	}

	var::var(long long unsigned u)
	{
		_data._unsigned = u;
		_type = var::UINT_TYPE;
	}

	var::var(char c)
	{
		_data._character = c;
		_type = var::CHAR_TYPE;
	}

	var::var(double d)
	{
		_data._float = d;
		_type = var::FLOAT_TYPE;
	}

	var::var(bool b)
	{
		_data._boolean = b;
		_type = var::BOOL_TYPE;
	}

	var::var(const std::string& s, bool error)
	{
		if (error)
			_type = ERROR_TYPE;
		else
			_type = STRING_TYPE;
		_data._string = new std::string(s);
	}

	var::var(const std::initializer_list<var>& list)
	{
		_type = ARRAY_TYPE;
		_data._array = new std::vector<var>(list);
	}

	var::~var()
	{
		switch (_type)
		{
		case ERROR_TYPE:
		case STRING_TYPE:
			delete _data._string;
			break;

		case ARRAY_TYPE:
			delete _data._array;
			break;

		case TABLE_TYPE:
			delete _data._table;
			break;
		}
	}

	intmax_t var::to_int() const
	{
		switch(_type)
		{
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
			case STRING_TYPE:
				try
				{
					return std::stoll(*_data._string);
				}
				catch(std::invalid_argument e)
				{
					return 0;
				}
			default:
				return 0;
		}
	}

	uintmax_t var::to_uint() const
	{
		switch(_type)
		{
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
			case STRING_TYPE:
				try
				{
					return std::stoull(*_data._string);
				}
				catch (std::invalid_argument e)
				{
					return 0;
				}
			default:
				return 0;
		}
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
			case STRING_TYPE:
				try
				{
					return std::stod(*_data._string);
				}
				catch(std::invalid_argument e)
				{
					return 0.0;
				}
			default:
				return 0.0;
		}
	}

	char var::to_char() const
	{
		switch(_type)
		{
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
			case STRING_TYPE:
				return (_data._string->empty() ? 0 : (*_data._string)[0]);
			default:
				return 0;
		}
	}

	bool var::to_bool() const
	{
		switch(_type)
		{
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
			case STRING_TYPE:
				return !_data._string->empty();
			default:
				return false;
		}
	}

	std::string var::to_json() const
	{
		switch(_type)
		{
		case ERROR_TYPE:
		case NULL_TYPE:
			return "null";
			break;
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
		case STRING_TYPE:
			return "\"" + *_data._string + "\"";
		case ARRAY_TYPE:
		case TABLE_TYPE:
			break;
		default:
			return "";
		}

		std::string out;

		if (_type == TABLE_TYPE)
		{
			std::vector<std::string> str_reps(_data._table->size());
			int i = 0;
			for (auto iter = _data._table->begin(); iter != _data._table->end(); iter++)
			{
				str_reps[i++] = "\"" + iter->first + "\":" + iter->second.to_json();
			}

			out = "{";
			for (i = i - 1; i >= 0; i--)
			{
				out += str_reps[i];
				if (i > 0) out += ',';
			}
			out += '}';
		}
		// ARRAY_TYPE
		else
		{
			out = "[";
			for (auto iter = _data._array->begin(); iter != _data._array->end(); iter++)
			{
				if (iter != _data._array->begin()) out += ',';
				out += iter->to_json();
			}
			out += ']';
		}

		return out;
	}

	namespace details
	{
		void indent(std::string& s)
		{
			for (int i = 1; i < s.size(); i++)
			{
				if (s[i] == '\n')
				{
					i++;
					s.insert(i, 1, '\t');
				}
			}
		}
	}

	std::string var::to_string() const
	{
		switch(_type)
		{
			case NULL_TYPE:
				return "null";
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
			case ERROR_TYPE:
			case STRING_TYPE:
				return *_data._string;
			case ARRAY_TYPE:
			case TABLE_TYPE:
				break;
			default:
				return "";
		}

		std::string out;
		std::vector<std::string> str_reps;

		if (_type == TABLE_TYPE)
		{
			if (_data._table->empty()) return "{}";
			str_reps.resize(_data._table->size());
			int i = 0;
			for (auto iter = _data._table->begin(); iter != _data._table->end(); iter++)
			{
				const var& v = iter->second;
				str_reps[i] = "\n\t" + iter->first + ":\t";
				std::string tmp = v.to_string();
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

		}
		// ARRAY_TYPE
		else
		{
			bool vert = false;
			std::vector<std::string> str_reps(_data._array->size());
			for (int i = 0; i < _data._array->size(); i++)
			{
				const var& v = (*_data._array)[i];
				std::string tmp = v.to_string();
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

			for (int i = 0; i < str_reps.size(); i++)
			{
				if (vert) out += "\n\t";
				
				details::indent(str_reps[i]);
				out += str_reps[i];
				if (i < str_reps.size() - 1) out += ',';
			}
			if (vert) out += '\n';
			out += ']';
		}

		return out;
	}

	bool var::empty() const
	{
		switch (_type)
		{
		case ERROR_TYPE:
		case STRING_TYPE:
			return _data._string->empty();
		case ARRAY_TYPE:
			return _data._array->empty();
		case TABLE_TYPE:
			return _data._table->empty();
		case NULL_TYPE:
			return true;
		default:
			return false;
		}
	}

	size_t var::size() const
	{
		switch (_type)
		{
		case INT_TYPE:
			return sizeof(_data._integer);
		case UINT_TYPE:
			return sizeof(_data._unsigned);
		case FLOAT_TYPE:
			return sizeof(_data._float);			
		case CHAR_TYPE:
			return sizeof(_data._character);
		case BOOL_TYPE:
			return sizeof(_data._boolean);
		case ERROR_TYPE:
		case STRING_TYPE:
			return _data._string->size();
		case ARRAY_TYPE:
			return _data._array->size();
		case TABLE_TYPE:
			return _data._table->size();
		default:
			return 0;
		}
	}

	var& var::operator=(const var& other)
	{
		_type = other.type();
		switch(_type)
		{
		case ARRAY_TYPE:
			_data._array = new std::vector<var>(*other.data()._array);
			break;

		case TABLE_TYPE:
			_data._table = new std::unordered_map<std::string, var>(*other.data()._table);
			break;

		case ERROR_TYPE:
		case STRING_TYPE:
			_data._string = new std::string(*other.data()._string);
			break;

		default:
			_data = other.data();
			break;
		}

		return *this;
	}

	var& var::operator[](const std::string& key)
	{
		switch (_type)
		{
		case ARRAY_TYPE:
			std::vector<var>* arr;
			arr = _data._array;
			_type = TABLE_TYPE;
			_data._table = new std::unordered_map<std::string, var>();

			for (int i = 0; i < arr->size(); i++)
			{
				(*_data._table)[std::to_string(i)] = (*arr)[i];
			}
			return (*_data._table)[key];

		case TABLE_TYPE:
			return (*_data._table)[key];

		default:
			_type = TABLE_TYPE;
			_data._table = new std::unordered_map<std::string, var>();
			return (*_data._table)[key];
		}
	}

	var& var::operator[](size_t i)
	{
		switch (_type)
		{
		case ARRAY_TYPE:
			if (i >= _data._array->size()) (*_data._array).resize(i + 1);
			return (*_data._array)[i];

		case TABLE_TYPE:
			return (*_data._table)[std::to_string(i)];

		default:
			_type = var::ARRAY_TYPE;
			_data._array = new std::vector<var>(i + 1);
			return (*_data._array)[i];
		}
	}

	std::ostream& operator<<(std::ostream& out, const var& v)
	{
		out << v.to_string();
		return out;
	}

	// Static functions

	var var::parse_json_value(const std::string& src, size_t& i)
	{
		char first = src[i];
		if (first >= '0' && first <= '9' || first == '-')
		{
			char tmp[128];
			char* pos = tmp;
			*pos++ = first;
			i++;
			bool dec = false;
			bool neg = false;
			if (first == '-') neg = true;

			while (true)
			{
				if (i == src.size()) break;
				char c = src[i];
				// not a number
				if (c == '.')
				{
					if (dec) return error("JSON: stray '.' found in number literal at position: " + std::to_string(i));
					dec = true;
				}
				else if (c < '0' || c > '9') break;
				*pos++ = c;
				i++;
			}
			*pos = 0;

			if (dec)
			{
				return var(std::stod(tmp));
			}
			else
			{
				if (neg)
					return var(std::stoll(tmp));
				else
					return var(std::stoull(tmp));
			}
		}
		else if (first == '\"')
		{
			char tmp[256];
			char* pos = tmp;
			i++;
			while (src[i] != '\"')
			{
				*pos++ = src[i++];
			}
			i++;
			*pos = 0;
			return var(tmp);
		}
		else
		{
			const char* match;
			int size;
			switch(first)
			{
			case '{':
				return parse_json_object(src, i);
			case '[':
				return parse_json_array(src, i);
			case 't':
				match = "true";
				size = 4;
				break;

			case 'f':
				match = "false";
				size = 5;
				break;

			case 'n':
				match = "null";
				size = 4;
				break;

			default:
				return error("JSON: invalid token '" + std::string(1, first) + "' at position: " + std::to_string(i));
			}

			const char* c = match + 1;
			size_t start_of_literal = i;
			i++;
			while (true)
			{	
				if (c - match == size) break;
				if (i == src.size())
				{
					return error("unexpected token '" + std::string(1, src[start_of_literal]) + "' in primitive literal at position: " + std::to_string(start_of_literal));
				}
				if (src[i++] != *c++)
				{
					return error("unexpected token '" + std::string(1, *c) + "' in primitive literal at position: " + std::to_string(i));
				}
			}

			switch (first)
			{
			case 't':
				return var(true);
			case 'f':
				return var(false);
			case 'n':
				return var();
			}
		}
		
		return var();
	}

	var var::parse_json_array(const std::string& src, size_t& i)
	{
		var arr(ARRAY_TYPE);
		int index = 0;
		i++;
		if (src[i] == ']') return arr;
		bool new_elem = true;
		while (new_elem)
		{
			var v = parse_json_value(src, i);
			if (v.is_error()) return v;
			arr[index++] = v;

			if (src[i] == ',')
				i++;
			else
				new_elem = false;
		}
		i++;
		return arr;
	}
	
	var var::parse_json_object(const std::string& src, size_t& i)
	{
		var obj(var::TABLE_TYPE);
		int index = 0;
		i++;
		bool new_member;
		if (src[i] != '}') new_member = true;
		
		while (new_member)
		{
			char label[128];
			char* pos = label;
		
			if (src[i] != '\"') return error("JSON: invalid label given for member at position: " + std::to_string(i));
			i++;
			while (src[i] != '\"')
			{
				*pos++ = src[i++];
			}
			i++;
			*pos = 0;

			if (src[i] != ':') return error("JSON: improper object member definition at position: " + std::to_string(i));
			i++;
			
			var& m = obj[label];
			m = parse_json_value(src, i);
			if (m.is_error()) return m;
			
			if (src[i] == ',')
			{
			}
			else
			{
				new_member = false;
				if (src[i] != '}') return error("JSON: unexpected token '" + std::string(1, src[i]) + "' at position: " + std::to_string(i));
			}
			i++;
		}

		return obj;
	}

	var var::parse_json(const std::string& src)
	{
		std::string src_mod(src.size(), 0);
		// removing all non vital white space
		size_t oi = 0;
		// maximum depth of 128 for json file 
		char pairs[128];
		char* pair = pairs;
		for (size_t i = 0; i < src.size(); i++)
		{
			if (src[i] < 33) continue;
			switch(src[i])
			{
			case '\"':
				src_mod[oi++] = src[i++];
				while (true)
				{
					if (i >= src.size()) return error("JSON: unterminated string at position: " + std::to_string(i - 1));
					// end of string
					if (src[i] == '\"' && src[i - 1] != '\\') break;
					src_mod[oi++] = src[i++];
				}
				break;

			case '[':
				*++pair = ']';
				break;

			case '{':
				*++pair = '}';
				break;

			case ']':
			case '}':
				if (*pair == src[i])
					pair--;
				else
					return error("JSON: stray '" + std::string(1, src[i]) + "' at position: " + std::to_string(i));
				break;
			}

			src_mod[oi++] = src[i];
		}

		src_mod.resize(oi);

		if (src_mod.empty()) return error("JSON: source string was empty");
		
		if (pair > pairs)
		{
			std::string name = *pair == ']' ? "array" : "object";
			return error("JSON: unterminated " + name + " definition");
		}

		oi = 0;

		return parse_json_value(src_mod, oi);
	}
} // namespace hirzel

#endif // HIRZEL_VAR_IMPLEMENTATION