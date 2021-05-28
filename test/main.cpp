// standard library
#include <assert.h>
#include <stdio.h>
#include <string.h>

// external libraries
#define HIRZEL_IMPLEMENT
#include <hirzel/plugin.h>

#ifdef HIRZEL_IMPLEMENT
#error Preprocess must be cleaned up after inclusion
#endif

#define DNE_OBJ		"./dne.so"
#define OBJ			"./obj.so"
#define DNE_SYM		"dne"
#define GET_FUNC	"get"
#define ADD_FUNC	"add"
#define MSG_VAR		"msg"
#define I_VAR		"i"

int main()
{
	hirzel::Plugin p;
	// assure empty string gives error
	assert(p.bind(""));
	assert(p.bind(DNE_OBJ));
	assert(!p.is_bound());
	assert(p.filepath().empty());

	assert(p.bind(DNE_OBJ));
	assert(!p.is_bound());
	assert(p.filepath().empty());
	assert(!p.contains(GET_FUNC));
	assert(!p.contains(ADD_FUNC));
	assert(!p.contains(MSG_VAR));
	assert(!p.contains(I_VAR));
	assert(!p.contains(DNE_SYM));


	// assure no error when binding
	assert(!p.bind(OBJ));

	// testing non-existent variable
	assert(p.bind_variable(DNE_SYM));
	assert(!p.get_variable_ptr(DNE_SYM));
	assert(!p.get_function(DNE_SYM));
	assert(!p.contains(DNE_SYM));
	assert(!p.contains_variable(DNE_SYM));
	assert(!p.contains_function(DNE_SYM));

	// testing non-existent function
	assert(p.bind_function(DNE_SYM));
	assert(!p.get_variable_ptr(DNE_SYM));
	assert(!p.get_function(DNE_SYM));
	assert(!p.contains(DNE_SYM));
	assert(!p.contains_variable(DNE_SYM));
	assert(!p.contains_function(DNE_SYM));

	// testing existing function
	assert(!p.bind_function(GET_FUNC));
	assert(p.contains(GET_FUNC));
	assert(p.contains_function(GET_FUNC));
	assert(!p.contains_variable(GET_FUNC));
	assert(p.get_function(GET_FUNC));
	assert(!p.get_variable_ptr(GET_FUNC));
	assert(p.execute<int>(GET_FUNC) == 88);
	
	assert(!p.bind_function(ADD_FUNC));
	assert(p.contains(ADD_FUNC));
	assert(p.contains_function(ADD_FUNC));
	assert(!p.contains_variable(ADD_FUNC));
	assert(p.get_function(ADD_FUNC));
	assert(!p.get_variable_ptr(ADD_FUNC));
	p.execute<void>(ADD_FUNC);
	assert(p.execute<int>(GET_FUNC) == 89);


	// testing existing global variable
	assert(!p.bind_variable(MSG_VAR));
	assert(p.contains(MSG_VAR));
	assert(p.contains_variable(MSG_VAR));
	assert(!p.contains_function(MSG_VAR));
	assert(!p.get_function(MSG_VAR));
	assert(p.get_variable_ptr(MSG_VAR));
	assert(!strcmp("this is a message", p.get_variable_val<const char *>(MSG_VAR)));

	assert(!p.bind_variable(I_VAR));
	assert(p.contains(I_VAR));
	assert(p.contains_variable(I_VAR));
	assert(!p.contains_function(I_VAR));
	assert(!p.get_function(I_VAR));
	assert(p.get_variable_ptr(I_VAR));
	assert(p.get_variable_val<int>(I_VAR) == 89);

	int(*f)() = (decltype(f))p.get_function(GET_FUNC);
	assert(f() == 89);

	puts("All tests passed");
	return 0;
}
