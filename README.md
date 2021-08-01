C++ Logger
========
A simple cross platform C++ info logging library.

It relies on ascii escape sequences so if using windows, Windows 10 is required.

Example
========
```c++
	// contents of main.cpp
	#define HIRZEL_IMPLEMENTATION
	#include <hirzel/logger.h>

	int main()
	{
		hirzel::Logger log("Main");
		
		log.println("Hello, world!");
		log.info("This is an {} message". { "info" });
		log.error("This is a formatted {}", { "error message" });
		
		return 0;
	}
```
License
=======
MIT License © 2020 Ike Hirzel
