#ifndef HIRZEL_UTIL_STR_H
#define HIRZEL_UTIL_STR_H

#include <vector>
#include <string>

namespace hirzel
{
	namespace str
	{
		/**
		 * Checks if given char is invisible
		 * @return	truth value
		 */
		inline bool is_invisible(unsigned char c)
		{
			return (c < 33) || (c == 127);
		}

		inline bool is_alpha(unsigned char c)
		{
			return (c > 64  && c < 91) || (c > 96 && c < 123);
		}

		inline bool is_digit(unsigned char c)
		{
			return (c > 47 && c < 58);
		}

		inline bool is_special(unsigned char c)
		{
			return (c > 32 && c < 48) || (c > 57 && c < 65) ||
				(c > 90 && c < 97) || (c > 122 && c < 127);
		}

		// tokenize a string with multi-char delimiters
		std::vector<std::string> tokenize(const std::string& str,
			const std::vector<std::string>& delims) noexcept;

		std::string purge_tokens(const std::string& str,
			const std::vector<std::string>& tokens) noexcept;

		std::string replace_tokens(const std::string& str,
			const std::vector<std::string>& tokens,
			const std::string& replacement) noexcept;

		std::string get_folder(const std::string& filepath) noexcept;
		std::string get_filename(const std::string& filepath) noexcept;
		std::string get_extension(const std::string& filepath) noexcept;
		std::string get_basename(const std::string& filepath) noexcept;
	}
}

#endif // HIRZEL_UTIL_STR_H

#ifdef HIRZEL_IMPLEMENT

namespace hirzel
{
	namespace str
	{
		std::vector<std::string> tokenize(const std::string& str,
			const std::vector<std::string>& delims) noexcept
		{
			std::string token;
			std::vector<std::string> tokens;

			for (size_t i = 0; i < str.size(); i++)
			{
				bool delim_found = false;
				
				// check if the next chunk is a save delim
				for (const std::string& delim : delims)
				{
					size_t checki = i;

					delim_found = true;
					for (char c : delim)
					{
						if (checki >= str.size() || str[checki] != c)
						{
							delim_found = false;
							break;
						}
						checki += 1;
					}
					
					if (delim_found)
					{
						if (token.size() > 0)
						{
							tokens.push_back(token);
							token.clear();
						}
						
						i += delim.size();
						break;
					}
				}

				token += str[i];
			}

			if (token.size() > 0)
			{
				tokens.push_back(token);
			}

			return tokens;
		}


		std::string purge_tokens(const std::string& str,
			const std::vector<std::string>& tokens) noexcept
		{
			std::string out = str;
			size_t oi = 0;

			for (size_t i = 0; i < out.size(); ++i)
			{
				for (const std::string& tok : tokens)
				{
					size_t checki = i;
					bool is_tok = true;
					for (char c : tok)
					{
						if (c != out[checki])
						{
							is_tok = false;
							break;
						}
					}

					if (is_tok)
					{
						oi += tok.size();
						break;
					}
				}

				out[oi] = out[i];
			}

			return out;
		}


		std::string replace_tokens(const std::string& str,
			const std::vector<std::string>& tokens,
			const std::string& replacement) noexcept
		{
			std::string out;
			out.reserve(str.size());

			for (size_t i = 0; i < str.size(); ++i)
			{
				for (const std::string& tok : tokens)
				{
					size_t checki = i;
					bool is_tok = true;

					for (char c : tok)
					{
						if (c != str[checki])
						{
							is_tok = false;
							break;
						}
					}

					if (is_tok)
					{
						out += replacement;
						break;
					}
				}
				out += str[i];
			}

			out.shrink_to_fit();

			return out;
		}


		std::string get_folder(const std::string &filepath) noexcept
		{
			unsigned int slashIndex = -1;
			for (unsigned int i = 0; i < filepath.size(); i++)
			{
				if (filepath[i] == '/' || filepath[i] == '\\')
				{
					slashIndex = i;
				}
			}
			if (slashIndex < filepath.size())
			{
				return filepath.substr(0, slashIndex);
			}

			return ".";
		}


		std::string get_filename(const std::string &filepath) noexcept
		{
			int slashIndex = -1;
			for (unsigned int i = 0; i < filepath.size(); i++)
			{
				if (filepath[i] == '/' || filepath[i] == '\\')
				{
					slashIndex = i;
				}
			}

			return filepath.substr(slashIndex + 1);
		}


		std::string get_extension(const std::string &filepath) noexcept
		{
			int slashIndex = -1, dotIndex = -1;
			for (unsigned int i = 0; i < filepath.size(); i++)
			{
				if (filepath[i] == '/' || filepath[i] == '\\')
				{
					slashIndex = i;
					dotIndex = slashIndex;
				}
				if (filepath[i] == '.' && dotIndex == slashIndex)
				{
					dotIndex = i;
				}
			}

			return filepath.substr(dotIndex + 1);
		}


		std::string get_basename(const std::string &filepath) noexcept
		{
			int slashIndex = -1, dotIndex = -1;
			for (unsigned int i = 0; i < filepath.size(); i++)
			{
				if (filepath[i] == '/' || filepath[i] == '\\')
				{
					slashIndex = i;
					dotIndex = slashIndex;
				}
				if (filepath[i] == '.' && dotIndex == slashIndex)
				{
					dotIndex = i;
				}
			}

			return filepath.substr(slashIndex + 1, dotIndex - slashIndex - 1);
		}
	} // namespace str
} // namespace hirzel

#endif // UTIL_STR_I
