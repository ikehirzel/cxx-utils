#include "hirzel/print.hpp"
#include <hirzel/json/Token.hpp>
#include <string>
#include <stdexcept>
#include <cassert>
#include <cctype>

namespace hirzel::json
{
	Token::Token(const char* src, size_t pos, size_t length, TokenType type) :
	_src(src),
	_pos(pos),
	_length(length),
	_type(type)
	{}

	static size_t endOfLineCommentPos(const char* src, size_t pos)
	{
		size_t i;

		for (i = pos; src[i]; ++i)
		{
			if (src[i] == '\n')
			{
				i += 1;
				break;
			}
		}

		return i;
	}

	static size_t endOfBlockCommentPos(const char* src, size_t pos)
	{
		size_t i;

		for (i = pos; src[i]; ++i)
		{
			if (src[i] == '*' && src[i + 1] == '/')
			{
				i += 2;
				break;
			}
		}

		return i;
	}

	static size_t nextTokenPos(const char* src, size_t pos)
	{
		size_t i;

		for (i = pos; src[i]; ++i)
		{
			auto c = src[i];

			if (c <= ' ')
				continue;

			if (c == '/')
			{
				switch (src[i + 1])
				{
				case '/':
					i = endOfLineCommentPos(src, i + 2) - 1;
					continue;

				case '*':
					i = endOfBlockCommentPos(src, i + 2) - 1;
					continue;

				default:
					break;
				}
			}

			break;
		}

		return i;
	}

	static std::runtime_error unexpectedToken(const char* src, size_t startPos)
	{
		size_t i = startPos;

		while (isalpha(src[i]))
			i += 1;

		throw std::runtime_error("Unexpected token '"
			+ std::string(src, i - startPos)
			+ "' at pos: "
			+ std::to_string(startPos)
			+ ".");
	}

	static Token parseStringToken(const char* src, const size_t startPos)
	{
		assert(src[startPos] == '\"');

		size_t i = startPos + 1;

		while (true)
		{
			if (src[i] == '\"')
				break;

			if (src[i] == '\0')
				throw std::runtime_error("Unterminated string: " + std::string(src, i - startPos) + ".");

			i += 1;
		}

		i += 1;

		return Token(src, startPos, i - startPos, TokenType::String);
	}

	static size_t numberLength(const char* const src)
	{
		const char *iter = src;

		while (isdigit(*iter))
			iter += 1;

		return iter - src;
	}

	static Token parseNumberToken(const char* src, const size_t start)
	{
		assert(isdigit(src[start]) || src[start] == '-');

		auto i = start;

		if (src[i] == '-')
		{
			i += 1;

			if (!isdigit(src[i]))
				throw std::runtime_error("Expected number after '-'.");
		}

		i += numberLength(&src[i]);

		if (src[i] == '.')
		{
			i += 1;

			auto fractionLength = numberLength(&src[i]);

			if (fractionLength == 0)
				throw std::runtime_error("Expected fractional part of number after '.'.");

			i += fractionLength;
		}

		switch (src[i])
		{
		case '.':
			throw std::runtime_error("Invalid number format.");

		case 'e':
		case 'E':
		{
			i += 1;

			auto exponentLength = numberLength(&src[i]);

			if (exponentLength == 0)
				throw std::runtime_error("Number is missing exponent at pos: " + std::to_string(i) + ".");

			i += exponentLength;

			if (src[i] == '.')
				throw std::runtime_error("Exponents must be integers.");
			break;
		}

		default:
			break;
		}

		auto length = i - start;
		auto token =  Token(src, start, length, TokenType::Number);

		return token;
	}

	static Token parseTrueToken(const char* src, size_t pos)
	{
		assert(src[pos] == 't');

		if (src[pos + 1] == 'r' && src[pos + 2] == 'u' && src[pos + 3] == 'e' && !isalpha(src[pos + 4]))
			return Token(src, pos, 4, TokenType::True);

		throw unexpectedToken(src, pos);
	}

	static Token parseFalseToken(const char* src, size_t pos)
	{
		assert(src[pos] == 'f');

		if (src[pos + 1] == 'a' && src[pos + 2] == 'l' && src[pos + 3] == 's' && src[pos + 4] == 'e' && !isalpha(src[pos + 5]))
			return Token(src, pos, 5, TokenType::False);

		throw unexpectedToken(src, pos);
	}

	static Token parseNullToken(const char* src, size_t pos)
	{
		assert(src[pos] == 'n');

		if (src[pos + 1] == 'u' && src[pos + 2] == 'l' && src[pos + 3] == 'l' && !isalpha(src[pos + 4]))
			return Token(src, pos, 4, TokenType::Null);

		throw unexpectedToken(src, pos);
	}

	static Token parseToken(const char* src, size_t pos)
	{
		auto c = src[pos];

		switch (c)
		{
		case '\0':
			return Token(src, pos, 1, TokenType::EndOfFile);

		case '{':
			return Token(src, pos, 1, TokenType::LeftBrace);

		case '}':
			return Token(src, pos, 1, TokenType::RightBrace);

		case '[':
			return Token(src, pos, 1, TokenType::LeftBracket);

		case ']':
			return Token(src, pos, 1, TokenType::RightBracket);

		case ',':
			return Token(src, pos, 1, TokenType::Comma);

		case ':':
			return Token(src, pos, 1, TokenType::Colon);

		case '\"':
			return parseStringToken(src, pos);

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
			return parseNumberToken(src, pos);

		case 't':
			return parseTrueToken(src, pos);

		case 'f':
			return parseFalseToken(src, pos);

		case 'n':
			return parseNullToken(src, pos);

		default:
			throw std::runtime_error(std::string("Unexpected token '") + src[pos] + "' at pos: " + std::to_string(pos));
		}
	}

	Token Token::initialFor(const char* src)
	{
		auto pos = nextTokenPos(src, 0);
		auto token = parseToken(src, pos);

		return token;
	}

	void Token::seekNext()
	{
		auto pos = nextTokenPos(_src, _pos + _length);
		auto token = parseToken(_src, pos);

		new(this) auto(std::move(token));
	}

	std::string Token::text() const
	{
		return std::string(&_src[_pos], _length);
	}
}
