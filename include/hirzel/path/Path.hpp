#ifndef HIRZEL_PATH_PATH_HPP
#define HIRZEL_PATH_PATH_HPP

#include <string>

namespace hirzel::path
{
	class Path
	{
		char _buffer[4096];
		size_t _length;
		char _separator;
		char _root;

	private:

		std::string toString(size_t start, size_t length) const;

	public:

		Path(const char *path);

		void append(char c);
		void append(const char* path);
		void pop();

		std::string filename(bool includeExtension = true) const;
		std::string parent() const;
		std::string extension() const;
		std::string toString() const;

		char separator() const;
		bool isAbsolute() const { return !!_root; }
		bool isRelative() const { return !_root; }
	};
}

#endif
