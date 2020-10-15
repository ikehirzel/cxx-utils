#include "strutil.h"
#include <iostream>
namespace hirzel
{
	std::vector<std::string> tokenize(const std::string& str, const std::string& delims)
	{
		std::string token;
		std::vector<std::string> tokens;

		for (char c : str)
		{
			bool is_delim = false;
			for (char d : delims)
			{
				if (d == c)
				{
					is_delim = true;
					break;
				}
			}
			if (is_delim)
			{
				if (token.size() > 0)
				{
					tokens.push_back(token);
				}
				token.clear();
				continue;
			}
			token += c;
		}
		if (token.size() > 0)
		{
			tokens.push_back(token);
		}
		return tokens;
	}

	std::string purge_delims(const std::string& str, const std::string& delims)
	{
		std::string out = str;

		for (unsigned int i = out.size() - 1; i < out.size(); i--)
		{
			bool is_delim = false;
			for (char c : delims)
			{
				if (out[i] == c)
				{
					is_delim = true;
				}
			}
			if (is_delim)
			{
				out.erase(i, 1);
			}
		}

		return out;
	}

	std::string replace_delims(const std::string& str, const std::string& delims, char replacement)
	{
		std::string out = str;

		for (unsigned int i = 0; i < out.size(); i++)
		{
			bool is_delim = false;
			for (char c : delims)
			{
				if (out[i] == c)
				{
					is_delim = true;
				}
			}
			if (is_delim)
			{
				out[i] = replacement;
			}
		}

		return out;
	}

	void replace_hook(std::string& str, const std::string& hook, const std::string& replacement)
	{
		size_t pos;
		while((pos = str.find(hook)) != std::string::npos)
		{
			str.replace(pos, hook.size(), replacement);

		}
	}

	std::string get_folder(const std::string& filepath)
	{
		unsigned int slashIndex = -1;
		for (unsigned int i = 0; i < filepath.size(); i++)
		{
			if(filepath[i] == '/')
			{
				slashIndex = i;
			}
		}
		if(slashIndex < filepath.size())
		{
			return filepath.substr(0, slashIndex);
		}

		return ".";
	}

	std::string get_filename(const std::string& filepath)
	{
		int slashIndex = -1;
		for (unsigned int i = 0; i < filepath.size(); i++)
		{
			if (filepath[i] == '/')
			{
				slashIndex = i;
			}
		}

		return filepath.substr(slashIndex + 1);
	}

	std::string get_extension(const std::string& filepath)
	{
		int slashIndex = -1, dotIndex = -1;
		for (unsigned int i = 0; i < filepath.size(); i++)
		{
			if (filepath[i] == '/')
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

	std::string get_basename(const std::string& filepath)
	{
		int slashIndex = -1, dotIndex = -1;
		for (unsigned int i = 0; i < filepath.size(); i++)
		{
			if (filepath[i] == '/')
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
}