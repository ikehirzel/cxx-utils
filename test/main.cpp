#define HIRZEL_PLUGIN_I
#include <hirzel/plugin.h>

int main()
{
	hirzel::Plugin p("./dne.so");
	if (p.error())
	{
		std::cout << p.error() << std::endl;
	}
	return 0;
}
