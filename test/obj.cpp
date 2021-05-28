#include <iostream>

extern "C"
{
	int i = 88;
	const char *msg = "this is a message";

	int get()
	{
		return i;
	}

	void add()
	{
		i += 1;
	}
}
