Fountain
========
A simple cross platform C++ info logging library.

It relies on ascii escape sequences so if using windows, Windows 10 is required.

Example
========
```c++
	// contents of main.cpp
	#define HIRZEL_FOUNTAIN_IMPLEMENTATION
	#include <hirzel/fountain.h>
	using namespace hirzel;
	int main()
	{
		fountain::init(true, true, "./report.log", 15);
		INFO("This is an info message");
		ERROR("This is a formatted %s\n", "error message");
		fountain::dump(); // puts all logs into "./report.log"
		return 0;
	}
```
License
=======
MIT License © 2020 Ike Hirzel
