#include "fileutil.h"

#include <iostream>
#include <fstream>

#include "strutil.h"

namespace hirzel
{
	std::vector<std::string> read_file(const std::string& filename)
	{
		std::string line;
		std::vector<std::string> lines;
		std::ifstream fin(filename);
		
		if (fin.is_open())
		{
			while (std::getline(fin, line))
			{
				if(line.empty())
				{
					continue;
				}
				if (line.back() == '\r')
				{
					line.resize(line.size() - 1);
				}
				lines.push_back(line);
			}
			fin.close();
		}
		else
		{
			#ifdef HXUTILS_DEBUG
			std::cout << "FileUtil::read_file() : Failed to open '" + filename + "'" << std::endl;
			#endif
		}
		return lines;
	}
	
	std::string read_string(const std::string& filename, const std::string& line_ending_fmt,
		bool ignore_empty_lines)
	{
		std::string text, line;
		std::ifstream fin(filename);
		
		if (fin.is_open())
		{
			while (std::getline(fin, line))
			{
				if(line.empty() && ignore_empty_lines)
				{
					continue;
				}

				while (line.back() == '\r' || line.back() == '\n')
				{
					line.pop_back();
				}

				text += line + line_ending_fmt;
			}

			fin.close();
		}

		return text;
	}

	std::vector<std::vector<std::string>> read_csv(const std::string& filename)
	{
		std::vector<std::string> lines = read_file(filename);
		std::vector<std::vector<std::string>> tokens;

		for (std::string str : lines)
		{
			tokens.push_back(tokenize(str, ",", false, false));
		}
		return tokens;
	}

	void write_file(const std::string& filename, const std::string& buf)
	{
		std::ofstream file;
		file.open(filename);
		file << buf;
		file.close();
	}

	bool file_exists(const std::string& filepath)
	{
		std::ifstream file(filepath);
		return file.good();
	}
}