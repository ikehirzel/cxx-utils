#define HIRZEL_VAR_IMPLEMENTATION
#include <hirzel/var.h>
#include <iostream>
int main()
{
	char buf[16] = "This is buf\n";
	std::cout << buf;
	hirzel::var j = 12;
	std::cout << j.as_string() << std::endl;
	j = 12U;
	std::cout << j.as_string() << std::endl;
	j = true;
	std::cout << j.as_string() << std::endl;
	j = 'c';
	std::cout << j.as_string() << std::endl;
	j = 12.45f;
	std::cout << j.as_string() << std::endl;
	j = 12.6276;
	std::cout << j.as_string() << std::endl;
	j = "GOBO";
	std::cout << j.as_string() << std::endl;
	return 0;
}