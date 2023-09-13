#ifndef HIRZEL_JSON_JSON_TOKEN_HPP
#define HIRZEL_JSON_JSON_TOKEN_HPP

#include "hirzel/primitives.hpp"
#include "hirzel/json/TokenType.hpp"
#include <string>

namespace hirzel::json
{
	class Token
	{
		const char* _src;
		size_t _pos;
		size_t _length;
		TokenType _type;

	public:

		Token(const char* src, size_t pos, size_t length, TokenType type);
		Token(Token&&) = default;
		Token(const Token&) = default;

		static Token initialFor(const char* src);

		void seekNext();
		std::string text() const;

		const auto* src() const { return _src; }
		const auto& pos() const { return _pos; }
		const auto& length() const { return _length; }
		const auto& type() const { return _type; }
	};
}

#endif
