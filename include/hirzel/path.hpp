#ifndef HIRZEL_PATH_HPP
#define HIRZEL_PATH_HPP

#include <string>

namespace hirzel::path
{
	// TODO: toRelative, toAbsolute
	extern const char directorySeparator;
	extern const char pathSeparator;

	const char* homeDirectoryPath();
	const char* configDirectoryPath();
	std::string executablePath();
	std::string sanitize(const char* path);
	std::string concatenate(const std::initializer_list<const char*> paths);
	std::string parentOf(const char* filepath);
	std::string filenameOf(const char* filepath, bool includeExtension = true);
	std::string extensionOf(const char* filepath);
}

#endif
