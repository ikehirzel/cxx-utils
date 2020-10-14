Logger
========
A simple cross platform C++ info logging library

It relies on ascii escape sequences so if using windows, Windows 10 is required.

Example
========
```c++
	// contents of main.cpp
	#include <logger.h>

	int main()
	{
		hirzel::Logger l;
		l.info("This is an info message");
		l.dump(); // puts all logs into "./report.log"
		return 0;
	}
```
License
=======
MIT License © 2020 Ike Hirzel
