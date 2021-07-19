
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
#include <string_view>

namespace hirzel
{
	class Data;

	namespace details
	{
		template <typename T>
		constexpr bool is_bool_data()
		{
			return std::is_same<T, bool>();
		}

		template <typename T>
		constexpr bool is_int_data()
		{
			return std::is_same<T, char> () || std::is_same<T, short>()
				|| std::is_same<T, int>() || std::is_same<T, long>()
				|| std::is_same<T, long long>();
		}
		
		template <typename T>
		constexpr bool is_float_data()
		{
			return std::is_same<T, float>() || std::is_same<T, double>();
		}

		template <typename T>
		constexpr bool is_string_data()
		{
			return std::is_same<T, std::string>();
		}

		template <typename T>
		constexpr bool is_array_data()
		{
			return std::is_same<T, std::vector<Data>>();
		}

		template <typename T>
		constexpr bool is_table_data()
		{
			return std::is_same<T, std::unordered_map<std::string, Data>>();
		}
	}

	class Data
	{
	public:
		typedef std::unordered_map<std::string, Data> Table;
		typedef std::vector<Data> Array;

		enum Type
		{
			NULL_TYPE,
			INT_TYPE,
			FLOAT_TYPE,
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

		class View
		{
		private:
			Data *_ptr = nullptr;
		public:
			View(Data *ptr) : _ptr(ptr) {}

			inline View operator[] (const std::string& key) const noexcept
			{
				if (!_ptr || !_ptr->is<Table>())
					return View(nullptr);

				return { &_ptr->as_table()[key] };
			}

			template <typename T>
			inline T as()
			{
				if (!_ptr)
					throw TypeException("data is nulllptr");

				return _ptr->as<T>();
			}

			inline operator bool() const noexcept 
			{ 
				return _ptr;
			}

			inline Data& operator=(const Data& data)
			{
				if (!_ptr)
					throw TypeException("data is nullptr");

				*_ptr = data;
				return *_ptr;
			}

			friend std::ostream& operator<< (std::ostream& out, const View v)
			{
				if (v)
					out << "valid reference";
				else
					out << "invalid reference";
					
				return out;
			}
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
			const char *what() const noexcept override
			{
				return _msg.c_str(); 
			}
		};

	private:

		unsigned _type = NULL_TYPE;
		Storage _storage;

		static Data parse_json_object(const std::string& src, size_t& i);
		static Data parse_json_array(const std::string& src, size_t& i);
		static Data parse_json_value(const std::string& src, size_t& i);

	public:

		Data() = default;
		Data(Data&& other);
		Data(Type t);

		inline Data(const Data& other) { *this = other; }

		inline Data(short i) : _type(INT_TYPE) { _storage.itg = i; }
		inline Data(int i) : _type(INT_TYPE) { _storage.itg = i; }
		inline Data(long i) : _type(INT_TYPE) { _storage.itg = i; }
		inline Data(long long i) : _type(INT_TYPE) { _storage.itg = i; }

		inline Data(unsigned short i) : _type(INT_TYPE) { _storage.itg = i; }
		inline Data(unsigned int i) : _type(INT_TYPE) { _storage.itg = i; }
		inline Data(unsigned long i) : _type(INT_TYPE) { _storage.itg = i; }
		inline Data(unsigned long long i) : _type(INT_TYPE) { _storage.itg = i; }
		
		inline Data(float f) : _type(FLOAT_TYPE) { _storage.flt = f; }
		inline Data(double f) : _type(FLOAT_TYPE) { _storage.flt = f; }

		inline Data(bool b) : _type(BOOL_TYPE) { _storage.bol = b; }

		inline Data(const std::string& s) : _type(STRING_TYPE) { _storage.str = new std::string(s); }
		inline Data(char* s) : _type(STRING_TYPE) { _storage.str = new std::string(s); }
		inline Data(const char* s) : _type(STRING_TYPE) { _storage.str = new std::string(s); }

		inline Data(const Array& arr) : _type(ARRAY_TYPE) { _storage.arr = new Array(arr); }
		inline Data(const Table& table) : _type(TABLE_TYPE) { _storage.tbl = new Table(table); }
		inline Data(const std::initializer_list<Data>& list) : Data(Array(list)) {}

		~Data();

		static Data parse_json(const std::string& src);
		
		long long as_int() const;
		double as_float() const;
		char as_char() const;
		bool as_bool() const;
		std::string	as_string() const;
		std::string as_json() const;

		Table& as_table();
		inline const Table& as_table() const
		{
			return as_table();
		}
		
		Array& as_array();
		inline const Array& as_array() const
		{
			return as_array();
		}

		inline bool contains(const std::string& key) const
		{
			return _type == TABLE_TYPE ?
				_storage.tbl->find(key) != _storage.tbl->end() :
				false;
		}

		template <typename T>
		inline auto as()
		{
			using namespace hirzel::details;

			if constexpr (is_bool_data<T>())
				return as_bool();
			else if constexpr (is_int_data<T>())
				return as_int();
			else if constexpr (is_float_data<T>())
				return as_float();
			else if constexpr (is_string_data<T>())
				return as_string();
			else if constexpr (is_array_data<T>())
				return as_array();
			else if constexpr (is_table_data<T>())
				return as_table();
			else
				throw TypeException("data is an invalid type");
		}

		bool is_empty() const;
		size_t size() const;

		inline bool is_null() const noexcept
		{
			return _type == NULL_TYPE;
		}

		inline bool is_bool() const noexcept
		{
			return _type == BOOL_TYPE;
		}

		inline bool is_int() const noexcept
		{
			return _type == BOOL_TYPE;
		}

		inline bool is_float() const noexcept
		{
			return _type == BOOL_TYPE;
		}

		inline bool is_num() const noexcept
		{
			return _type == INT_TYPE || _type == FLOAT_TYPE;
		}

		inline bool is_string() const noexcept
		{
			return _type == BOOL_TYPE;
		}

		inline bool is_array() const noexcept
		{
			return _type == BOOL_TYPE;
		}

		inline bool is_table() const noexcept
		{
			return _type == BOOL_TYPE;
		}

		template <typename T>
		inline bool is() const noexcept
		{
			using namespace hirzel::details;

			if constexpr (is_bool_data<T>())
				return is_bool();
			else if constexpr (is_int_data<T>())
				return _type == INT_TYPE;
			else if constexpr (is_float_data<T>())
				return _type == FLOAT_TYPE;
			else if constexpr (is_string_data<T>())
				return _type == STRING_TYPE;
			else if constexpr (is_array_data<T>())
				return _type == ARRAY_TYPE;
			else if constexpr (is_table_data<T>())
				return _type == TABLE_TYPE;
			else
				return false;
		}

		inline const Storage data() const { return _storage; }
		inline unsigned type() const { return (unsigned)_type; }

		Data& operator=(const Data& other);

		inline View at(size_t i)
		{
			if (_type != ARRAY_TYPE || i >= _storage.arr->size())
				return View(nullptr);

			return View(&((*_storage.arr)[i]));
		}

		inline const View at(size_t i) const
		{
			return at(i);
		}

		inline View operator[](size_t i)
		{
			return at(i); 
		}

		inline const View operator[](size_t i) const
		{
			return at(i); 
		}

		inline View at(const std::string& key)
		{
			if (_type != TABLE_TYPE || _storage.tbl->find(key) == _storage.tbl->end())
				return View(nullptr);

			return View(&((*_storage.tbl)[key]));
		}

		inline const View at(const std::string& key) const
		{
			return at(key);
		}

		inline View operator[](const std::string& key)
		{
			return at(key); 
		}

		inline const View operator[](const std::string& key) const
		{ 
			return at(key);
		} 

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
		case FLOAT_TYPE:
			_storage.flt = 0.0;
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

	long long Data::as_int() const
	{
		switch(_type)
		{
			case NULL_TYPE:
				return 0;
			case INT_TYPE:
				return _storage.itg;
			case BOOL_TYPE:
				return (long long)_storage.bol;
			case FLOAT_TYPE:
				return (long long)_storage.flt;
			case STRING_TYPE:
				return std::stoll(*_storage.str);
			default:
				throw TypeException("data can not be converted to integer");
		}
	}

	double Data::as_float() const
	{
		switch(_type)
		{
			case NULL_TYPE:
				return 0.0;
			case INT_TYPE:
				return (double)_storage.itg;
			case BOOL_TYPE:
				return (double)_storage.bol;
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

	bool Data::as_bool() const
	{
		switch(_type)
		{
			case INT_TYPE:
				return (bool)_storage.itg;
			case BOOL_TYPE:
				return _storage.bol;
			case FLOAT_TYPE:
				return (bool)_storage.flt;
			case STRING_TYPE:
				return !_storage.str->empty();
			default:
				return false;
		}
	}

	std::string Data::as_json() const
	{
		switch(_type)
		{
			case NULL_TYPE:
				return "null";
			case INT_TYPE:
				return std::to_string(_storage.itg);
			case BOOL_TYPE:
				return (_storage.bol ? "true" : "false");
			case FLOAT_TYPE:
				return std::to_string(_storage.flt);
			case STRING_TYPE:
				return "\"" + *_storage.str + "\"";
			case ARRAY_TYPE:
			{
				std::string out;

				out = "[";
				for (auto iter = _storage.arr->begin(); iter != _storage.arr->end(); iter++)
				{
					if (iter != _storage.arr->begin()) out += ',';
					out += iter->as_json();
				}
				out += ']';

				return out;
			}
			case TABLE_TYPE:
			{
				std::string out;

				std::vector<std::string> str_reps(_storage.tbl->size());
				int i = 0;
				for (auto iter = _storage.tbl->begin(); iter != _storage.tbl->end(); iter++)
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
				throw TypeException("data is an invalid type");
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
			case INT_TYPE:
				return std::to_string(_storage.itg);
			case BOOL_TYPE:
				return (_storage.bol ? "true" : "false");
			case FLOAT_TYPE:
				return std::to_string(_storage.flt);
			case STRING_TYPE:
				return *_storage.str;
			case ARRAY_TYPE:
			{
				std::string out;
				std::vector<std::string> str_reps(_storage.arr->size());

				bool vert = false;
				for (size_t i = 0; i < _storage.arr->size(); i++)
				{
					const Data& v = (*_storage.arr)[i];
					std::string tmp = v.as_string();
					if (v.is<std::string>())
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

				if (_storage.tbl->empty()) return "{}";
				str_reps.resize(_storage.tbl->size());
				int i = 0;
				for (auto iter = _storage.tbl->begin(); iter != _storage.tbl->end(); iter++)
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
				throw TypeException("data is not a valid type");
		}
	}

	bool Data::is_empty() const
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

	std::ostream& operator<<(std::ostream& out, const Data& v)
	{
		out << v.as_string();
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
			case FLOAT_TYPE:
				return _storage.flt == other.data().flt;
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
			bool is_decimal = false;

			while (true)
			{
				if (i == src.size()) break;
				char c = src[i];
				// not a number
				if (c == '.')
				{
					if (is_decimal)
						throw ParseException("stray '.' found in number literal at position: " + std::to_string(i));
					is_decimal = true;
				}
				else if (c < '0' || c > '9') break;
				*pos++ = c;
				i++;
			}
			*pos = 0;

			return is_decimal ? Data(std::stod(tmp)) : Data(std::stoll(tmp));
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
				arr[index++] = parse_json_value(src, i);
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
		std::string preprocessed_src = src + '\0';

		// removing all non vital white space
		size_t oi = 0;
		// maximum depth of 128 for json file 
		char pairs[128];
		char* pair = pairs;
		for (size_t i = 0; i < src.size(); i++)
		{
			if (preprocessed_src[i] < 33) continue;

			switch(preprocessed_src[i])
			{
			case '\"':
				preprocessed_src[oi++] = preprocessed_src[i++];
				while (true)
				{
					if (i >= preprocessed_src.size()) throw ParseException("unterminated string at position: " + std::to_string(i - 1));
					// end of string
					if (preprocessed_src[i] == '\"' && preprocessed_src[i - 1] != '\\') break;
					preprocessed_src[oi++] = preprocessed_src[i++];
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
				if (*pair == preprocessed_src[i])
					pair--;
				else
					throw ParseException("stray '" + std::string(1, preprocessed_src[i]) + "' at position: " + std::to_string(i));
			}

			preprocessed_src[oi++] = preprocessed_src[i];
		}

		preprocessed_src.resize(oi);

		if (preprocessed_src.empty()) throw ParseException("source string was empty");
		
		if (pair > pairs)
		{
			std::string type = (*pair == ']') ? "array" : "object";
			throw ParseException("unterminated " + type + " definition");
		}

		oi = 0;

		return parse_json_value(preprocessed_src, oi);
	}
} // namespace hirzel

#endif // HIRZEL_IMPLEMENT
