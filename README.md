HLog
========
A simple C++ info logging library

As of right now, it is only supported on UNIX-like systems due to relying on ascii escape sequences.

I have future plans to extend this to support the windows api.

Example
========
```c++
	// contents of exampleplugin.cpp
	#include "hlog.h"

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
