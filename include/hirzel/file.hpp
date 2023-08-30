#ifndef HIRZEL_FILE_HPP
#define HIRZEL_FILE_HPP

#include <vector>
#include <string>

namespace hirzel::file
{
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

	std::vector<std::string> readLines(const std::string& filepath, size_t firstLineIndex = 0, size_t n = 0);
	std::string read(const std::string& filepath, const char* lineEnding = "\n");
	bool write(const std::string& filepath, const std::string& buffer);
	bool exists(const std::string& filepath) noexcept;
}

#endif
