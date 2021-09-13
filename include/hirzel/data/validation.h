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

#include <climits>
#include <cfloat>

namespace hirzel
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
	public:

		struct Range
		{
			long long min = LLONG_MIN;
			long long max = LLONG_MAX;
			bool min_exclusive = false;
			bool max_exclusive = false;
		};

	private:

		bool _is_nullable = false;
		Range _range;

	public:

		IntegerValidator(const char *&fmt);
		~IntegerValidator() {}

		std::vector<std::string> validate(const Data& data) const;
	};

	class StringValidator : public DataValidator
	{
	private:

		bool _is_nullable = false;

	public:

		StringValidator(const char *&fmt);
		~StringValidator() {}

		std::vector<std::string> validate(const Data& data) const;
	};

	class DecimalValidator : public DataValidator
	{
	public:
	
		struct Range
		{
			double min = -DBL_MAX;
			double max = DBL_MAX;
			bool min_exclusive = false;
			bool max_exclusive = false;
		};

	private:

		bool _is_nullable = false;
		Range _range;

	public:

		DecimalValidator(const char *&fmt);
		~DecimalValidator() {}

		std::vector<std::string> validate(const Data& data) const;
	};

	class BooleanValidator : public DataValidator
	{
	private:

		bool _is_nullable = false;

	public:

		BooleanValidator(const char *&fmt);
		~BooleanValidator() {}

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
		bool _is_last_variadic = false;
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

namespace hirzel
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
			return FormatException("unexpected token in "
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
				case '$':
					return new StringValidator(iter);
				case '&':
					return new BooleanValidator(iter);
				case '%':
					return new DecimalValidator(iter);
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

		inline bool is_integer_char(char c)
		{
			return c >= '0' && c <= '9';
		}

		inline bool is_range_terminal(char c)
		{
			switch (c)
			{
				case ',':
				case '[':
				case ']':
				case '(':
				case ')':
					return true;
				default:
					return false;
			}
		}

		inline long long parse_range_integer(const char *&iter, bool is_lower_bound)
		{
			const char *start_of_literal = iter;

			while (!is_range_terminal(*iter))
				iter += 1;

			auto number_str = std::string(start_of_literal, iter - start_of_literal);

			if (number_str == "~")
				return is_lower_bound ? LLONG_MIN : LLONG_MAX;

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

		IntegerValidator::Range parse_integer_range(const char *& iter)
		{
			IntegerValidator::Range out;

			switch (*iter)
			{
				case '[':
					break;
				case '(':
					out.min_exclusive = true;
					break;
				default:
					return out;
			}

			iter += 1;

			out.min = details::parse_range_integer(iter, true);

			if (*iter != ',')
				throw details::unexpected_token_error("integer range", *iter);

			iter += 1;

			out.max = details::parse_range_integer(iter, false);
			
			if (out.min > out.max)
				throw FormatException("lower-bound must be less than or equal to upper-bound in range");

			switch (*iter)
			{
				case ']':
					break;
				case ')':
					out.max_exclusive = true;
					break;
				default:
					throw details::unexpected_token_error("integer range", *iter);
			}

			iter += 1;

			return out;
		}

		bool is_decimal_char(char c)
		{
			return (c >= '0' && c <= '9') || c == '.';
		}

		double parse_range_decimal(const char *&iter, bool is_lower_bound)
		{
			const char *start_of_literal = iter;

			while (!is_range_terminal(*iter))
				iter += 1;

			auto number_str = std::string(start_of_literal, iter - start_of_literal);

			if (number_str == "~")
				return (is_lower_bound) ? -DBL_MAX : DBL_MAX;

			try
			{
				return std::stod(number_str);
			}
			catch (const std::exception& e)
			{
				throw FormatException("expected decimal for range bounds but got '"
					+ number_str
					+ "'");
			}
		}

		std::string integer_range_error(const IntegerValidator::Range& range, long long value)
		{
			auto range_str = (range.min_exclusive ? "(" : "[")
				+ std::to_string(range.min)
				+ ", "
				+ std::to_string(range.max)
				+ (range.max_exclusive ? ")" : "]");

			return "expected integer in range " + range_str + " but got " + std::to_string(value);
		}

		std::string decimal_range_error(const DecimalValidator::Range& range, double value)
		{
			auto range_str = (range.min_exclusive ? "(" : "[")
				+ std::to_string(range.min)
				+ ", "
				+ std::to_string(range.max)
				+ (range.max_exclusive ? ")" : "]");

			return "expected decimal in range " + range_str + " but got " + std::to_string(value);
		}

		DecimalValidator::Range parse_decimal_range(const char *&iter)
		{
			DecimalValidator::Range out;

			switch (*iter)
			{
				case '(':
					out.min_exclusive = true;
					break;
				case '[':
					break;
				default:
					return out;
			}

			iter += 1;

			out.min = parse_range_decimal(iter, true);

			if (*iter != ',')
				throw unexpected_token_error("decimal range", *iter);
			
			iter += 1;

			out.max = parse_range_decimal(iter, false);

			if (out.min > out.max)
				throw FormatException("lower-bound must be less than or equal to upper-bound in range");

			switch (*iter)
			{
				case ')':
					out.max_exclusive = true;
					break;
				case ']':
					break;
				default:
					throw unexpected_token_error("decimal range", *iter);
			}

			iter += 1;

			return out;
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

		inline void parse_elipsis(const char *&iter)
		{
			iter += 1;

			for (unsigned i = 0; i < 2; ++i)
			{
				if (*iter != '.')
					throw unexpected_token_error("elipsis", *iter);
				iter += 1;
			}
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

		_range = details::parse_integer_range(iter);
		_is_nullable = details::parse_is_nullable(iter);
	}

	std::vector<std::string> IntegerValidator::validate(const Data& data) const
	{
		bool is_correct_type = data.is_integer() || (data.is_null() && _is_nullable);

		if (!is_correct_type)
			return { details::value_type_error("integer", data.type_name(), _is_nullable) };

		auto value = data.as_long_long();
		auto is_out_of_range = value < _range.min
			|| (value == _range.min && _range.min_exclusive)
			|| value > _range.max
			|| (value == _range.max && _range.max_exclusive);

		if (is_out_of_range)
			return { details::integer_range_error(_range, value) };

		return {};
	}

	DecimalValidator::DecimalValidator(const char *&iter)
	{
		iter += 1;

		_range = details::parse_decimal_range(iter);
		_is_nullable = details::parse_is_nullable(iter);
	}

	std::vector<std::string> DecimalValidator::validate(const Data& data) const
	{
		bool is_correct_type = data.is_number() || (data.is_null() && _is_nullable);

		if (!is_correct_type)
			return { details::value_type_error("decimal", data.type_name(), _is_nullable) };

		auto value = data.as_double();
		auto is_out_of_range = value < _range.min
			|| (value == _range.min && _range.min_exclusive)
			|| value > _range.max
			|| (value == _range.max && _range.max_exclusive);

		if (is_out_of_range)
			return { details::decimal_range_error(_range, value) };

		return {};
	}

	StringValidator::StringValidator(const char *&iter)
	{
		iter += 1;
		_is_nullable = details::parse_is_nullable(iter);
	}

	std::vector<std::string> StringValidator::validate(const Data& data) const
	{
		auto is_correct_type = data.is_string() || (data.is_null() && _is_nullable);

		if (!is_correct_type)
			return { details::value_type_error("string", data.type_name(), _is_nullable) };

		return {};
	}

	BooleanValidator::BooleanValidator(const char *&iter)
	{
		iter += 1;
		_is_nullable = details::parse_is_nullable(iter);
	}

	std::vector<std::string> BooleanValidator::validate(const Data& data) const
	{
		auto is_correct_type = data.is_boolean() || (data.is_null() && _is_nullable);

		if (!is_correct_type)
			return { details::value_type_error("boolean", data.type_name(), _is_nullable) };

		return {};
	}

	ArrayValidator::ArrayValidator(const char *&iter)
	{
		iter += 1;

		if (*iter != ']')
		{
			auto element_index = 0u;

			while (true)
			{
				if (_is_last_variadic)
					throw FormatException("only the last element in an array may be variadic");
				try
				{
					auto validator = details::parse_data_validator(iter);
					_validators.push_back(validator);

					switch (*iter)
					{
						case ',':
							iter += 1;
							element_index += 1;
							continue;
						case ']':
							break;
						case '.':
							_is_last_variadic = true;
							details::parse_elipsis(iter);

							if (*iter == ',')
							{
								iter += 1;
								continue;
							}
							break;
						default:
							throw details::unexpected_token_error("array", *iter);
					}
				}
				catch (const FormatException& e)
				{
					throw FormatException("error in array at element ("
						+ std::to_string(element_index)
						+ "): "
						+ std::string(e.what()));
				}

				break;
			}
		}
		if (*iter != ']')
			throw details::unexpected_token_error("end of array", *iter);
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

		const auto& array = data.array();

		if (_validators.empty())
		{
			if (!array.empty())
				return { "expected no array elements but got " + std::to_string(data.size()) };

			return {};
		}

		if (_is_last_variadic)
		{
			if (array.size() < _validators.size() - 1)
				return { "expected at least " + std::to_string(_validators.size() - 1)
					+ "array element(s) but got " + std::to_string(array.size()) };

			if (array.empty())
				return {};
		}
		else if (array.size() != _validators.size())
			return { "expected "  + std::to_string(_validators.size()) +
				" array element(s) but got " + std::to_string(array.size()) };

		std::vector<std::string> out;
		size_t element_index = 0;

		auto validator_count = _validators.size() - (size_t)_is_last_variadic;

		for (size_t i = 0; i < validator_count; ++i)
		{
			auto validation_errors = _validators[i]->validate(array[element_index]);

			out.insert(out.end(),
				std::make_move_iterator(validation_errors.begin()),
				std::make_move_iterator(validation_errors.end()));

			element_index += 1;
		}

		if (_is_last_variadic)
		{
			auto validator = _validators.back();

			for (size_t i = element_index; i < array.size(); ++i)
			{
				auto validation_errors = validator->validate(array[i]);

				out.insert(out.end(),
					std::make_move_iterator(validation_errors.begin()),
					std::make_move_iterator(validation_errors.end()));
			}
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

				try
				{
					auto validator = details::parse_data_validator(iter);
					_validators.push_back({ key, validator });

					switch (*iter)
					{
						case ',':
							iter += 1;
							continue;
						case '}':
							break;
						default:
							throw details::unexpected_token_error("table", *iter);
					}
				}
				catch (const FormatException& e)
				{
					throw FormatException("error in table entry '" + key + "': " + std::string(e.what()));
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
