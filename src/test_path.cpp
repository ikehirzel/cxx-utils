#include <hirzel/path.hpp>
#include <hirzel/print.hpp>
#include <iostream>
#include <cassert>

using namespace hirzel;
using namespace hirzel::path;
using namespace std::literals;

void test_paths()
{
	println("Home Dir	: $", homeDirectoryPath());
	println("Config	Dir	: $", configDirectoryPath());
	println("Executable	: $", executablePath());

	println("'$'", extensionOf("/usr/var/lib"));
}

void test_concatenate()
{

}

void test_parentOf()
{
	assert(parentOf("c:\\") == "C:\\");
	assert(parentOf("\\") == "\\");
	assert(parentOf("/") == "\\");
	assert(parentOf("hello") == ".");
	assert(parentOf("./hello") == ".");
	assert(parentOf("/hello/") == "\\");
	assert(parentOf("\\hello\\") == "\\");
	assert(parentOf("/var/lib/file.txt") == "\\var\\lib");
	assert(parentOf("/var/lib/file") == "\\var\\lib");
	assert(parentOf("/var/lib/folder/") == "\\var\\lib");
	assert(parentOf("/a/b/d/c/b/d/s/t.1.2.3.4") == "\\a\\b\\d\\c\\b\\d\\s");
}

void test_filenameOf()
{
	assert(filenameOf("hello", true) == "hello");
	assert(filenameOf("hello.exe", true) == "hello.exe");
	assert(filenameOf("hello", false) == "hello");
	assert(filenameOf("hello.exe", false) == "hello");
	assert(filenameOf("/var/lib/file.txt", true) == "file.txt");
	assert(filenameOf("/var/lib/file.txt", false) == "file");
	assert(filenameOf("/var/lib/file.txt.bak", true) == "file.txt.bak");
	assert(filenameOf("/var/lib/file.txt.bak", false) == "file");
	assert(filenameOf("/JsonValue.json", true) == "JsonValue.json");
	assert(filenameOf("/JsonValue.json", false) == "JsonValue");
}

void test_extensionOf()
{
	assert(extensionOf("file.txt") == "txt");
	assert(extensionOf("/var/lib/exec.bin") == "bin");
	assert(extensionOf("./image.png") == "png");
	assert(extensionOf("./img.1.png") == "1.png");
	assert(extensionOf("/usr/var/lib") == "");
	assert(extensionOf("/usr/var/lib.") == "");
	assert(extensionOf("/usr/var/lib.b") == "b");
	assert(extensionOf("/a/b/d/c/b/d/s/t.1.2.3.4") == "1.2.3.4");
}

int main(void)
{
	test_paths();
	test_concatenate();
	test_parentOf();
	test_filenameOf();
	test_extensionOf();

	return 0;
}
