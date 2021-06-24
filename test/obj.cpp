#include <iostream>

#if defined(_WIN32) || defined(_WIN64)
#define OS_IS_WINDOWS true
#else
#define OS_IS_WINDOWS false
#endif


extern "C"
{
	#if OS_IS_WINDOWS
	__declspec(dllexport)
	#endif
	int i = 88;
	#if OS_IS_WINDOWS
	__declspec(dllexport)
	#endif
	const char *msg = "this is a message";

	#if OS_IS_WINDOWS
	__declspec(dllexport)
	#endif
	double var0 = 0.0;
	#if OS_IS_WINDOWS
	__declspec(dllexport)
	#endif
	double var1 = 1.0;
	#if OS_IS_WINDOWS
	__declspec(dllexport)
	#endif
	double var2 = 2.0;

	#if OS_IS_WINDOWS
	__declspec(dllexport)
	#endif
	int get()
	{
		return i;
	}

	#if OS_IS_WINDOWS
	__declspec(dllexport)
	#endif
	void add()
	{
		i += 1;
	}

	#if OS_IS_WINDOWS
	__declspec(dllexport)
	#endif
	int func0()
	{
		return 1;
	}


	#if OS_IS_WINDOWS
	__declspec(dllexport)
	#endif
	int func1()
	{
		return 1;
	}

	#if OS_IS_WINDOWS
	__declspec(dllexport)
	#endif
	int func2()
	{
		return 1;
	}
}
