
/**
 * @file data.h
 * @brief A universal data type
 * @author Ike Hirzel
 * 
 * Copyright 2020 Ike Hirzel
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

#ifndef HIRZEL_DATA_H
#define HIRZEL_DATA_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

namespace hirzel
{
	class Data
	{
	public:
		typedef std::unordered_map<std::string, Data> Table;
		typedef std::vector<Data> Array;

		enum Type
		{
			NULL_TYPE,
			INT_TYPE,
			UINT_TYPE,
			FLOAT_TYPE,
			CHAR_TYPE,
			BOOL_TYPE,
			STRING_TYPE,
			ARRAY_TYPE,
			TABLE_TYPE
		};

		union Storage
		{
			bool bol;
			char chr;
			double flt;
			std::string* str;
			unsigned long long uns;
			long long itg = 0;
			Array* arr;
			Table* tbl;
		};


		class ParseException : public std::exception
		{
		private:
			std::string _msg;
		public:
			ParseException(const std::string& msg) : _msg(msg) {}
			const char *what() const noexcept override { return _msg.c_str(); }
		};


	private:
		unsigned _type = NULL_TYPE;
		Storage _storage;

		static Data parse_json_object(const std::string& src, size_t& i);
		static Data parse_json_array(const std::string& src, size_t& i);
		static Data parse_json_value(const std::string& src, size_t& i);

	public:

		Data() = default;

		Data(const Data& other) { *this = other; }
		Data(Data&& other);

		Data(Type t);

		inline Data(long long i) : _type(INT_TYPE) { _storage.itg = i; }
		inline Data(short i) : _type(INT_TYPE) { _storage.itg = i; }
		inline Data(int i) : _type(INT_TYPE) { _storage.itg = i; }
		inline Data(long i) : _type(INT_TYPE) { _storage.itg = i; }

		inline Data(unsigned long long u) : _type(UINT_TYPE) { _storage.uns = u; }
		inline Data(unsigned short u) : _type(UINT_TYPE) { _storage.uns = u; }
		inline Data(unsigned int u) : _type(UINT_TYPE) { _storage.uns = u; }
		inline Data(unsigned long u) : _type(UINT_TYPE) { _storage.uns = u; }
		
		inline Data(double f) : _type(FLOAT_TYPE) { _storage.flt = f; }
		inline Data(float f) : _type(FLOAT_TYPE) { _storage.flt = f; }

		inline Data(bool b) : _type(BOOL_TYPE) { _storage.bol = b; }

		inline Data(char c) : _type(CHAR_TYPE) { _storage.chr = c; }

		inline Data(const std::string& s) : _type(STRING_TYPE) { _storage.str = new std::string(s); }
		inline Data(char* s) : _type(STRING_TYPE) { _storage.str = new std::string(s); }
		inline Data(const char* s) : _type(STRING_TYPE) { _storage.str = new std::string(s); }

		inline Data(const Array& arr) : _type(ARRAY_TYPE) { _storage.arr = new Array(arr); }
		inline Data(const Table& table) : _type(TABLE_TYPE) { _storage.tbl = new Table(table); }
		inline Data(const std::initializer_list<Data>& list) : Data(Array(list)) {}

		~Data();

		static Data parse_json(const std::string& src);
		
		long long to_int() const;
		unsigned long long to_uint() const;
		double to_double() const;
		char to_char() const;
		bool to_bool() const;
		std::string to_string() const;
		std::string to_json() const;
		Table to_table() const;
		Array to_array() const;

		inline bool contains(const std::string& key) const
		{
			return _type == TABLE_TYPE ?
				_storage.tbl->find(key) != _storage.tbl->end() :
				false;
		}
		bool empty() const;
		size_t size() const;

		inline bool is_null() const { return _type == NULL_TYPE; }
		inline bool is_int() const { return _type == INT_TYPE; }
		inline bool is_uint() const { return _type == UINT_TYPE; }
		inline bool is_float() const { return _type == FLOAT_TYPE; }
		inline bool is_num() const
		{
			return _type == INT_TYPE || _type == UINT_TYPE || _type == FLOAT_TYPE;
		}
		inline bool is_bool() const { return _type == BOOL_TYPE; }
		inline bool is_char() const { return _type == CHAR_TYPE; }
		inline bool is_string() const { return _type == STRING_TYPE; }
		inline bool is_table() const { return _type == TABLE_TYPE; }
		inline bool is_array() const { return _type == ARRAY_TYPE; }

		inline Storage data() const { return _storage; }
		inline unsigned type() const { return (unsigned)_type; }

		Data& operator=(const Data& other);

		Data& operator[](size_t i);
		const Data& at(size_t i) const;
		inline const Data& operator[](size_t i) const { return this->at(i); }

		Data& operator[](const std::string& key);
		const Data& at(const std::string& key) const;
		inline const Data& operator[](const std::string& key) const { return this->at(key); }

		friend std::ostream& operator<<(std::ostream& out, const Data& v);

		bool operator==(const Data& other) const;
		inline bool operator!=(const Data& other) const { return !(*this == other); }
	};
}

#endif // HIRZEL_DATA_H

#ifdef HIRZEL_IMPLEMENT

namespace hirzel
{
	Data::Data(Data&& other)
	{
		_type = other._type;
		_storage = other._storage;
		other._type = NULL_TYPE;
	}

	Data::Data(Type t)
	{
		_type = t;
		switch (t)
		{
		case NULL_TYPE:
			_storage.itg = 0;
			break;
		case INT_TYPE:
			_storage.itg = 0;
			break;
		case UINT_TYPE:
			_storage.uns = 0;
			break;
		case FLOAT_TYPE:
			_storage.flt = 0.0;
			break;
		case CHAR_TYPE:
			_storage.chr = 0;
			break;
		case BOOL_TYPE:
			_storage.bol = false;
			break;
		case STRING_TYPE:
			_storage.str = new std::string();
			break;
		case ARRAY_TYPE:
			_storage.arr = new Array();
			break;
		case TABLE_TYPE:
			_storage.tbl = new Table();
			break;
		}
	}

	Data::~Data()
	{
		switch (_type)
		{
		case STRING_TYPE:
			delete _storage.str;
			break;

		case ARRAY_TYPE:
			delete _storage.arr;
			break;

		case TABLE_TYPE:
			delete _storage.tbl;
			break;
		}
	}


	long long Data::to_int() const
	{
		switch(_type)
		{
			case INT_TYPE:
				return _storage.itg;
			case UINT_TYPE:
				return (long long)_storage.uns;
			case BOOL_TYPE:
				return (long long)_storage.bol;
			case CHAR_TYPE:
				return (long long)_storage.chr;
			case FLOAT_TYPE:
				return (long long)_storage.flt;
			case STRING_TYPE:
				try
				{
					return std::stoll(*_storage.str);
				}
				catch(const std::invalid_argument& e)
				{
					return 0;
				}
			default:
				return 0;
		}
	}


	unsigned long long Data::to_uint() const
	{
		switch(_type)
		{
			case INT_TYPE:
				return (unsigned long long)_storage.itg;
			case UINT_TYPE:
				return (unsigned long long)_storage.uns;
			case BOOL_TYPE:
				return (unsigned long long)_storage.bol;
			case CHAR_TYPE:
				return (unsigned long long)_storage.chr;
			case FLOAT_TYPE:
				return (unsigned long long)_storage.flt;
			case STRING_TYPE:
				try
				{
					return std::stoull(*_storage.str);
				}
				catch (const std::invalid_argument& e)
				{
					return 0;
				}
			default:
				return 0;
		}
	}


	double Data::to_double() const
	{
		switch(_type)
		{
			case NULL_TYPE:
				return 0.0;
			case INT_TYPE:
				return (double)_storage.itg;
			case UINT_TYPE:
				return (double)_storage.uns;
			case BOOL_TYPE:
				return (double)_storage.bol;
			case CHAR_TYPE:
				return (double)_storage.chr;
			case FLOAT_TYPE:
				return _storage.flt;
			case STRING_TYPE:
				try
				{
					return std::stod(*_storage.str);
				}
				catch(const std::invalid_argument& e)
				{
					return 0.0;
				}
			default:
				return 0.0;
		}
	}


	char Data::to_char() const
	{
		switch(_type)
		{
			case INT_TYPE:
				return (char)_storage.itg;
			case UINT_TYPE:
				return (char)_storage.uns;
			case BOOL_TYPE:
				return (char)_storage.bol;
			case CHAR_TYPE:
				return (char)_storage.chr;
			case FLOAT_TYPE:
				return (char)_storage.flt;
			case STRING_TYPE:
				return (_storage.str->empty() ? 0 : (*_storage.str)[0]);
			default:
				return 0;
		}
	}

	bool Data::to_bool() const
	{
		switch(_type)
		{
			case INT_TYPE:
				return (bool)_storage.itg;
			case UINT_TYPE:
				return (bool)_storage.uns;
			case BOOL_TYPE:
				return _storage.bol;
			case CHAR_TYPE:
				return (bool)_storage.chr;
			case FLOAT_TYPE:
				return (bool)_storage.flt;
			case STRING_TYPE:
				return !_storage.str->empty();
			default:
				return false;
		}
	}

	std::string Data::to_json() const
	{
		switch(_type)
		{
		case NULL_TYPE:
			return "null";
			break;
		case INT_TYPE:
			return std::to_string(_storage.itg);
		case UINT_TYPE:
			return std::to_string(_storage.uns);
		case BOOL_TYPE:
			return (_storage.bol ? "true" : "false");
		case CHAR_TYPE:
			return std::string(1, _storage.chr);
		case FLOAT_TYPE:
			return std::to_string(_storage.flt);
		case STRING_TYPE:
			return "\"" + *_storage.str + "\"";
		case ARRAY_TYPE:
		case TABLE_TYPE:
			break;
		default:
			return "";
		}

		std::string out;

		if (_type == TABLE_TYPE)
		{
			std::vector<std::string> str_reps(_storage.tbl->size());
			int i = 0;
			for (auto iter = _storage.tbl->begin(); iter != _storage.tbl->end(); iter++)
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
			for (auto iter = _storage.arr->begin(); iter != _storage.arr->end(); iter++)
			{
				if (iter != _storage.arr->begin()) out += ',';
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

	std::string Data::to_string() const
	{
		switch(_type)
		{
			case NULL_TYPE:
				return "null";
			case INT_TYPE:
				return std::to_string(_storage.itg);
			case UINT_TYPE:
				return std::to_string(_storage.uns);
			case BOOL_TYPE:
				return (_storage.bol ? "true" : "false");
			case CHAR_TYPE:
				return std::string(1, _storage.chr);
			case FLOAT_TYPE:
				return std::to_string(_storage.flt);
			case STRING_TYPE:
				return *_storage.str;
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
			if (_storage.tbl->empty()) return "{}";
			str_reps.resize(_storage.tbl->size());
			int i = 0;
			for (auto iter = _storage.tbl->begin(); iter != _storage.tbl->end(); iter++)
			{
				const Data& v = iter->second;
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
			std::vector<std::string> str_reps(_storage.arr->size());
			for (size_t i = 0; i < _storage.arr->size(); i++)
			{
				const Data& v = (*_storage.arr)[i];
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
		}

		return out;
	}

	Data::Table Data::to_table() const
	{
		Table t;
		switch (_type)
		{
		case ARRAY_TYPE:
			for (unsigned i = 0; i < _storage.arr->size(); ++i)
			{
				t[std::to_string(i)] = (*_storage.arr)[i];
			}
			return t;

		case TABLE_TYPE:
			return *_storage.tbl;

		default:
			return t;
		}
	}

	Data::Array Data::to_array() const
	{
		Array arr;
		switch (_type)
		{
		case ARRAY_TYPE:
			return *_storage.arr;

		case TABLE_TYPE:
			unsigned i;
			i = 0;
			arr.resize(_storage.tbl->size());
			for (auto p : *_storage.tbl)
			{
				arr[i++] = { p.first, p.second };
			}
			return arr;

		default:
			return arr;
		}
	}

	bool Data::empty() const
	{
		switch (_type)
		{
		case STRING_TYPE:
			return _storage.str->empty();
		case ARRAY_TYPE:
			return _storage.arr->empty();
		case TABLE_TYPE:
			return _storage.tbl->empty();
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
		case STRING_TYPE:
			return _storage.str->size();
		case ARRAY_TYPE:
			return _storage.arr->size();
		case TABLE_TYPE:
			return _storage.tbl->size();
		default:
			return 1;
		}
	}

	Data& Data::operator=(const Data& other)
	{
		_type = other.type();
		switch(_type)
		{
		case ARRAY_TYPE:
			_storage.arr = new Array(*other.data().arr);
			break;

		case TABLE_TYPE:
			_storage.tbl = new Table(*other.data().tbl);
			break;

		case STRING_TYPE:
			_storage.str = new std::string(*other.data().str);
			break;

		default:
			_storage = other.data();
			break;
		}

		return *this;
	}

	const Data& Data::at(size_t i) const
	{
		switch (_type)
		{
		case ARRAY_TYPE:
			return (*_storage.arr)[i];
		case TABLE_TYPE:
			return (*_storage.tbl)[std::to_string(i)];
		default:
			return *this;
		}
	}

	const Data& Data::at(const std::string& key) const
	{
		if (_type == TABLE_TYPE) return (*_storage.tbl)[key];
		return *this;
	}

	Data& Data::operator[](const std::string& key)
	{
		switch (_type)
		{
		case ARRAY_TYPE:
			Array* arr;
			arr = _storage.arr;
			_type = TABLE_TYPE;
			_storage.tbl = new Table();

			for (size_t i = 0; i < arr->size(); i++)
			{
				(*_storage.tbl)[std::to_string(i)] = (*arr)[i];
			}
			delete arr;
			return (*_storage.tbl)[key];

		case TABLE_TYPE:
			return (*_storage.tbl)[key];

		default:
			_type = TABLE_TYPE;
			_storage.tbl = new Table();
			return (*_storage.tbl)[key];
		}
	}

	Data& Data::operator[](size_t i)
	{
		switch (_type)
		{
		case ARRAY_TYPE:
			if (i >= _storage.arr->size()) (*_storage.arr).resize(i + 1);
			return (*_storage.arr)[i];

		case TABLE_TYPE:
			return (*_storage.tbl)[std::to_string(i)];

		default:
			_type = Data::ARRAY_TYPE;
			_storage.arr = new Array(i + 1);
			return (*_storage.arr)[i];
		}
	}

	std::ostream& operator<<(std::ostream& out, const Data& v)
	{
		out << v.to_string();
		return out;
	}

	bool Data::operator==(const Data& other) const
	{
		if (_type != other.type()) return false;

		switch (_type)
		{
		case NULL_TYPE:
			return true;

		case INT_TYPE:
			return _storage.itg == other.data().itg;

		case UINT_TYPE:
			return _storage.uns == other.data().uns;

		case FLOAT_TYPE:
			return _storage.flt == other.data().flt;

		case CHAR_TYPE:
			return _storage.chr == other.data().chr;

		case BOOL_TYPE:
			return _storage.bol == other.data().bol;

		case STRING_TYPE:
			return *_storage.str == *other.data().str;

		case ARRAY_TYPE:
			{
				// for every child element
				const std::vector<Data>& arr = *_storage.arr;
				const std::vector<Data>& oarr = *other.data().arr;

				for (size_t i = 0; i < arr.size(); ++i)
				{
					if (arr[i] != oarr[i]) return false;
				}

				return true;
			}

		case TABLE_TYPE:
			{
				const std::unordered_map<std::string, Data>& table = *_storage.tbl;
				const std::unordered_map<std::string, Data>& otable = *other.data().tbl;
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

	/*#################################################
	#	Parsing code
	###################################################*/

	Data Data::parse_json_value(const std::string& src, size_t& i)
	{
		char first = src[i];
		if ((first >= '0' && first <= '9') || first == '-')
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
					if (dec) throw ParseException("stray '.' found in number literal at position: " + std::to_string(i));
					dec = true;
				}
				else if (c < '0' || c > '9') break;
				*pos++ = c;
				i++;
			}
			*pos = 0;

			if (dec)
			{
				return Data(std::stod(tmp));
			}
			else
			{
				if (neg)
					return Data(std::stoll(tmp));
				else
					return Data(std::stoull(tmp));
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
			return Data(tmp);
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
				throw ParseException("invalid token '" + std::string(1, first) + "' at position: " + std::to_string(i));
			}

			const char* c = match + 1;
			size_t start_of_literal = i;
			i++;
			while (true)
			{	
				if (c - match == size) break;
				if (i == src.size())
					throw ParseException("unexpected token '" + std::string(1, src[start_of_literal]) + "' in primitive literal at position: " + std::to_string(start_of_literal));
				
				if (src[i++] != *c++)
					throw ParseException("unexpected token '" + std::string(1, *c) + "' in primitive literal at position: " + std::to_string(i));
			}

			switch (first)
			{
			case 't':
				return Data(true);
			case 'f':
				return Data(false);
			case 'n':
				return Data();
			}
		}
		
		return Data();
	}

	Data Data::parse_json_array(const std::string& src, size_t& i)
	{
		Data arr(ARRAY_TYPE);
		int index = 0;
		i++;
		if (src[i] == ']') return arr;
		bool new_elem = true;
		while (new_elem)
		{
			try
			{
				arr[index++] = parse_json_value(src, i);;
			}
			catch (const ParseException& e)
			{
				throw;
			}

			if (src[i] == ',')
				i++;
			else
				new_elem = false;
		}
		i++;
		return arr;
	}
	
	Data Data::parse_json_object(const std::string& src, size_t& i)
	{
		Data obj(Data::TABLE_TYPE);
		i++;
		bool new_member = false;
		if (src[i] != '}') new_member = true;
		
		while (new_member)
		{
			char label[128];
			char* pos = label;
		
			if (src[i] != '\"') throw ParseException("invalid label given for member at position: " + std::to_string(i));
			i++;
			while (src[i] != '\"')
			{
				*pos++ = src[i++];
			}
			i++;
			*pos = 0;

			if (src[i] != ':') throw ParseException("improper object member definition at position: " + std::to_string(i));
			i++;
			
			try
			{
				obj[label] = parse_json_value(src, i);
			}
			catch (const ParseException& e)
			{
				throw;
			}
			
			if (src[i] == ',')
			{
			}
			else
			{
				new_member = false;
				if (src[i] != '}') throw ParseException("unexpected token '" + std::string(1, src[i]) + "' at position: " + std::to_string(i));
			}
			i++;
		}

		return obj;
	}

	Data Data::parse_json(const std::string& src)
	{
		std::string prep = src + '\0';

		// removing all non vital white space
		size_t oi = 0;
		// maximum depth of 128 for json file 
		char pairs[128];
		char* pair = pairs;
		for (size_t i = 0; i < src.size(); i++)
		{
			if (prep[i] < 33) continue;
			switch(prep[i])
			{
			case '\"':
				prep[oi++] = prep[i++];
				while (true)
				{
					if (i >= prep.size()) throw ParseException("unterminated string at position: " + std::to_string(i - 1));
					// end of string
					if (prep[i] == '\"' && prep[i - 1] != '\\') break;
					prep[oi++] = prep[i++];
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
				if (*pair == prep[i])
					pair--;
				else
					throw ParseException("stray '" + std::string(1, prep[i]) + "' at position: " + std::to_string(i));
			}

			prep[oi++] = prep[i];
		}

		prep.resize(oi);

		if (prep.empty()) throw ParseException("source string was empty");
		
		if (pair > pairs)
		{
			std::string type = (*pair == ']') ? "array" : "object";
			throw ParseException("JSON: unterminated " + type + " definition");
		}

		oi = 0;

		return parse_json_value(prep, oi);
	}
} // namespace hirzel

#endif // HIRZEL_IMPLEMENT
