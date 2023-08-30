#include <hirzel/system.hpp>
#include <stdexcept>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

namespace hirzel::system
{
	std::string pathToCurrentExecutable()
	{
#if HIRZEL_CURRENT_OS == Windows

		char buf[MAX_PATH + 1];
		GetModuleFileNameA(NULL, buf, MAX_PATH);
		return std::string(buf);

#elif HIRZEL_CURRENT_OS == Linux

		char buf[512];
		readlink("/proc/self/exe", buf, sizeof(buf) / sizeof(char) - 1);
		return std::string(buf);

#else
		throw std::runtime_error("Unable to get executable path for OS: " + std::string(currentOsName));
#endif
	}

	std::string pathToCurrentExecutableDirectory()
	{
		auto executablePath = pathToCurrentExecutable();

		for (usize i = executablePath.size(); i-- > 0;)
		{
			if (executablePath[i] != HIRZEL_PATH_SEPARATOR)
				continue;

			executablePath.resize(i + 1);

			return executablePath;
		}

		throw std::runtime_error("Failed to get directory of path: " + executablePath);
	}
}
