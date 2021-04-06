#define HIRZEL_UTIL_SYS_I
#include <hirzel/util/sys.h>
#define HIRZEL_UTIL_FILE_I
#include <hirzel/util/file.h>
#define HIRZEL_UTIL_STR_I
#include <hirzel/util/str.h>

#include <cassert>

using namespace hirzel;

int main()
{
	std::cout << "Time: " << sys::epoch_seconds() << std::endl;
	assert(file::exists("./main.cpp"));
	assert(!file::exists("./dne.txt"));
	auto s = file::read("./main.cpp");
	assert(!s.empty());
	auto lines = file::read_lines("./main.cpp", 1,3);
	assert(!lines.empty());
	for (size_t i = 0; i < lines.size(); i++)
	{
		std::cout << "line " << i << ": " << lines[i] << std::endl;
	}

	auto v = str::tokenize("a,b,c,d,e,f", ",", false, false);
	for (const std::string& s : v)
	{
		std::cout << s << std::endl;
	}
	return 0;
}
