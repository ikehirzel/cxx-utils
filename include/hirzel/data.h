
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
			long long itg = 0;
			bool bol;
			double flt;
			std::string* str;
			Array* arr;
			Table* tbl;
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

		static std::pair<const char *, bool> parse_json_number_literal(const char *iter);

		static std::string preprocess_json(const std::string& src);
		template <bool expected>
		static std::pair<Data, const char *> parse_json_bool(const char * iter);
		static std::pair<Data, const char *> parse_json_null(const char * iter);
		static std::pair<Data, const char *> parse_json_string(const char * iter);
		static std::pair<Data, const char *> parse_json_number(const char * iter);
		static std::pair<Data, const char *> parse_json_object(const char * iter);
		static std::pair<Data, const char *> parse_json_array(const char * iter);
		static std::pair<Data, const char *> parse_json_value(const char * iter);

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

		~Data();

		static Data parse_json(const std::string& src);
		
		long long as_int() const;
		double as_float() const;
		bool as_bool() const;
		std::string	as_string() const;

		inline const Array& as_array() const
		{
			if (_type != ARRAY_TYPE)
				throw TypeException("type must be an array to be cast as one");

			return *_storage.arr;
		}

		inline const Table& as_table() const
		{
			if (_type != TABLE_TYPE)
				throw TypeException("type must be a table to be cast as one");
			
			return *_storage.tbl;
		}
		
		std::string as_json() const;

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
			return _type == INT_TYPE;
		}

		inline bool is_float() const noexcept
		{
			return _type == FLOAT_TYPE;
		}

		inline bool is_num() const noexcept
		{
			return _type == INT_TYPE || _type == FLOAT_TYPE;
		}

		inline bool is_string() const noexcept
		{
			return _type == STRING_TYPE;
		}

		inline bool is_array() const noexcept
		{
			return _type == ARRAY_TYPE;
		}

		inline bool is_table() const noexcept
		{
			return _type == TABLE_TYPE;
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

		inline Data& at(size_t i)
		{
			if (_type != ARRAY_TYPE)
				throw TypeException("data must be array for integer indexing");

			if (i >= _storage.arr->size())
			{
				_storage.arr->resize(i + 1);
			}

			return (*_storage.arr)[i];
		}

		inline const Data& at(size_t i) const
		{
			if (_type != ARRAY_TYPE)
				throw TypeException("data must be array for integer indexing");

			if (i >= _storage.arr->size())
				throw std::out_of_range("attempted to access index "
					+ std::to_string(i)
					+ " but size is "
					+ std::to_string(_storage.arr->size()));

			return (*_storage.arr)[i];
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
			
			return (*_storage.tbl)[key];
		}

		inline const Data& at(const std::string& key) const
		{
			if (_type != TABLE_TYPE)
				throw TypeException("data must be a table for string indexing");

			if (_storage.tbl->find(key) == _storage.tbl->end())
				throw std::out_of_range("attempted to access element at '"
					+ key + "' but it does not exist");

			return (*_storage.tbl)[key];
		}

		inline Data& operator[](const std::string& key)
		{
			return at(key); 
		}

		inline const Data& operator[](const std::string& key) const
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
				try
				{
					return std::stoll(*_storage.str);
				}
				catch (const std::exception& e)
				{
					return 0;
				}
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
		case NULL_TYPE:
			return 0;
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

	std::string Data::preprocess_json(const std::string& src)
	{
		std::string out = src;
		auto out_iter = out.begin();

		for (auto src_iter = src.begin(); src_iter != src.end(); ++src_iter)
		{
			if (*src_iter < 33)
				continue;
			
			if (*src_iter == '\"')
			{
				while (1)
				{
					*out_iter = *src_iter;

					if (*src_iter == '\"')
					{
						out_iter += 1;
						break;
					}

					if (src_iter == src.end())
						throw ParseException("unterminated string at position: "
							+ std::to_string(src_iter - src.end()));

					out_iter += 1;
					src_iter += 1;
				}

				continue;
			}

			*out_iter = *src_iter;
			out_iter += 1;
		}

		out.resize(out_iter - out.begin());
		
		return out;
	}
	
	std::pair<const char *, bool> Data::parse_json_number_literal(const char *iter)
	{
		bool is_decimal = false;
		
		if (*iter == '-' || *iter == '+')
			iter += 1;

		while (true)
		{
			switch (*iter)
			{
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
					iter += 1;
					break;

				case '.':
					if (is_decimal)
						throw ParseException("extra '.' in number literal");

					iter += 1;
					is_decimal = true;
					break;

				default:
					return { iter, is_decimal };
			}
		}
	}

	std::pair<Data, const char *> Data::parse_json_number(const char *iter)
	{
		const char * const start_of_number = iter;
		auto res = parse_json_number_literal(iter);

		iter = res.first;

		if (*iter == 'e' || *iter == 'E')
		{
			auto exponent = parse_json_number_literal(iter);

			if (exponent.second)
				throw ParseException("stray '.' in exponent literal");

			iter = exponent.first;
		}

		std::string number_literal(start_of_number, iter - start_of_number);

		return 
		{
			res.second
				? Data(std::stod(number_literal))
				: Data(std::stoll(number_literal)),
			iter
		};
	}

	std::pair<Data, const char *> Data::parse_json_string(const char *iter)
	{
		iter += 1;

		const char * const start_of_string = iter;

		while (*iter != '\"')
		{
			if (*iter == '\0')
				throw ParseException("unterminated string: "
					+ std::string(start_of_string, iter - start_of_string));

			iter += 1;
		}

		Data out = std::string(start_of_string, iter - start_of_string);

		iter += 1;

		return { out, iter };
	}
	
	std::pair<Data, const char *> Data::parse_json_null(const char *iter)
	{
		iter += 1;

		for (const char *match = "ull"; *match; ++match)
		{
			if (*iter != *match)
				throw ParseException("unexpected character '"
					+ std::string(1, *iter)
					+ "' found in null literal");

			iter += 1;
		}

		return { Data(), iter };
	}

	template <bool expected>
	std::pair<Data, const char *> Data::parse_json_bool(const char *iter)
	{
		iter += 1;

		const char *match;

		if constexpr (expected)
			match = "rue";
		else
			match = "alse";

		while (*match)
		{
			if (*iter != *match)
				throw ParseException("unexpected character '"
					+ std::string(1, *iter)
					+ "' found in bool literal");

			iter += 1;
			match += 1;
		}
		
		return { Data(expected), iter };
	}

	std::pair<Data, const char *> Data::parse_json_value(const char *iter)
	{
		switch (*iter)
		{
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case '-':
				return parse_json_number(iter);
			case '\"':
				return parse_json_string(iter);
			case '{':
				return Data::parse_json_object(iter);
			case '[':
				return Data::parse_json_array(iter);
			case 't':
				return parse_json_bool<true>(iter);
			case 'f':
				return parse_json_bool<false>(iter);
			case 'n':
				return parse_json_null(iter);
			default:
				throw ParseException("invalid literal");
		}
	}

	std::pair<Data, const char *> Data::parse_json_array(const char *iter)
	{
		Data arr(ARRAY_TYPE);

		iter += 1;

		if (*iter == ']')
			return { arr, iter + 1 };
		size_t curr_array_index = 0;

		while (true)
		{
			auto res = parse_json_value(iter);

			arr[curr_array_index++] = res.first;
			iter = res.second;

			if (*iter != ',')
				break;
				
			iter += 1;
		}

		if (*iter != ']')
			throw ParseException("unterminated array");

		iter += 1;

		return { arr, iter };
	}
	
	std::pair<Data, const char *> Data::parse_json_object(const char *iter)
	{
		Data obj(Data::TABLE_TYPE);

		iter += 1;

		if (*iter == '}')
			return { obj, iter };
		
		while (true)
		{
			if (*iter != '\"')
				throw ParseException("invalid label");

			iter += 1;

			const char *end_of_label = iter;
			const char *start_of_label = iter;

			while (*end_of_label != '\"')
			{
				if (!end_of_label[0])
					throw ParseException("unterminated label: '"
						+ std::string(start_of_label, end_of_label - start_of_label)
						+ "'");

				end_of_label += 1;
			}

			std::string label(start_of_label, end_of_label - start_of_label);
			iter = end_of_label + 1;

			if (*iter != ':')
				throw ParseException("expected ':' but found: "
					+ std::string(iter, 1));

			iter += 1;
			
			auto res = parse_json_value(iter);
			
			obj[{ start_of_label, (size_t)(end_of_label - start_of_label) }] = res.first;
			iter = res.second;
			
			if (*iter != ',')
			{
				if (*iter != '}')
					throw ParseException("unterminated object found in JSON");
				iter += 1;
				break;
			}

			iter += 1;
		}

		return { obj, iter };
	}

	Data Data::parse_json(const std::string& src)
	{
		std::string preprocessed_src = preprocess_json(src);

		if (preprocessed_src.empty())
			throw ParseException("source string was empty");

		auto json = parse_json_value(preprocessed_src.c_str());

		return json.first;
	}
} // namespace hirzel

#endif // HIRZEL_IMPLEMENT
