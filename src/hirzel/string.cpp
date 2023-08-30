#include <hirzel/string.hpp>

namespace hirzel::string
{
	size_t lastSlashIndexOf(const char* filepath) noexcept
	{
		size_t slash_index = 0;

		for (size_t i = 0; filepath[i]; ++i)
		{
			bool is_dir_char = filepath[i] == '/';

#ifdef _WIN32
			is_dir_char = is_dir_char || filepath[i] == '\\';
#endif

			if (is_dir_char)
				slash_index = i + 1;
		}

		return slash_index;
	}

	size_t firstDotIndexOf(const char* filepath) noexcept
	{
		size_t size = 0;

		while (filepath[size])
		{
			if (filepath[size] == '.')
				break;

			++size;
		}

		return size;
	}

	std::string directoryOf(const char* filepath)
	{
		size_t slash_index = lastSlashIndexOf(filepath);

		return slash_index > 0
			? std::string(filepath, slash_index)
			: "./";
	}

	std::string filenameOf(const char* filepath, bool include_extension)
	{
		size_t slash_index = lastSlashIndexOf(filepath);
		const char* filename = filepath + slash_index;

		size_t size = 0;

		if (include_extension)
		{
			while (filename[size])
				++size;
		}
		else
		{
			size = firstDotIndexOf(filename);
		}

		return std::string(filename, size);
	}

	std::string fileExtensionOf(const char* filepath)
	{
		size_t slash_index = lastSlashIndexOf(filepath);
		const char* filename = filepath + slash_index;

		size_t dot_index = firstDotIndexOf(filename);
		const char* extension = filename + dot_index;

		size_t size = 0;

		while (extension[size])
			++size;

		return std::string(extension, size);
	}
}
