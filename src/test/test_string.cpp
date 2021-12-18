#define HIRZEL_IMPLEMENT
#include <hirzel/string.h>

#include <iostream>
#include <assert.h>

using namespace hirzel::string;

void test_get_folder()
{
	#define test(path, folder) assert(hirzel::string::get_folder(path) == folder)
	std::cout << "\thirzel::string::get_folder()\n";

	test("hello", "./");
	test("./hello", "./");
	test("/hello", "/");
	test("/var/lib/file.txt", "/var/lib/");
	test("/var/lib/file", "/var/lib/");
	test("/var/lib/folder/", "/var/lib/folder/");

	#undef test
}

void test_get_filename()
{
	#define test(path, filename, include_extension) assert(hirzel::string::get_filename(path, include_extension) == filename)
	std::cout << "\thirzel::string::get_filename()\n";

	test("hello", "hello", true);
	test("hello.exe", "hello.exe", true);
	test("hello", "hello", false);
	test("hello.exe", "hello", false);
	test("/var/lib/file.txt", "file.txt", true);
	test("/var/lib/file.txt", "file", false);
	test("/var/lib/file.txt.bak", "file.txt.bak", true);
	test("/var/lib/file.txt.bak", "file", false);
	test("/data.json", "data.json", true);
	test("/data.json", "data", false);

	#undef test
}

void test_get_extension()
{
	#define test(path, expected) assert(hirzel::string::get_extension(path) == expected)
	std::cout << "\thirzel::string::get_extension()\n";

	test("file.txt", ".txt");
	test("/var/lib/exec.bin", ".bin");
	test("./image.png", ".png");
	test("./img.1.png", ".1.png");
	test("/usr/var/lib", "");
	test("/usr/var/lib.", ".");
	test("/usr/var/lib.b", ".b");
	test("/a/b/d/c/b/d/s/t.1.2.3.4", ".1.2.3.4");

	#undef test
}

int main(void)
{
	std::cout << "Testing hirzel::string functions...\n";

	test_get_folder();
	test_get_filename();
	test_get_extension();

	std::cout << "All tests passed\n";

	return 0;
}
