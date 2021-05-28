#include <iostream>

extern "C"
{
	int i = 88;
	const char *msg = "this is a message";

	double var0 = 0.0;
	double var1 = 1.0;
	double var2 = 2.0;

	int get()
	{
		return i;
	}

	void add()
	{
		i += 1;
	}

	int func0()
	{
		return 1;
	}

	int func1()
	{
		return 1;
	}

	int func2()
	{
		return 1;
	}
}
