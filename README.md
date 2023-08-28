
# C++ Utils

> NOTE: this project is still in active development, however it is not currently
ready for production use

# Brief

A suite of single-header, cross-platform utilities with broad functionality for use in C++ applications

Included Utilities:

- **JsonValue.hpp**
	- A generic JsonValue container that is intended to be used in JsonValue file deserialization
	- Supports tables, arrays, boolean values, strings, integers, floating point numbers, and null values
- **json.hpp**
	- JSON de/serializer that follows JSON standard (keys with quotes, no comments, no trailing commas)
	- Makes use of hirzel::JsonValue as the primary container for deserialized JsonValue
- **validation.hpp**
	- JsonValue validator intended to assure the JsonValue within config files or JsonValue received from an API
	- Makes use of hirzel::JsonValue as the primary target for validation
	- Supports specifying expected ValueTypes, acceptable ranges for numeric values nullable, and nullability of fields
- **plugin.hpp**
	- Provides API for loading dynamic libraries and their associated global JsonValue and functions.
- **logger.hpp**
	- Logger with urgency levels and consistent layout to allow for use with stream manipulator such as grep or select-string
- **file.hpp**
	- Provides API for handling simple file operations such as reading and writing
- **string.hpp**
	- Provides useful common funtions for manipulating strings, such as tokenizing, or getting different segments of a filepath
- **system.hpp**
	- Provides API for handling the underlying system, such as getting current OS name, unix timestamps, or making the current thread sleep.

# Current Status

As of right now, the JsonValue container itself as well as json parsing are effective
but still need further testing before being used in a production environment.
The projects is currently still in active development

# To Do

- **JsonValue.hpp**
	- Make use of safer system for different JsonValue types
- **json.hpp**
	- Add pretty-printing for JSON output
- **validation.hpp**
	- String regexes
- **file.hpp**
	- Implement functions for handling directories
- **plugin.hpp**
	- Find way to implement type safety
- **logger.hpp**
	- Code cleanup
- **string.hpp**
	- Reimplement tokenizing functions
	- Make C++11 compliant
- **system.hpp**
	- Make c++11 compliant

# Dependencies

* C++11 Standard Library
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
