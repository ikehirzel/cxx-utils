#ifndef HIRZEL_PATH_HPP
#define HIRZEL_PATH_HPP

#include <hirzel/path/Path.hpp>

namespace hirzel::path
{
	// TODO: toRelative, toAbsolute
	extern const char directorySeparator;
	extern const char pathSeparator;

	const char* homeDirectoryPath();
	const char* configDirectoryPath();
	std::string executablePath();
	std::string sanitize(const char* path);
	std::string parentOf(const char* filepath);
	std::string filenameOf(const char* filepath, bool includeExtension = true);
	std::string extensionOf(const char* filepath);

	template <typename ...Arg>
	std::string concatenate(Arg const&... paths)
	{
		auto path = Path();

		for (const auto& p : { paths... })
			path.append(p);

		return path.toString();
	}
}

#endif
