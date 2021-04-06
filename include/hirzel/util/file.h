#ifndef UTIL_FILE_H
#define UTIL_FILE_H

#include <vector>
#include <string>

namespace hirzel
{
	namespace file
	{
		std::vector<std::string> read_lines(const std::string& filename,
			 size_t first_line = 0, size_t n = -1);
		std::string read(const std::string& filename, bool ignore_empty_lines = false,
			const std::string& line_ending_fmt = "\n");
		void write(const std::string& filename, const std::string& buf);
		bool exists(const std::string& filepath);
	}
}

#endif // UTIL_FILE_H

#ifdef HIRZEL_UTIL_FILE_I
#undef HIRZEL_UTIL_FILE_I

#include <fstream>

namespace hirzel
{
	namespace file
	{
		std::vector<std::string> read_lines(const std::string &filename, size_t first_line, size_t n)
		{
			std::ifstream fin(filename);
			if (!fin.is_open()) return {};

			std::vector<std::string> lines(n);
			std::string line;
			size_t curr_line = 0;
			size_t lines_i = 0;

			while (std::getline(fin, line) && curr_line < first_line + n)
			{
				if (curr_line >= first_line)
				{
					if (line.back() == '\r') line.resize(line.size() - 1);
					lines[lines_i++] = line;
				}
				curr_line++;
			}

			fin.close();

			return lines;
		}

		std::string read(const std::string &filename, bool ignore_empty_lines, const std::string &line_ending_fmt)
		{
			std::string text, line;
			std::ifstream fin(filename);

			if (fin.is_open())
			{
				while (std::getline(fin, line))
				{
					if (line.empty() && ignore_empty_lines)
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

		void write(const std::string &filename, const std::string &buf)
		{
			std::ofstream file;
			file.open(filename);
			file << buf;
			file.close();
		}

		bool exists(const std::string &filepath)
		{
			std::ifstream file(filepath);
			return file.good();
		}
	} // namespace file
} // namespace hirzel

#endif // HIRZEL_UTIL_FILE_I