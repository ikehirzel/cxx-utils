#include <hirzel/path/Path.hpp>
#include <hirzel/primitives.hpp>
#include <cctype>
#include <cassert>

#ifdef _WIN32
#define DIRECTORY_SEPARATOR	('\\')
#else
#define DIRECTORY_SEPARATOR	('/')
#endif

namespace hirzel::path
{
	static inline bool isDirectorySeparator(char c)
	{
#ifdef _WIN32
		return c == '\\' || c == '/';
#else
		return c == '/';
#endif
	}

	static inline char rootOf(const char* path)
	{
#ifdef _WIN32
		if (isalpha(path[0]) && path[1] == ':')
			return (char)toupper(path[0]);
#endif

		if (isDirectorySeparator(path[0]))
			return ' ';

		return '\0';
	}

	Path::Path(const char* path) :
	_buffer(""),
	_length(0),
	_root(rootOf(path))
	{
		append(path);
	}

	void Path::append(char c)
	{
		_buffer[_length] = c;
		_length += 1;
	}

	void Path::append(const char* path)
	{
		assert(path != nullptr);

		if (path[0] == '\0')
			return;

		auto start = 0;
		auto pathRoot = rootOf(path);

		if (pathRoot)
		{
			_root = pathRoot;
			_length = 0;

#ifdef _WIN32
			if (isalpha(_root))
				start = 2;
#endif
		}

		auto needsSlash = _length > 0;

		for (usize i = start; path[i]; ++i)
		{
			if (isDirectorySeparator(path[i]))
			{
				if (_length == 0)
					continue;

				needsSlash = true;
				continue;
			}

			if (needsSlash)
			{
				append(DIRECTORY_SEPARATOR);
				needsSlash = false;
			}

			append(path[i]);
		}

		_buffer[_length] = '\0';
	}

	void Path::pop()
	{
		if (_root && _length == 0)
			return;

		for (usize i = _length; i-- > 0;)
		{
			if (_buffer[i] == DIRECTORY_SEPARATOR)
			{
				_length = i;
				_buffer[_length] = '\0';
				return;
			}
		}

		_buffer[_length] = '\0';
		_length = 0;
	}

	std::string Path::filename(bool includeExtension) const
	{
		if (_length == 0)
			return "";

		size_t dotIndex = SIZE_MAX;
		size_t slashIndex = SIZE_MAX;

		for (usize i = _length; i-- > 0;)
		{
			char c = _buffer[i];

			if (c == '.')
			{
				dotIndex = i;
				continue;
			}

			if (_buffer[i] == DIRECTORY_SEPARATOR)
			{
				slashIndex = i;
				break;
			}
		}

		size_t start = slashIndex != SIZE_MAX
			? slashIndex + 1
			: 0;
		size_t end = !includeExtension && dotIndex != SIZE_MAX
			? dotIndex
			: _length;

		return std::string(&_buffer[start], end - start);
	}

	std::string Path::parent() const
	{
		for (usize i = _length; i-- > 0;)
		{
			if (_buffer[i] == DIRECTORY_SEPARATOR)
				return toString(0, i);
		}

		return toString(0, 0);
	}

	std::string Path::extension() const
	{
		size_t dotIndex = SIZE_MAX;
	
		for (usize i = _length; i-- > 0;)
		{
			if (_buffer[i] == '.')
			{
				dotIndex = i;
				continue;
			}

			if (_buffer[i] == DIRECTORY_SEPARATOR)
				break;
		}

		if (dotIndex == SIZE_MAX)
			return "";

		size_t start = dotIndex + 1;

		return std::string(&_buffer[start], _length - start);
	}

	std::string Path::toString(size_t start, size_t length) const
	{
		if (length == 0 && !_root)
			return ".";

		std::string out;

		out.reserve(3 + _length);

#ifdef _WIN32

		if (isalpha(_root))
		{
			out += _root;
			out += ':';
		}
#endif

		if (_root)
			out += DIRECTORY_SEPARATOR;

		out.append(&_buffer[start], length);

		return out;
	}

	std::string Path::toString() const
	{
		return toString(0, _length);
	}
}
