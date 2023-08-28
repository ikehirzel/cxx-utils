#ifndef HIRZEL_FILE_HPP
#define HIRZEL_FILE_HPP

#include <vector>
#include <string>

namespace hirzel
{
	namespace file
	{
		std::vector<std::string> read_lines(const std::string& filepath,
			 size_t first_line_index = 0, size_t n = 0);
		std::string read(const std::string& filepath,
			const std::string& line_ending = "\n");
		bool write(const std::string& filepath, const std::string& buf);
		bool exists(const std::string& filepath) noexcept;

		class IoException : public std::exception
		{
		private:
		
			std::string _msg;
			IoException(const std::string& msg) : _msg(msg) {}

		public:
			static IoException dne(const std::string& filename) noexcept
			{
				return IoException("failed to open file: " + filename);
			}
			const char *what() const noexcept { return _msg.c_str(); }
		};
	}
}

#endif

#if defined(HIRZEL_IMPLEMENT) && !defined(HIRZEL_FILE_I)
#define HIRZEL_FILE_I

#include <fstream>
#include <iostream>

namespace hirzel
{
	namespace file
	{
		std::vector<std::string> read_lines(const std::string &filepath, size_t first_line, size_t n)
		{
			std::ifstream fin(filepath);

			if (!fin.is_open()) throw IoException::dne(filepath);

			std::vector<std::string> lines(n);
			std::string line;
			size_t curr_line = 0;
			size_t lines_i = 0;

			while (std::getline(fin, line) && curr_line < first_line + n)
			{
				if (curr_line >= first_line)
				{
					if (line.back() == '\r') line.resize(line.length() - 1);
					lines[lines_i++] = line;
				}
				curr_line++;
			}

			fin.close();

			return lines;
		}


		std::string read(const std::string& filepath,
			const std::string& line_ending)
		{
			std::ifstream file(filepath);
			// confirm file exists
			if (!file.is_open()) throw IoException::dne(filepath);

			// create output buffer
			file.seekg(0, std::ios::end);
			std::string out;
			out.reserve(file.tellg());
			file.seekg(0, std::ios::beg);

			std::string line;
			
			// copy data into buffer
			while (std::getline(file, line))
			{
				// pop windows line ending
				if (line.back() == '\r') line.pop_back();

				// push line ending
				out += line;
				out += line_ending;
			}

			// close file
			file.close();

			out.shrink_to_fit();

			return out;
		}


		bool write(const std::string &filepath, const std::string &buf)
		{
			std::ofstream file(filepath);
			if (!file.is_open()) throw IoException::dne(filepath);
			file << buf;
			bool success = file.good();
			file.close();
			return success;
		}


		bool exists(const std::string &filepath) noexcept
		{
			std::ifstream file(filepath);
			return file.good();
		}
	} // namespace file
} // namespace hirzel

#endif // HIRZEL_UTIL_FILE_I