#define UTILS_SYS_IMPLEMENTATION
#include <hirzel/utils/sys.h>
#define UTILS_FILE_IMPLEMENTATION
#include <hirzel/utils/file.h>
#define UTILS_STR_IMPLEMENTATION
#include <hirzel/utils/str.h>

int main()
{
	auto v = hirzel::str::tokenize("a,b,c,d,e,f", ",", false, false);
	for (const std::string& s : v)
	{
		std::cout << s << std::endl;
	}
	return 0;
}
