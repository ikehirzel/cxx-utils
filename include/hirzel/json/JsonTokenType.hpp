#ifndef HIRZEL_JSON_TOKEN_TYPE_HPP
#define HIRZEL_JSON_TOKEN_TYPE_HPP

namespace hirzel::json
{
	enum class JsonTokenType
	{
		LeftBrace,
		RightBrace,
		LeftBracket,
		RightBracket,
		Comma,
		Colon,
		String,
		Number,
		Integer,
		True,
		False,
		Null,
		EndOfFile
	};
}

#endif
