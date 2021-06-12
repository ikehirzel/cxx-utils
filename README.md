# C++ Plugin library

A C++11 single-header cross-platform plugin handling library

It has no external depencies and therefore will simply work by just including **plugin.h**.

NOTE: Care must be taken when executing functions. Types of functions are not stored and 
are up to the user to handle

# Examples

### Example plugin
```c++
	// contents of exampleplugin.cpp
	#include <string>

	// extern "C" is required to avoid name mangling
	extern "C"
	{
		int double_value(int val)
		{
			return val * 2;
		}

		std::string hello(const std::string& name)
		{
			return "Hello, " + name + "!";
		}
	}
```

### Driver code
```c++
	// contents of main.cpp
	#include <iostream>
	#include <string>
	#include <hirzel/plugin.h>

	int main()
	{
		hirzel::Plugin p;

		p.bind_library("./exampleplugin.dll");
		p.bind_function("double_value");
		p.bind_function("hello");

		// loadLibrary and loadFunction can be handled in the constructor well:
		// hirzel::Plugin p("./exampleplugin.dll", { "double_value", "hello" });

		int dub = p.execute<int, int>("double_value", 5);
		string text = p.execute<string, string>("hello", "Ike");


		std::cout << dub << std::endl; // outputs 10
		std::cout << text << std::endl; // output 'Hello, Ike!'

		return 0;
	}
```

# License

Copyright 2021 Ike Hirzel

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

