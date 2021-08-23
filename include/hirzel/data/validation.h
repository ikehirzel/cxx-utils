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

	// class TableValidator : public DataValidator
	// {
	// private:

	// 	bool _is_nullable = false;
	// 	std::vector<std::pair<DataValidator*, std::string>> _validators;
	
	// 	TableValidator(bool is_nullable, decltype(_validators)&& validators) :
	// 	_is_nullable(is_nullable),
	// 	_validators(validators)
	// 	{}

	// public:

	// 	TableValidator(const char *&fmt);

	// 	~TableValidator()
	// 	{
	// 		for (const auto& pair : _validators)
	// 			delete pair.first;
	// 	}

	// 	std::vector<std::string> validate(const Data& data) const;
	// };

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
	std::string value_type_error(const std::string& expected, const std::string& actual, bool is_nullable)
	{
		return (is_nullable)
			? "expected value of type '" + expected + "' or null but got '" + actual + "'"
			: "expected value of type '" + expected + "' but got '" + actual + "'";
	}


	namespace details
	{
		DataValidator *parse_data_validator(const char *&iter)
		{
			switch (*iter)
			{
				case '#':
					return new IntegerValidator(iter);
				case '[':
					return new ArrayValidator(iter);
				default:
					throw FormatException("expected value format but got '"
						+ std::string(1, *iter)
						+ "'");
			}
		}

		bool parse_nullable(const char *&iter)
		{
			if (*iter != '?')
				return false;

			iter += 1;

			return true;
		}
	}

	namespace details
	{
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
	}

	Validator::Validator(const std::string& fmt)
	{
		auto preprocessed_fmt = details::preprocess_format(fmt);

		if (preprocessed_fmt.empty())
			throw std::invalid_argument("format must not be empty");

		const char *iter = preprocessed_fmt.c_str();

		_validator = details::parse_data_validator(iter);
	}

	namespace details
	{
		inline long long parse_integer(const char *&iter)
		{
			const char *start_of_literal = iter;

			while (*iter >= '0' && *iter <= '9')
				iter += 1;

			auto number_str = std::string(start_of_literal, iter - start_of_literal);

			return std::stoll(number_str);
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

			long long min;

			try
			{
				min = parse_integer(iter) + exclusive;
			}
			catch (const std::exception& e)
			{
				throw FormatException("invalid lower bound of integer range");
			}

			if (*iter != ',')
				throw FormatException("unexpected token in range format '"
					+ std::string(1, *iter)
					+ "'");

			iter += 1;

			long long max;

			try
			{
				max = parse_integer(iter);
			}
			catch (const std::exception& e)
			{
				throw FormatException("invalid upper bound of integer range");
			}
			
			switch (*iter)
			{
				case ']':
					exclusive = false;
					break;
				case ')':
					exclusive = true;
					break;
				default:
					throw FormatException("unexpected token in range format '"
					+ std::string(1, *iter)
					+ "'");
			}

			iter += 1;

			return { min, max - exclusive };
		}
	}

	IntegerValidator::IntegerValidator(const char *&iter)
	{
		iter += 1;

		auto range = details::parse_integer_range(iter);

		_min = range.first;
		_max = range.second;
		_is_nullable = details::parse_nullable(iter);
	}

	std::vector<std::string> IntegerValidator::validate(const Data& data) const
	{
		bool is_correct_type = data.is_integer() || (data.is_null() && _is_nullable);

		if (!is_correct_type)
			return { "expected data of type 'integer' but got '" + std::string(data.type_name()) + "'" };

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
						throw FormatException("unexpected token in array format '"
							+ std::string(1, *iter)
							+ "'");
				}

				break;
			}
		}

		iter += 1;
		_is_nullable = details::parse_nullable(iter);
	}


	std::vector<std::string> ArrayValidator::validate(const Data& data) const
	{
		auto is_correct_type = data.is_array() || (data.is_null() && _is_nullable);

		if (!is_correct_type)
			return { "expected data of type 'array' but got '"
				+ std::string(data.type_name())
				+ "'" };

		if (data.is_null())
			return {};

		std::vector<std::string> out;

		DataValidator *last_validator = nullptr;

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

	std::string validate_boolean(const Data& data, const char *& iter, bool is_nullable)
	{
		iter += 1;

		bool is_correct_type = data.is_boolean() || (is_nullable && data.is_null());

		if (!is_correct_type)
			return value_type_error("boolean", data.type_name(), is_nullable);

		return "";
	}

	std::string validate_string(const Data& data, const char *& iter, bool is_nullable)
	{
		iter += 1;

		if (*iter != '\'')
			throw FormatException("expected end of string literal but got '"
				+ std::string(1, *iter)
				+ "'");

		iter += 1;

		bool is_correct_type = data.is_string() || (is_nullable && data.is_null());

		if (!is_correct_type)
			return value_type_error("string", data.type_name(), is_nullable);

		return "";
	}

	std::string validate_integer(const Data& data, const char *& iter, bool is_nullable)
	{
		iter += 1;
		bool is_correct_type = data.is_integer() || (is_nullable && data.is_null());

		if (!is_correct_type)
			return value_type_error("integer", data.type_name(), is_nullable);

		return "";
	}

	std::string validate_decimal(const Data& data, const char *&iter, bool is_nullable)
	{
		iter += 1;

		if (*iter != '#')
			return "expected '#' but got '" + std::string(1, *iter) + "'";
		
		iter += 1;

		bool is_correct_type = data.is_number() || (is_nullable && data.is_null());

		if (!is_correct_type)
			return value_type_error("decimal", data.type_name(), is_nullable);

		return "";
	}

	std::string validate_percent(const Data& data, const char *&iter, bool is_nullable)
	{
		bool is_correct_type = data.is_number() || (is_nullable && data.is_null());

		iter += 1;
		if (!is_correct_type)
			return value_type_error("decimal", data.type_name(), is_nullable);

		double value = data.as_double();

		if (value < 0 || value > 1)
			return "expected number in range [0, 1] but got " + std::to_string(value);

		return "";
	}

	std::vector<std::string> validate_array(const Data& data, const char *&iter, bool is_nullable)
	{
		throw std::runtime_error(std::string(__func__) + " is not implemented yet");
	}

	inline bool is_label_char(char c)
	{
		return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
	}

	std::vector<std::string> validate_value(const Data& data, const char *&iter);

	std::vector<std::string> validate_table(const Data& data, const char *&iter, bool is_nullable)
	{
		iter += 1;

		std::vector<std::string> out;

		while (true)
		{
			const char * const start_of_label = iter;

			while (is_label_char(*iter))
				iter += 1;
			
			std::string label(start_of_label, iter - start_of_label);
			
			if (label.empty())
			{
				if (*iter == '}')
					throw FormatException("trailing commas are not allowed after table values");

				throw FormatException("expected label but got '"
					+ std::string(1, *iter)
					+ "'");
			}

			if (*iter != ':')
				throw FormatException("expected ':' but got '"
					+ std::string(1, *iter)
					+ "'");

			iter += 1;

			auto validation_errors = data.contains(label)
				? validate_value(data[label], iter)
				: validate_value(Data(), iter);

			out.insert(out.end(),
				std::make_move_iterator(validation_errors.begin()),
				std::make_move_iterator(validation_errors.end()));
			
			switch (*iter)
			{
				case ',':
					iter += 1;
					continue;
				case '}':
					iter += 1;
					return out;
				default:
					throw FormatException("expected new field or end of table but got '"
						+ std::string(1, *iter)
						+ "'");
			}
		}
	}

	std::vector<std::string> validate_value(const Data& data, const char *&iter)
	{
		std::vector<std::string> out;


		bool is_nullable = false;
		if (*iter == '?')
		{
			is_nullable = true;
			iter += 1;
		}
		
		switch(*iter)
		{
			case 0:
				throw FormatException("expected value format but reached end of input");

			case '{':
			{
				auto validation_errors = validate_table(data, iter, is_nullable);
				out.insert(out.end(),
					std::make_move_iterator(validation_errors.begin()),
					std::make_move_iterator(validation_errors.end()));
				break;
			}
			case '[':
			{
				auto validation_errors = validate_array(data, iter, is_nullable);
				out.insert(out.end(),
					std::make_move_iterator(validation_errors.begin()),
					std::make_move_iterator(validation_errors.end()));
				break;
			}
			case '.':
			{
				auto error = validate_decimal(data, iter, is_nullable);
				if (!error.empty())
					out.push_back(error);
				break;
			}
			case '#':
			{
				auto error = validate_integer(data, iter, is_nullable);
				if (!error.empty())
					out.push_back(error);
				break;
			}
			case '%':
			{
				auto error = validate_percent(data, iter, is_nullable);
				if (!error.empty())
					out.push_back(error);
				break;
			}
			case '~':
			{
				auto error = validate_boolean(data, iter, is_nullable);
				if (!error.empty())
					out.push_back(error);
				break;
			}
			case '\'':
			{
				auto error = validate_string(data, iter, is_nullable);
				if (!error.empty())
					out.push_back(error);
				break;
			}
			
			default:
				if (*iter == '}' || *iter == ']')
					throw FormatException("values must not be empty");
				throw FormatException("invalid value format starting with '"
					+ std::string(1, *iter)
					+ "'");
		}

		return out;
	}
}

#endif
