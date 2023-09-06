
# C++ Utils

> NOTE: this project is still in active development, however it is not currently
ready for production use

# Brief

A suite of single-header, cross-platform utilities with broad functionality for use in C++ applications

Included Utilities:

- **json.hpp**
	- JSON parser and deserializer
- **plugin.hpp**
	- Provides API for loading dynamic libraries and their associated global JsonValue and functions.
- **logger.hpp**
	- Logger with urgency levels and consistent layout to allow for use with stream manipulator such as grep or select-string
- **path.hpp**
	- Functions that provide common paths and utils for handling path combination
- **file.hpp**
	- Provides API for handling simple file operations such as reading and writing
- **system.hpp**
	- Utility to get current OS as enum
- **primitives.hpp**
	- A set of renamed primitives with size specificity
- **time.hpp**
	- Utilities related to getting time and sleeping thread
- **print.hpp**
	- Variadic print function that makes use of iostreams

# Current Status

As of right now, the JsonValue container itself as well as json parsing are effective
but still need further testing before being used in a production environment.
The projects is currently still in active development

# To Do

- **JSON**
	- Implement validation using JSON Schema
- **file.hpp**
	- Implement functions for handling directories
- **plugin.hpp**
	- Reimplement
- **logger.hpp**
	- Code cleanup

# Dependencies

* C++17 Standard Library
* OS libraries (POSIX, Win32) depending on system

# License

Copyright 2023 Isaac Hirzel

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
