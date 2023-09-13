#include <hirzel/file.hpp>
#include <fstream>
#include <iostream>

namespace hirzel::file
{
	std::vector<std::string> readLines(const std::string& filepath, size_t firstLineIndex, size_t n)
	{
		std::ifstream fin(filepath);

		if (!fin.is_open()) throw IoException::dne(filepath);

		std::vector<std::string> lines(n);
		std::string line;
		size_t curr_line = 0;
		size_t lines_i = 0;

		while (std::getline(fin, line) && curr_line < firstLineIndex + n)
		{
			if (curr_line >= firstLineIndex)
			{
				if (line.back() == '\r') line.resize(line.length() - 1);
				lines[lines_i++] = line;
			}
			curr_line++;
		}

		fin.close();

		return lines;
	}


	std::string read(const std::string& filepath, const char *lineEnding)
	{
		if (lineEnding == nullptr)
			lineEnding = "\n";

		std::ifstream file(filepath);
		// confirm file exists
		if (!file.is_open()) throw IoException::dne(filepath);

		// create output buffer
		file.seekg(0, std::ios::end);
		std::string out;
		out.reserve(file.tellg());
		file.seekg(0, std::ios::beg);

		std::string line;

		// copy Value into buffer
		while (std::getline(file, line))
		{
			// pop windows line ending
			if (line.back() == '\r') line.pop_back();

			// push line ending
			out += line;
			out += lineEnding;
		}

		// close file
		file.close();

		out.shrink_to_fit();

		return out;
	}


	bool write(const std::string& filepath, const std::string& buf)
	{
		std::ofstream file(filepath);
		if (!file.is_open()) throw IoException::dne(filepath);
		file << buf;
		bool success = file.good();
		file.close();
		return success;
	}


	bool exists(const std::string& filepath) noexcept
	{
		std::ifstream file(filepath);
		return file.good();
	}
}