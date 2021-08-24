#ifndef HIRZEL_DATA_VALIDATION_H
#define HIRZEL_DATA_VALIDATION_H

/**
 * @file validation.h
 * @brief Validation utilities for hirzel::data::Data
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

#include <hirzel/data/data.h>

namespace hirzel::data
{
	class FormatException : public std::exception
	{
	private:

		std::string _msg;

	public:

		FormatException(const std::string& msg) : _msg(msg) {}

		const char * what() const noexcept { return _msg.c_str(); }
	};
	
	class DataValidator
	{
	public:
		virtual ~DataValidator() {};
		virtual std::vector<std::string> validate(const Data& data) const = 0;
	};

	class IntegerValidator : public DataValidator
	{
	private:

		bool _is_nullable = false;
		long long _min = 0;
		long long _max = 0;

	public:

		IntegerValidator(const char *&fmt);
		~IntegerValidator() {}

		std::vector<std::string> validate(const Data& data) const;
	};

	class TableValidator : public DataValidator
	{
	private:

		bool _is_nullable = false;
		std::vector<std::pair<std::string, DataValidator*>> _validators;

	public:

		TableValidator(const char *&fmt);

		~TableValidator()
		{
			for (const auto& pair : _validators)
				delete pair.second;
		}

		std::vector<std::string> validate(const Data& data) const;
	};

	class ArrayValidator : public DataValidator
	{
	private:

		bool _is_nullable = false;
		std::vector<DataValidator*> _validators;

	public:

		ArrayValidator(const char *&fmt);

		~ArrayValidator()
		{
			for (auto validator : _validators)
				delete validator;
		}

		std::vector<std::string> validate(const Data& data) const;
	};

	class Validator
	{
		DataValidator *_validator = nullptr;

	public:

		Validator(const std::string& fmt);

		~Validator()
		{
			delete _validator;
		}

		inline std::vector<std::string> operator()(const Data& data) const
		{
			return _validator->validate(data);
		}
	};
}

#endif

#if !defined(HIRZEL_DATA_VALIDATION_I) && defined(HIRZEL_IMPLEMENT)
#define HIRZEL_DATA_VALIDATION_I

#include <climits>

namespace hirzel::data
{
	namespace details
	{
		std::string value_type_error(const std::string& expected, const std::string& actual, bool is_nullable)
		{
			return (is_nullable)
				? "expected value of type '" + expected + "' or null but got '" + actual + "'"
				: "expected value of type '" + expected + "' but got '" + actual + "'";
		}

		inline FormatException unexpected_token_error(const std::string& type, char c)
		{
			return FormatException("unepxected token in "
				+ type
				+ " format '"
				+ std::string(1, c)
				+ "'");
		}

		DataValidator *parse_data_validator(const char *&iter)
		{
			switch (*iter)
			{
				case '#':
					return new IntegerValidator(iter);
				case '[':
					return new ArrayValidator(iter);
				case '{':
					return new TableValidator(iter);
				default:
					throw FormatException("expected value format but got '"
						+ std::string(1, *iter)
						+ "'");
			}
		}

		bool parse_is_nullable(const char *&iter)
		{
			if (*iter != '?')
				return false;

			iter += 1;

			return true;
		}
		
		std::string preprocess_format(const std::string& fmt)
		{
			std::string out(fmt.size(), 0);
			size_t oi = 0;

			for (char c : fmt)
			{
				if (c > ' ')
					out[oi++] = c;
			}

			out.resize(oi);

			return out;
		}

		inline long long parse_range_integer(const char *&iter)
		{
			const char *start_of_literal = iter;

			while (*iter >= '0' && *iter <= '9')
				iter += 1;

			auto number_str = std::string(start_of_literal, iter - start_of_literal);

			try
			{
				return std::stoll(number_str);
			}
			catch (const std::exception& e)
			{
				throw FormatException("expected integer for range bounds but got '"
					+ number_str
					+ "'");
			}
		}

		inline std::pair<long long, long long> parse_integer_range(const char *& iter)
		{
			bool exclusive = false;

			switch (*iter)
			{
				case '[':
					break;
				case '(':
					exclusive = true;
					break;
				default:
					return { LLONG_MIN, LLONG_MAX };
			}
			iter += 1;

			long long min = details::parse_range_integer(iter) + exclusive;

			if (*iter != ',')
				throw details::unexpected_token_error("range", *iter);

			iter += 1;

			long long max = details::parse_range_integer(iter);
			
			switch (*iter)
			{
				case ']':
					exclusive = false;
					break;
				case ')':
					exclusive = true;
					break;
				default:
					throw details::unexpected_token_error("range", *iter);
			}

			iter += 1;

			return { min, max - exclusive };
		}

		inline bool is_key_char(char c)
		{
			return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
		}

		std::string parse_table_key(const char *&iter)
		{
			const char * const start_of_key = iter;
			while (is_key_char(*iter))
				iter += 1;

			auto key = std::string(start_of_key, iter - start_of_key);
			
			if (*iter != ':')
				throw details::unexpected_token_error("table key", *iter);

			iter += 1;

			return key;
		}
	}

	Validator::Validator(const std::string& fmt)
	{
		auto preprocessed_fmt = details::preprocess_format(fmt);

		if (preprocessed_fmt.empty())
			throw std::invalid_argument("format must not be empty");

		const char *iter = preprocessed_fmt.c_str();

		_validator = details::parse_data_validator(iter);
	}

	IntegerValidator::IntegerValidator(const char *&iter)
	{
		iter += 1;

		auto range = details::parse_integer_range(iter);

		_min = range.first;
		_max = range.second;
		_is_nullable = details::parse_is_nullable(iter);
	}

	std::vector<std::string> IntegerValidator::validate(const Data& data) const
	{
		bool is_correct_type = data.is_integer() || (data.is_null() && _is_nullable);

		if (!is_correct_type)
			return { details::value_type_error("integer", data.type_name(), _is_nullable) };

		auto value = data.as_long_long();

		if (value < _min)
			return { "expected integer >= " + std::to_string(_min) + " but got " + std::to_string(value) };

		if (value > _max)
			return { "expected integer <= " + std::to_string(_max) + " but got " + std::to_string(value) };

		return {};
	}

	ArrayValidator::ArrayValidator(const char *&iter)
	{
		iter += 1;

		if (*iter != ']')
		{
			while (true)
			{
				auto validator = details::parse_data_validator(iter);
				_validators.push_back(validator);

				switch (*iter)
				{
					case ',':
						iter += 1;
						continue;
					case ']':
						break;
					default:
						throw details::unexpected_token_error("array", *iter);
				}

				break;
			}
		}

		iter += 1;
		_is_nullable = details::parse_is_nullable(iter);
	}


	std::vector<std::string> ArrayValidator::validate(const Data& data) const
	{
		auto is_correct_type = data.is_array() || (data.is_null() && _is_nullable);

		if (!is_correct_type)
			return { details::value_type_error("array", data.type_name(), _is_nullable) };

		if (data.is_null())
			return {};

		std::vector<std::string> out;

		size_t validator_index = 0;
		auto array = data.array();

		for (auto element : array)
		{
			if (validator_index >= _validators.size())
			{
				out.push_back("expected "
					+  std::to_string(_validators.size())
					+ " elements but got "
					+ std::to_string(data.size()));

				break;
			}

			auto validation_errors = _validators[validator_index]->validate(element);

			out.insert(out.end(),
				std::make_move_iterator(validation_errors.begin()),
				std::make_move_iterator(validation_errors.end()));

			validator_index += 1;
		}

		return out;
	}

	TableValidator::TableValidator(const char *&iter)
	{
		iter += 1;

		if (*iter != '}')
		{
			while (true)
			{
				auto key = details::parse_table_key(iter);
				auto validator = details::parse_data_validator(iter);

				_validators.push_back({ key, validator });

				switch (*iter)
				{
					case ',':
						continue;
					case '}':
						break;
					default:
						throw details::unexpected_token_error("table", *iter);
				}

				break;
			}
		}

		iter += 1;
		_is_nullable = details::parse_is_nullable(iter);
	}

	std::vector<std::string> TableValidator::validate(const Data& data) const
	{
		auto is_correct_type = data.is_table() || (data.is_null() && _is_nullable);

		if (!is_correct_type)
			return { details::value_type_error("table", data.type_name(), _is_nullable) };

		std::vector<std::string> out;

		for (const auto& pair : _validators)
		{
			const auto& element = data.contains(pair.first)
				? data[pair.first]
				: Data();

			auto validation_errors = pair.second->validate(element);

			out.insert(out.end(),
				std::make_move_iterator(validation_errors.begin()),
				std::make_move_iterator(validation_errors.end()));
		}

		return out;
	}
}

#endif
