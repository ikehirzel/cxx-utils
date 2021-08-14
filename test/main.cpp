// headers
#include <hirzel/util/sys.h>
#include <hirzel/util/file.h>
#include <hirzel/util/str.h>

// standard library
#include <cassert>
#include <cstdio>
#include <iostream>

using namespace hirzel;

#define assert_throws(x, type) { bool throws = false; try { x; } catch(const type& e) { throws = true; } assert(throws); }
#define assert_not_throws(x, type) { bool does_not_throw = true; try { x; } catch(const type& e) { does_not_throw = false; } assert(does_not_throw); }

void test_file()
{
	std::cout << "Testing file...\n";

	// testing exists
	std::cout << "\texists\n";
	assert(file::exists("./main.cpp"));
	assert(!file::exists("./dne.txt"));
	assert(!file::exists(""));

	// testing read
	std::cout << "\tread\n";
	assert_not_throws(file::read("./main.cpp"), file::IoException);
	assert_throws(file::read("./dne.txt"), file::IoException);
	
	std::string s;
	assert_not_throws(s = file::read("./main.cpp"), file::IoException);
	assert(!s.empty());
	// assert there is no windows line endings
	for (char c : s) assert(c != '\r');

	// testing basic line reading
	assert_not_throws(s = file::read("./main.cpp", "\r\n"), file::IoException);
	assert(!s.empty());
	bool has_carriage_return = false;
	for (char c : s) if (c == '\r') has_carriage_return = true;
	assert(has_carriage_return);

	std::cout << "\tread lines\n";
	auto lines = file::read_lines("./main.cpp", 1,3);
	assert(!lines.empty());

	std::cout << "All tests passed\n";
}


void test_str()
{
	std::cout << "\nTesting str...\n";

	std::cout << "\ttokenize\n";
	std::string str = "a:b,c:d,e:f";
	{
		auto v = str::tokenize(str, { ",", ":" });
		std::cout << "tokens: ";
		for (const std::string& s : v) std::cout << s << " ";
		std::cout << std::endl;
	}

	std::cout << "replace: " << str::replace_tokens(str, { ",", ":" }, "X") << std::endl;

	std::cout << "All tests passed\n\n";
}


void test_sys()
{
	std::cout << "Testing sys...\n";
	std::cout << "All tests passed\n\n";
}

#include <fstream>

int main()
{
	std::cout << "Currently running on " << sys::current_os() << std::endl;
	std::cout << "Path to executable: " << sys::executable_path() << std::endl;
	std::cout << "Dir of executable: " << sys::executable_dir() << std::endl;
	test_file();
	test_str();
	test_sys();
	return 0;
}
