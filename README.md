Fountain
========
A simple cross platform C++ info logging library -- like a fountain pen for your program

It relies on ascii escape sequences so if using windows, Windows 10 is required.

Example
========
```c++
	// contents of main.cpp
	#include <fountain.h>

	int main()
	{
		log_init("./report.log");
		log_info("This is an info message");
		log_dump(); // puts all logs into "./report.log"
		return 0;
	}
```
License
=======
MIT License © 2020 Ike Hirzel
