# C++ Utils

> NOTE: this project is still in active development, however it is not currently
ready for production use

# Brief

A suite of single-header utilities with broad functionality for use in C++ Applications. 

Included Utilities:

- **data.h**
	- A generic data container that is intended to be used in data file deserialization
	- Supports tables, arrays, boolean values, strings, integers, floating point numbers, and null values
- **json.h**
	- JSON de/serializer that follows JSON standard (keys with quotes, no comments, no trailing commas)
	- Makes use of hirzel::Data as the primary container for deserialized data
- **validation.h**
	- Data validator intended to assure the data within config files or data received from an API
	- Makes use of hirzel::Data as the primary target for validation
	- Supports specifying expected datatypes, acceptable ranges for numeric values nullable, and nullability of fields
- **file.h**
	- Provides simple API for handling simple file operations such as reading and writing
- **string.h**
	- Provides useful common funtions for manipulating strings, such as tokenizing, or getting different segments of a filepath
- **system.h**
	- Simple API for handling the underlying system, such as getting current OS name, unix timestamps, or making the current thread sleep.

# Current Status

As of right now, the data container itself as well as json parsing are effective
but still need further testing before being used in a production environment.
The projects is currently still in active development

# To Do

- **data.h**
	- Make use of safer system for different data types
- **json.h**
	- Add pretty-printing for JSON output
- **validation.h**
	- String regexes
- **file.h**
	- Implement functions for handling directories
- **string.h**
	- Reimplement tokenizing functions
- **system.h**
	- Stabilize API

# Dependencies

* C++11 Standard Library

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
