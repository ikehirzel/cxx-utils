#ifndef HIRZEL_JSON_JSON_TOKEN_HPP
#define HIRZEL_JSON_JSON_TOKEN_HPP

#include "hirzel/primitives.hpp"
#include "hirzel/json/JsonTokenType.hpp"
#include <string>

namespace hirzel::json
{
	class JsonToken
	{
		const char* _src;
		usize _pos;
		usize _length;
		JsonTokenType _type;

	public:

		JsonToken(const char* src, usize pos, usize length, JsonTokenType type);
		JsonToken(JsonToken&&) = default;
		JsonToken(const JsonToken&) = default;

		static JsonToken initialFor(const char* src);

		void seekNext();
		std::string text() const;

		const auto* src() const { return _src; }
		const auto& pos() const { return _pos; }
		const auto& length() const { return _length; }
		const auto& type() const { return _type; }
	};
}

#endif
