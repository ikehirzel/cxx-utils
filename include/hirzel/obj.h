
/**
 * @file obj.h
 * @brief A universal data type
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

#ifndef OBJ_H
#define OBJ_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

namespace hirzel
{
	class Obj
	{
	private:

		union Storage
		{
			bool _boolean;
			char _character;
			double _float;
			std::string* _string;
			unsigned long long _unsigned;
			long long _integer = 0;
			std::vector<Obj>* _array;
			std::unordered_map<std::string, Obj>* _table;
		};
		Storage _storage;
		// type of data
		char _type = 0;

		static Obj parse_json_object(const std::string& src, size_t& i);
		static Obj parse_json_array(const std::string& src, size_t& i);
		static Obj parse_json_value(const std::string& src, size_t& i);

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

		Obj() = default;

		Obj(const Obj& other);
		Obj(Obj&& other);

		Obj(Type t);

		Obj(long long i);
		inline Obj(short i) : Obj((long long)i) {}
		inline Obj(int i) : Obj((long long)i) {}
		inline Obj(long i) : Obj((long long)i) {}

		Obj(unsigned long long u);
		inline Obj(unsigned short u): Obj((unsigned long long)u) {}
		inline Obj(unsigned int u) : Obj((unsigned long long )u) {}
		inline Obj(unsigned long u) : Obj((unsigned long long)u) {}
		
		Obj(double d);
		inline Obj(float f) : Obj((double)f) {}

		Obj(bool b);

		Obj(char c);

		Obj(const std::string& s, bool error = false);
		inline Obj(char* c) : Obj(std::string(c)) {}
		inline Obj(const char* c) : Obj(std::string(c)) {}

		Obj(const std::initializer_list<Obj>& list);

		~Obj();

		static Obj parse_json(const std::string& src);
		inline static Obj error(const std::string& msg) { return Obj(msg, true); }
		
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
				_storage._table->find(key) != _storage._table->end() :
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

		inline Storage data() const { return _storage; }
		inline int type() const { return (int)_type; }

		Obj& operator=(const Obj& other);

		Obj& operator[](size_t i);
		inline const Obj& operator[](size_t i) const
		{
			return _type == ARRAY_TYPE ?
				(*_storage._array)[i] :
				*this;
		}

		Obj& operator[](const std::string& key);
		inline const Obj& operator[](const std::string& key) const
		{
			return _type == TABLE_TYPE ?
				(*_storage._table)[key] :
				*this;
		}

		friend std::ostream& operator<<(std::ostream& out, const Obj& v);
	};
}

#endif // OBJ_H

#ifdef HIRZEL_OBJ_IMPLEMENTATION
#undef HIRZEL_OBJ_IMPLEMENTATION

namespace hirzel
{
	Obj::Obj(const Obj& other)
	{
		*this = other;
	}

	Obj::Obj(Obj&& other)
	{
		_type = other._type;
		_storage = other._storage;
		other._type = NULL_TYPE;
	}

	Obj::Obj(Type t)
	{
		_type = t;
		switch (t)
		{
		case NULL_TYPE:
			_storage._integer = 0;
			break;
		case INT_TYPE:
			_storage._integer = 0;
			break;
		case UINT_TYPE:
			_storage._unsigned = 0;
			break;
		case FLOAT_TYPE:
			_storage._float = 0.0;
			break;
		case CHAR_TYPE:
			_storage._character = 0;
			break;
		case BOOL_TYPE:
			_storage._boolean = false;
			break;
		case ERROR_TYPE:
		case STRING_TYPE:
			_storage._string = new std::string();
			break;
		case ARRAY_TYPE:
			_storage._array = new std::vector<Obj>();
			break;
		case TABLE_TYPE:
			_storage._table = new std::unordered_map<std::string,Obj>();
			break;
		}
	}

	Obj::Obj(long long i)
	{
		_storage._integer = i;
		_type = Obj::INT_TYPE;
	}

	Obj::Obj(long long unsigned u)
	{
		_storage._unsigned = u;
		_type = Obj::UINT_TYPE;
	}

	Obj::Obj(char c)
	{
		_storage._character = c;
		_type = Obj::CHAR_TYPE;
	}

	Obj::Obj(double d)
	{
		_storage._float = d;
		_type = Obj::FLOAT_TYPE;
	}

	Obj::Obj(bool b)
	{
		_storage._boolean = b;
		_type = Obj::BOOL_TYPE;
	}

	Obj::Obj(const std::string& s, bool error)
	{
		if (error)
			_type = ERROR_TYPE;
		else
			_type = STRING_TYPE;
		_storage._string = new std::string(s);
	}

	Obj::Obj(const std::initializer_list<Obj>& list)
	{
		_type = ARRAY_TYPE;
		_storage._array = new std::vector<Obj>(list);
	}

	Obj::~Obj()
	{
		switch (_type)
		{
		case ERROR_TYPE:
		case STRING_TYPE:
			delete _storage._string;
			break;

		case ARRAY_TYPE:
			delete _storage._array;
			break;

		case TABLE_TYPE:
			delete _storage._table;
			break;
		}
	}

	intmax_t Obj::to_int() const
	{
		switch(_type)
		{
			case INT_TYPE:
				return _storage._integer;
			case UINT_TYPE:
				return (intmax_t)_storage._unsigned;
			case BOOL_TYPE:
				return (intmax_t)_storage._boolean;
			case CHAR_TYPE:
				return (intmax_t)_storage._character;
			case FLOAT_TYPE:
				return (intmax_t)_storage._float;
			case STRING_TYPE:
				try
				{
					return std::stoll(*_storage._string);
				}
				catch(std::invalid_argument e)
				{
					return 0;
				}
			default:
				return 0;
		}
	}

	uintmax_t Obj::to_uint() const
	{
		switch(_type)
		{
			case INT_TYPE:
				return (uintmax_t)_storage._integer;
			case UINT_TYPE:
				return (uintmax_t)_storage._unsigned;
			case BOOL_TYPE:
				return (uintmax_t)_storage._boolean;
			case CHAR_TYPE:
				return (uintmax_t)_storage._character;
			case FLOAT_TYPE:
				return (uintmax_t)_storage._float;
			case STRING_TYPE:
				try
				{
					return std::stoull(*_storage._string);
				}
				catch (std::invalid_argument e)
				{
					return 0;
				}
			default:
				return 0;
		}
	}

	double Obj::to_double() const
	{
		switch(_type)
		{
			case NULL_TYPE:
				return 0.0;
			case INT_TYPE:
				return (double)_storage._integer;
			case UINT_TYPE:
				return (double)_storage._unsigned;
			case BOOL_TYPE:
				return (double)_storage._boolean;
			case CHAR_TYPE:
				return (double)_storage._character;
			case FLOAT_TYPE:
				return _storage._float;
			case STRING_TYPE:
				try
				{
					return std::stod(*_storage._string);
				}
				catch(std::invalid_argument e)
				{
					return 0.0;
				}
			default:
				return 0.0;
		}
	}

	char Obj::to_char() const
	{
		switch(_type)
		{
			case INT_TYPE:
				return (char)_storage._integer;
			case UINT_TYPE:
				return (char)_storage._unsigned;
			case BOOL_TYPE:
				return (char)_storage._boolean;
			case CHAR_TYPE:
				return (char)_storage._character;
			case FLOAT_TYPE:
				return (char)_storage._float;
			case STRING_TYPE:
				return (_storage._string->empty() ? 0 : (*_storage._string)[0]);
			default:
				return 0;
		}
	}

	bool Obj::to_bool() const
	{
		switch(_type)
		{
			case INT_TYPE:
				return (bool)_storage._integer;
			case UINT_TYPE:
				return (bool)_storage._unsigned;
			case BOOL_TYPE:
				return _storage._boolean;
			case CHAR_TYPE:
				return (bool)_storage._character;
			case FLOAT_TYPE:
				return (bool)_storage._float;
			case STRING_TYPE:
				return !_storage._string->empty();
			default:
				return false;
		}
	}

	std::string Obj::to_json() const
	{
		switch(_type)
		{
		case ERROR_TYPE:
		case NULL_TYPE:
			return "null";
			break;
		case INT_TYPE:
			return std::to_string(_storage._integer);
		case UINT_TYPE:
			return std::to_string(_storage._unsigned);
		case BOOL_TYPE:
			return (_storage._boolean ? "true" : "false");
		case CHAR_TYPE:
			return std::string(1, _storage._character);
		case FLOAT_TYPE:
			return std::to_string(_storage._float);
		case STRING_TYPE:
			return "\"" + *_storage._string + "\"";
		case ARRAY_TYPE:
		case TABLE_TYPE:
			break;
		default:
			return "";
		}

		std::string out;

		if (_type == TABLE_TYPE)
		{
			std::vector<std::string> str_reps(_storage._table->size());
			int i = 0;
			for (auto iter = _storage._table->begin(); iter != _storage._table->end(); iter++)
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
			for (auto iter = _storage._array->begin(); iter != _storage._array->end(); iter++)
			{
				if (iter != _storage._array->begin()) out += ',';
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

	std::string Obj::to_string() const
	{
		switch(_type)
		{
			case NULL_TYPE:
				return "null";
			case INT_TYPE:
				return std::to_string(_storage._integer);
			case UINT_TYPE:
				return std::to_string(_storage._unsigned);
			case BOOL_TYPE:
				return (_storage._boolean ? "true" : "false");
			case CHAR_TYPE:
				return std::string(1, _storage._character);
			case FLOAT_TYPE:
				return std::to_string(_storage._float);
			case ERROR_TYPE:
			case STRING_TYPE:
				return *_storage._string;
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
			if (_storage._table->empty()) return "{}";
			str_reps.resize(_storage._table->size());
			int i = 0;
			for (auto iter = _storage._table->begin(); iter != _storage._table->end(); iter++)
			{
				const Obj& v = iter->second;
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
			std::vector<std::string> str_reps(_storage._array->size());
			for (int i = 0; i < _storage._array->size(); i++)
			{
				const Obj& v = (*_storage._array)[i];
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

	bool Obj::empty() const
	{
		switch (_type)
		{
		case ERROR_TYPE:
		case STRING_TYPE:
			return _storage._string->empty();
		case ARRAY_TYPE:
			return _storage._array->empty();
		case TABLE_TYPE:
			return _storage._table->empty();
		case NULL_TYPE:
			return true;
		default:
			return false;
		}
	}

	size_t Obj::size() const
	{
		switch (_type)
		{
		case INT_TYPE:
			return sizeof(_storage._integer);
		case UINT_TYPE:
			return sizeof(_storage._unsigned);
		case FLOAT_TYPE:
			return sizeof(_storage._float);			
		case CHAR_TYPE:
			return sizeof(_storage._character);
		case BOOL_TYPE:
			return sizeof(_storage._boolean);
		case ERROR_TYPE:
		case STRING_TYPE:
			return _storage._string->size();
		case ARRAY_TYPE:
			return _storage._array->size();
		case TABLE_TYPE:
			return _storage._table->size();
		default:
			return 0;
		}
	}

	Obj& Obj::operator=(const Obj& other)
	{
		_type = other.type();
		switch(_type)
		{
		case ARRAY_TYPE:
			_storage._array = new std::vector<Obj>(*other.data()._array);
			break;

		case TABLE_TYPE:
			_storage._table = new std::unordered_map<std::string, Obj>(*other.data()._table);
			break;

		case ERROR_TYPE:
		case STRING_TYPE:
			_storage._string = new std::string(*other.data()._string);
			break;

		default:
			_storage = other.data();
			break;
		}

		return *this;
	}

	Obj& Obj::operator[](const std::string& key)
	{
		switch (_type)
		{
		case ARRAY_TYPE:
			std::vector<Obj>* arr;
			arr = _storage._array;
			_type = TABLE_TYPE;
			_storage._table = new std::unordered_map<std::string, Obj>();

			for (int i = 0; i < arr->size(); i++)
			{
				(*_storage._table)[std::to_string(i)] = (*arr)[i];
			}
			return (*_storage._table)[key];

		case TABLE_TYPE:
			return (*_storage._table)[key];

		default:
			_type = TABLE_TYPE;
			_storage._table = new std::unordered_map<std::string, Obj>();
			return (*_storage._table)[key];
		}
	}

	Obj& Obj::operator[](size_t i)
	{
		switch (_type)
		{
		case ARRAY_TYPE:
			if (i >= _storage._array->size()) (*_storage._array).resize(i + 1);
			return (*_storage._array)[i];

		case TABLE_TYPE:
			return (*_storage._table)[std::to_string(i)];

		default:
			_type = Obj::ARRAY_TYPE;
			_storage._array = new std::vector<Obj>(i + 1);
			return (*_storage._array)[i];
		}
	}

	std::ostream& operator<<(std::ostream& out, const Obj& v)
	{
		out << v.to_string();
		return out;
	}

	// Static functions

	Obj Obj::parse_json_value(const std::string& src, size_t& i)
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
				return Obj(std::stod(tmp));
			}
			else
			{
				if (neg)
					return Obj(std::stoll(tmp));
				else
					return Obj(std::stoull(tmp));
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
			return Obj(tmp);
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
				return Obj(true);
			case 'f':
				return Obj(false);
			case 'n':
				return Obj();
			}
		}
		
		return Obj();
	}

	Obj Obj::parse_json_array(const std::string& src, size_t& i)
	{
		Obj arr(ARRAY_TYPE);
		int index = 0;
		i++;
		if (src[i] == ']') return arr;
		bool new_elem = true;
		while (new_elem)
		{
			Obj v = parse_json_value(src, i);
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
	
	Obj Obj::parse_json_object(const std::string& src, size_t& i)
	{
		Obj obj(Obj::TABLE_TYPE);
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
			
			Obj& m = obj[label];
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

	Obj Obj::parse_json(const std::string& src)
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

#endif // HIRZEL_OBJ_IMPLEMENTATION