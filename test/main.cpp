// standard library
#include <assert.h>
#include <stdio.h>

// external libraries
#define HIRZEL_IMPLEMENT
#include <hirzel/plugin.h>

#ifdef HIRZEL_IMPLEMENT
#error Preprocess must be cleaned up after inclusion
#endif

int main()
{
	hirzel::Plugin p;
	// assure empty string gives error
	assert(p.bind(""));
	assert(!p.is_bound());
	assert(p.filepath().empty());

	assert(p.bind("./dne.so"));
	assert(!p.is_bound());
	assert(p.filepath().empty());
	assert(!p.contains("function"));


	// assure no error when binding
	assert(!p.bind("./obj.so"));

	// testing non-existent symbol
	assert(p.bind_symbol("dne"));
	assert(!p.get_symbol("dne"));
	assert(!p.contains("dne"));

	// testing with existing function
	assert(!p.bind_symbol("function"));
	assert(p.contains("function"));
	assert(p.get_symbol("function"));
	assert(p.execute<int>("function") == 88);

	// testing with existing global variable
	assert(!p.bind_symbol("msg"));
	assert(p.contains("msg"));
	assert(p.get_symbol("msg"));
	puts(p.get<const char *>("msg"));


	puts("All tests passed");
	return 0;
}
