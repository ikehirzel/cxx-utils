#include "hirzel/json.hpp"
#include "hirzel/json/Token.hpp"
#include "hirzel/file.hpp"
#include "hirzel/json/ValueType.hpp"
#include "hirzel/print.hpp"
#include <stdexcept>
#include <utility>
#include <cassert>
#include <cstdlib>

namespace hirzel::json
{
	static int xIndex = std::ios::xalloc();

	static Value deserializeValue(Token& token);

	static Value deserializeObject(Token& token)
	{
		assert(token.type() == TokenType::LeftBrace);

		token.seekNext();

		auto object = Object();

		if (token.type() != TokenType::RightBrace)
		{
			while (true)
			{
				if (token.type() != TokenType::String)
					throw std::runtime_error("Expected label, got '" + token.text() + "'.");

				auto label = std::string(token.src() + token.pos() + 1, token.length() - 2);

				token.seekNext();

				if (token.type() != TokenType::Colon)
					throw std::runtime_error("Expected ':' before '" + token.text() + "'.");

				token.seekNext();

				auto value = deserializeValue(token);

				object.emplace(std::move(label), std::move(value));

				if (token.type() == TokenType::Comma)
				{
					token.seekNext();
					continue;
				}

				break;
			}

			if (token.type() != TokenType::RightBrace)
				throw std::runtime_error("Expected '}' before '" + token.text() + "'.");
		}

		token.seekNext();

		return object;
	}

	static Value deserializeArray(Token& token)
	{
		assert(token.type() == TokenType::LeftBracket);
		token.seekNext();

		auto arr = Array();

		if (token.type() != TokenType::RightBracket)
		{
			while (true)
			{
				auto value = deserializeValue(token);

				arr.emplace_back(std::move(value));

				if (token.type() == TokenType::Comma)
				{
					token.seekNext();
					continue;
				}

				break;
			}

			if (token.type() != TokenType::RightBracket)
				throw std::runtime_error("Expected ']' before '" + token.text() + "'.");
		}

		token.seekNext();

		return arr;
	}

	static Value deserializeString(Token& token)
	{
		assert(token.type() == TokenType::String);

		auto text = std::string(token.src() + token.pos() + 1, token.length() - 2);
		auto json = Value(std::move(text));

		token.seekNext();

		return json;
	}

	static Value deserializeNumber(Token& token)
	{
		assert(token.type() == TokenType::Number);

		auto text = token.text();
		auto value = atof(text.c_str());
		auto json = Value(value);

		token.seekNext();

		return json;
	}

	static Value deserializeValue(Token& token)
	{
		switch (token.type())
		{
			case TokenType::LeftBrace:
				return deserializeObject(token);

			case TokenType::LeftBracket:
				return deserializeArray(token);

			case TokenType::String:
				return deserializeString(token);

			case TokenType::Number:
				return deserializeNumber(token);

			case TokenType::True:
				token.seekNext();
				return Value(true);

			case TokenType::False:
				token.seekNext();
				return Value(false);

			case TokenType::Null:
				token.seekNext();
				return Value();

			case TokenType::EndOfFile:
				throw std::runtime_error("Unexpected end of file.");

			default:
				throw std::runtime_error("Unexpected token: '" + token.text() + "'.");
		}
	}
	
	Value deserialize(const char* json)
	{
		try
		{
			auto token = Token::initialFor(json);
			auto out = deserializeValue(token);

			if (token.type() != TokenType::EndOfFile)
				throw std::runtime_error("Unexpected token: " + token.text());

			return out;
		}
		catch (const std::exception& e)
		{
			throw std::runtime_error("Failed to deserialize JSON: " + std::string(e.what()));
		}
	}

	Value deserialize(const std::string& json)
	{
		return deserialize(json.c_str());
	}


	void indent(std::ostream& out)
	{
		auto index = out.iword(xIndex);

		for (long i = 0; i < index; ++i)
			out << "\t";
	}

	template <bool minimized>
	void serializeValue(std::ostream& out, const Value& json);

	template <bool minimized>
	void serializeArray(std::ostream& out, const Value& json)
	{
		if (json.array().empty())
		{
			out << "[]";
			return;
		}

		out << "[";

		auto isFirst = true;
		auto& depth = out.iword(xIndex);

		depth += 1;

		for (const auto& item : json.array())
		{
			if (isFirst)
			{
				isFirst = false;
			}
			else
			{
				out << ",";
			}

			if constexpr (minimized == false)
			{
				out << "\n";
				indent(out);
			}

			serializeValue<minimized>(out, item);
		}

		depth -= 1;

		if constexpr (minimized == false)
		{
			out << "\n";
			indent(out);
		}

		out << "]";
	}

	template <bool minimized>
	void serializeObject(std::ostream& out, const Value& json)
	{
		if (json.object().empty())
		{
			out << "{}";
			return;
		}

		out << "{";

		auto& depth = out.iword(xIndex);

		depth += 1;

		auto isFirst = true;

		for (const auto& pair : json.object())
		{
			if (isFirst)
			{
				isFirst = false;
			}
			else
			{
				out << ",";
			}

			if constexpr (minimized == false)
			{
				out << "\n";

				indent(out);
			}

			out << "\"" << pair.first << "\":";

			if constexpr (minimized == false)
				out << " ";

			serializeValue<minimized>(out, pair.second);
		}

		depth -= 1;

		if constexpr (minimized == false)
		{
			out << "\n";
			indent(out);
		}

		out << "}";
	}

	template <bool minimized>
	void serializeValue(std::ostream& out, const Value& json)
	{
		switch (json.type())
		{
		case ValueType::Null:
			out << "null";
			break;

		case ValueType::Boolean:
			out << json.boolean();
			break;

		case ValueType::Number:
			out << json.number();
			break;

		case ValueType::String:
			out << "\"" << json.string() << "\"";
			break;

		case ValueType::Array:
			serializeArray<minimized>(out, json);
			break;

		case ValueType::Object:
			serializeObject<minimized>(out, json);
			break;

		default:
			break;
		}
	}

	void serialize(std::ostream& out, const Value& json, bool minimized)
	{
		if (minimized)
		{
			serializeValue<true>(out, json);
			return;
		}

		serializeValue<false>(out, json);
	}

	std::string serialize(const Value& json, bool minimized)
	{
		std::ostringstream out;

		serialize(out, json, minimized);

		return out.str();
	}
}
