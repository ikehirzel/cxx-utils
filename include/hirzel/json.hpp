#ifndef HIRZEL_DATA_JSON_HPP
#define HIRZEL_DATA_JSON_HPP

/**
 * @file json.hpp
 * @brief Json de/serialization utilities for hirzel::data::Data
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
#include <hirzel/data.hpp>

namespace hirzel
{
	class Json
	{
	public:

		class SyntaxError : public std::exception
		{
		private:
			std::string _msg;
		public:
			SyntaxError(const std::string& msg) : _msg(msg) {}
			const char *what() const noexcept { return _msg.c_str(); }
		};

	private:

		static std::string preprocess(const std::string& src);
		static bool parseNumberLiteral(const char *& iter);
		static Data parseNumber(const char *& iter);
		static Data parseString(const char *& iter);
		static Data parseNull(const char *& iter);
		static Data parseBool(const char *& iter, bool expected);
		static Data parseArray(const char *& iter);
		static Data parseObject(const char *& iter);
		static Data parseValue(const char *& iter);

		static std::string serialize_array(const Data& data);
		static std::string serialize_object(const Data& data);
		static std::string serialize_value(const Data& data);

	public:

		static Data deserialize(const std::string& json);
		static std::string serialize(const Data& data)
		{
			return serialize_value(data);
		}
	};
}

#endif

#if !defined(HIRZEL_DATA_JSON_I) && defined(HIRZEL_IMPLEMENT)
#define HIRZEL_DATA_JSON_I

namespace hirzel
{
	std::string Json::preprocess(const std::string& src)
	{
		const char *iter = src.c_str();
		std::string out(src.length(), '\0');
		size_t oi = 0;

		while (*iter)
		{
			if (*iter <= ' ')
			{
				iter += 1;
				continue;
			}
		
			if (*iter == '\"')
			{
				const char *start_of_string = iter;

				out[oi++] = *iter;
				iter += 1;

				while (true)
				{
					if (*iter == '\0')
					{
						size_t size_of_preview = (iter - start_of_string > 15 ? 15 : iter - start_of_string);
						throw SyntaxError("unterminated string starting with: "
							+ std::string(start_of_string, size_of_preview));
					}

					if (*iter == '\"' && iter[-1] != '\\')
						break;

					out[oi++] = *iter;
					iter += 1;
				}
			}

			out[oi++] = *iter;
			iter += 1;
		}

		out.resize(oi);
		
		return out;
	}

	bool Json::parseNumberLiteral(const char *& iter)
	{
		bool isFloat = false;
		bool expecting_digit = true;
		
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
					expecting_digit = false;
					iter += 1;
					break;

				case '.':
					if (isFloat)
						throw SyntaxError("extra '.' in number literal");

					iter += 1;
					isFloat = true;
					expecting_digit = true;
					break;

				default:
					if (expecting_digit)
						throw SyntaxError("number literals must end in a digit");

					return isFloat;
			}
		}
	}

	Data Json::parseNumber(const char *& iter)
	{
		const char * const start_of_number = iter;

		try
		{
			bool is_base_decimal = parseNumberLiteral(iter);

			if (*iter == 'e' || *iter == 'E')
			{
				iter += 1;

				if (parseNumberLiteral(iter))
					throw SyntaxError("exponents must be integers");
			}

			std::string number_literal(start_of_number, iter - start_of_number);

			return is_base_decimal
				? Data(std::stod(number_literal))
				: Data(std::stoll(number_literal));
		}
		catch (const std::exception& e)
		{
			throw SyntaxError("failed to parse number '"
				+ std::string(start_of_number, iter - start_of_number)
				+ "': "
				+ std::string(e.what()));
		}
	}

	Data Json::parseString(const char *& iter)
	{
		iter += 1;

		const char * const start_of_string = iter;

		while (*iter != '\"')
		{
			if (*iter == '\0')
				throw SyntaxError("unterminated string: "
					+ std::string(start_of_string, iter - start_of_string));

			iter += 1;
		}

		Data out = std::string(start_of_string, iter - start_of_string);

		iter += 1;

		return out;
	}

	Data Json::parseNull(const char *& iter)
	{
		iter += 1;

		for (const char *match = "ull"; *match; ++match)
		{
			if (*iter != *match)
				throw SyntaxError("unexpected character '"
					+ std::string(1, *iter)
					+ "' found in null literal");

			iter += 1;
		}

		return Data();
	}

	Data Json::parseBool(const char *&iter, bool expected)
	{
		iter += 1;

		const char *match;

		if (expected)
			match = "rue";
		else
			match = "alse";

		while (*match)
		{
			if (*iter != *match)
				throw SyntaxError("unexpected character '"
					+ std::string(1, *iter)
					+ "' found in bool literal");

			iter += 1;
			match += 1;
		}
		
		return Data(expected);
	}

	Data Json::parseArray(const char *& iter)
	{
		Data arr(DataType::Array);

		iter += 1;

		if (*iter == ']')
		{
			iter += 1;
			return arr;
		}

		size_t curr_array_index = 0;

		while (true)
		{
			arr[curr_array_index++] = parseValue(iter);

			if (*iter != ',')
				break;
				
			iter += 1;
		}

		if (*iter != ']')
			throw SyntaxError("unterminated array");

		iter += 1;

		return arr;
	}

	Data Json::parseObject(const char *& iter)
	{
		Data obj(DataType::Table);

		iter += 1;

		if (*iter == '}')
		{
			iter += 1;
			return obj;
		}
		
		while (true)
		{
			if (*iter != '\"')
				throw SyntaxError("invalid label");

			iter += 1;

			const char *end_of_label = iter;
			const char *start_of_label = iter;

			while (*end_of_label != '\"')
			{
				if (!end_of_label[0])
					throw SyntaxError("unterminated label: '"
						+ std::string(start_of_label, end_of_label - start_of_label)
						+ "'");

				end_of_label += 1;
			}

			std::string label(start_of_label, end_of_label - start_of_label);
			iter = end_of_label + 1;

			if (*iter != ':')
				throw SyntaxError("expected ':' but found: "
					+ std::string(iter, 1));

			iter += 1;
			
			obj[label] = parseValue(iter);
			
			if (*iter != ',')
			{
				if (*iter != '}')
					throw SyntaxError("unterminated object found in JSON");
				iter += 1;
				break;
			}

			iter += 1;
		}

		return obj;
	}

	Data Json::parseValue(const char *& iter)
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
				return parseNumber(iter);
			case '\"':
				return parseString(iter);
			case '{':
				return parseObject(iter);
			case '[':
				return parseArray(iter);
			case 't':
				return parseBool(iter, true);
			case 'f':
				return parseBool(iter, false);
			case 'n':
				return parseNull(iter);
			case 0:
				throw SyntaxError("expected value but reached end of input");
			default:
				throw SyntaxError("invalid value starting with '"
					+ std::string(1, *iter)
					+ "'");
		}
	}

	Data Json::deserialize(const std::string& src)
	{
		std::string preprocessed_src = preprocess(src);

		if (preprocessed_src.empty())
			throw SyntaxError("source string was empty");

		const char *iter = preprocessed_src.c_str();

		auto out = parseValue(iter);

		if (*iter != '\0')
			throw SyntaxError("unexpected characters at end of JSON: '" + std::string(iter) + "'");

		return out;
	}

	std::string Json::serialize_array(const Data& data)
	{
		std::string out = "[";

		const auto& array = data.array();

		bool is_first_element = true;
		for (const auto& element : array)
		{
			if (!is_first_element)
				out += ',';

			out += serialize_value(element);
			is_first_element = false;
		}

		return out + "]";
	}

	std::string Json::serialize_object(const Data& data)
	{
		std::string out = "{";
		
		const auto& table = data.table();

		bool is_first_element = true;
		for (const auto& pair : table)
		{
			if (!is_first_element)
				out += ',';

			out += "\"" + pair.first + "\":" + serialize_value(pair.second);
			is_first_element = false;
		}

		return out + "}";
	}

	std::string Json::serialize_value(const Data& data)
	{
		switch (data.type())
		{
			case DataType::Null:
				return "null";
			case DataType::Integer:
				return std::to_string(data.integer());
			case DataType::Float:
				return std::to_string(data.decimal());
			case DataType::Boolean:
				return data.boolean() ? "true" : "false";
			case DataType::String:
				return "\"" + data.string() + "\"";
			case DataType::Array:
				return serialize_array(data);
			case DataType::Table:
				return serialize_object(data);
			default:
				throw std::runtime_error("Invalid type!");
		}
	}
}

#endif
