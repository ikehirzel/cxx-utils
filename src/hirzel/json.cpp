#include "hirzel/json.hpp"
#include "hirzel/json/JsonToken.hpp"
#include "hirzel/file.hpp"
#include "hirzel/json/JsonValueType.hpp"
#include "hirzel/print.hpp"
#include <utility>
#include <cassert>
#include <cstdlib>

namespace hirzel::json
{
	static int xIndex = std::ios::xalloc();

	static JsonValue deserializeValue(JsonToken& token);

	static JsonValue deserializeObject(JsonToken& token)
	{
		assert(token.type() == JsonTokenType::LeftBrace);

		token.seekNext();

		auto object = JsonObject();

		if (token.type() != JsonTokenType::RightBrace)
		{
			while (true)
			{
				if (token.type() != JsonTokenType::String)
					throw std::runtime_error("Expected label, got '" + token.text() + "'.");

				auto label = std::string(token.src() + token.pos() + 1, token.length() - 2);

				token.seekNext();

				if (token.type() != JsonTokenType::Colon)
					throw std::runtime_error("Expected ':' before '" + token.text() + "'.");

				token.seekNext();

				auto value = deserializeValue(token);

				object.emplace(std::move(label), std::move(value));

				if (token.type() == JsonTokenType::Comma)
				{
					token.seekNext();
					continue;
				}

				break;
			}

			if (token.type() != JsonTokenType::RightBrace)
				throw std::runtime_error("Expected '}' before '" + token.text() + "'.");
		}

		token.seekNext();

		return object;
	}

	static JsonValue deserializeArray(JsonToken& token)
	{
		assert(token.type() == JsonTokenType::LeftBracket);
		token.seekNext();

		auto arr = JsonArray();

		if (token.type() != JsonTokenType::RightBracket)
		{
			while (true)
			{
				auto value = deserializeValue(token);

				arr.emplace_back(std::move(value));

				if (token.type() == JsonTokenType::Comma)
				{
					token.seekNext();
					continue;
				}

				break;
			}

			if (token.type() != JsonTokenType::RightBracket)
				throw std::runtime_error("Expected ']' before '" + token.text() + "'.");
		}

		token.seekNext();

		return arr;
	}

	static JsonValue deserializeString(JsonToken& token)
	{
		assert(token.type() == JsonTokenType::String);

		auto text = std::string(token.src() + token.pos() + 1, token.length() - 2);
		auto json = JsonValue(std::move(text));

		token.seekNext();

		return json;
	}

	static JsonValue deserializeNumber(JsonToken& token)
	{
		assert(token.type() == JsonTokenType::Number);

		auto text = token.text();
		auto value = atof(text.c_str());
		auto json = JsonValue(value);

		token.seekNext();

		return json;
	}

	static JsonValue deserializeValue(JsonToken& token)
	{
		switch (token.type())
		{
			case JsonTokenType::LeftBrace:
				return deserializeObject(token);

			case JsonTokenType::LeftBracket:
				return deserializeArray(token);

			case JsonTokenType::String:
				return deserializeString(token);

			case JsonTokenType::Number:
				return deserializeNumber(token);

			case JsonTokenType::True:
				token.seekNext();
				return JsonValue(true);

			case JsonTokenType::False:
				token.seekNext();
				return JsonValue(false);

			case JsonTokenType::Null:
				token.seekNext();
				return JsonValue();

			case JsonTokenType::EndOfFile:
				throw std::runtime_error("Unexpected end of file.");

			default:
				throw std::runtime_error("Unexpected token: '" + token.text() + "'.");
		}
	}
	
	JsonValue deserialize(const char* json)
	{
		auto token = JsonToken::initialFor(json);
		auto out = deserializeValue(token);

		if (token.type() != JsonTokenType::EndOfFile)
			throw std::runtime_error("Unexpected token: " + token.text());

		return out;
	}

	JsonValue deserialize(const std::string& json)
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
	void serializeValue(std::ostream& out, const JsonValue& json);

	template <bool minimized>
	void serializeArray(std::ostream& out, const JsonValue& json)
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
			out << "\n";

		out << "]";
	}

	template <bool minimized>
	void serializeObject(std::ostream& out, const JsonValue& json)
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
			out << "\n";

		out << "}";
	}

	template <bool minimized>
	void serializeValue(std::ostream& out, const JsonValue& json)
	{
		switch (json.type())
		{
		case JsonValueType::Null:
			out << "null";
			break;

		case JsonValueType::Boolean:
			out << json.boolean();
			break;

		case JsonValueType::Number:
			out << json.number();
			break;

		case JsonValueType::String:
			out << "\"" << json.string() << "\"";
			break;

		case JsonValueType::Array:
			serializeArray<minimized>(out, json);
			break;

		case JsonValueType::Object:
			serializeObject<minimized>(out, json);
			break;

		default:
			break;
		}
	}

	void serialize(std::ostream& out, const JsonValue& json, bool minimized)
	{
		if (minimized)
		{
			serializeValue<true>(out, json);
			return;
		}

		serializeValue<false>(out, json);
	}

	std::string serialize(const JsonValue& json, bool minimized)
	{
		std::ostringstream out;

		serialize(out, json, minimized);

		return out.str();
	}
}
