// standard library
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

// external libraries
#define HIRZEL_IMPLEMENT
#include <hirzel/plugin.h>

#if OS_IS_WINDOWS
#define DNE_OBJ		"./dne.dll"
#define OBJ			"./obj.dll"
#else
#define OBJ			"./obj.so"
#define DNE_OBJ		"./dne.so"
#endif

#define DNE_LIST	{ "dne0", "dne1", "dne2" }
#define VAR_LIST	{ "var0", "var1", "var2" }
#define FUNC_LIST	{ "func0", "func1", "func2" }
#define DNE_SYM		"dne"
#define GET_FUNC	"get"
#define ADD_FUNC	"add"
#define MSG_VAR		"msg"
#define I_VAR		"i"

#define assert_throws(x) { bool throws = false; try { x; } catch (const std::exception& e) { throws = true; } assert(throws); }

int main()
{
	hirzel::Plugin p;
	// assure empty string gives error
	assert_throws(p.bind(""));
	assert_throws(p.bind(DNE_OBJ));
	assert(!p.is_bound());
	assert(p.filepath().empty());

	assert(!p.contains(GET_FUNC));
	assert(!p.contains(ADD_FUNC));
	assert(!p.contains(MSG_VAR));
	assert(!p.contains(I_VAR));
	assert(!p.contains(DNE_SYM));


	// assert no throw when binding
	p.bind(OBJ);
	// assert throw when double binding
	assert_throws(p.bind(OBJ));
	assert(p.is_bound());
	assert(p.count() == 0);
	assert(p.function_count() == 0);
	assert(p.variable_count() == 0);

	// testing non-existent variable
	assert_throws(p.bind_variable(DNE_SYM));
	assert(!p.get_variable_ptr(DNE_SYM));
	assert(!p.get_function(DNE_SYM));
	assert(!p.contains(DNE_SYM));
	assert(!p.contains_variable(DNE_SYM));
	assert(!p.contains_function(DNE_SYM));
	assert(p.count() == 0);
	assert(p.function_count() == 0);
	assert(p.variable_count() == 0);

	// testing non-existent function
	assert_throws(p.bind_function(DNE_SYM));
	assert(!p.get_variable_ptr(DNE_SYM));
	assert(!p.get_function(DNE_SYM));
	assert(!p.contains(DNE_SYM));
	assert(!p.contains_variable(DNE_SYM));
	assert(!p.contains_function(DNE_SYM));
	assert(p.count() == 0);
	assert(p.function_count() == 0);
	assert(p.variable_count() == 0);

	// testing existing function
	p.bind_function(GET_FUNC);
	assert_throws(p.bind_function(GET_FUNC));
	assert(p.count() == 1);
	assert(p.function_count() == 1);
	assert(p.variable_count() == 0);
	assert(p.contains(GET_FUNC));
	assert(p.contains_function(GET_FUNC));
	assert(!p.contains_variable(GET_FUNC));
	assert(!p.get_variable_ptr(GET_FUNC));
	assert_throws(p.get_variable_val<int>(GET_FUNC));
	assert(p.get_function(GET_FUNC));
	assert(!p.get_variable_ptr(GET_FUNC));
	assert(p.execute<int>(GET_FUNC) == 88);
	
	p.bind_function(ADD_FUNC);
	assert_throws(p.bind_function(ADD_FUNC));
	assert(p.count() == 2);
	assert(p.function_count() == 2);
	assert(p.variable_count() == 0);
	assert(p.contains(ADD_FUNC));
	assert(p.contains_function(ADD_FUNC));
	assert(!p.contains_variable(ADD_FUNC));
	assert(!p.get_variable_ptr(ADD_FUNC));
	assert_throws(p.get_variable_val<int>(ADD_FUNC));
	assert(p.get_function(ADD_FUNC));
	assert(!p.get_variable_ptr(ADD_FUNC));
	p.execute<void>(ADD_FUNC);
	assert(p.execute<int>(GET_FUNC) == 89);


	// testing existing global variable
	p.bind_variable(MSG_VAR);
	assert_throws(p.bind_variable(MSG_VAR));
	assert(p.count() == 3);
	assert(p.function_count() == 2);
	assert(p.variable_count() == 1);
	assert(p.contains(MSG_VAR));
	assert(p.contains_variable(MSG_VAR));
	assert(!p.contains_function(MSG_VAR));
	assert(!p.get_function(MSG_VAR));
	assert_throws(p.execute<void>(MSG_VAR));
	assert(p.get_variable_ptr(MSG_VAR));
	assert(!strcmp("this is a message", p.get_variable_val<const char *>(MSG_VAR)));

	p.bind_variable(I_VAR);
	assert_throws(p.bind_variable(I_VAR));
	assert(p.count() == 4);
	assert(p.function_count() == 2);
	assert(p.variable_count() == 2);
	assert(p.contains(I_VAR));
	assert(p.contains_variable(I_VAR));
	assert(!p.contains_function(I_VAR));
	assert(!p.get_function(I_VAR));
	assert_throws(p.execute<void>(I_VAR));
	assert(p.get_variable_ptr(I_VAR));
	assert(p.get_variable_val<int>(I_VAR) == 89);

	// testing multiple non-existent functions / variables
	
	assert_throws(p.bind_functions(DNE_LIST));
	assert(p.count() == 4);
	assert(p.function_count() == 2);
	assert(p.variable_count() == 2);

	for (const std::string& label : DNE_LIST)
	{
		assert(!p.contains(label));
		assert(!p.contains_function(label));
		assert(!p.contains_variable(label));
		assert(!p.get_function(label));
		assert(!p.get_variable_ptr(label));
	}

	// testing with multiple existing variables
	for (const std::string& label : VAR_LIST)
	{
		assert(!p.contains(label));
		assert(!p.contains_variable(label));
		assert(!p.contains_function(label));
	}

	size_t var_count = p.variable_count();
	p.bind_variables(VAR_LIST);
	assert_throws(p.bind_variables(VAR_LIST));
	assert(p.variable_count() == var_count + 3);

	double val = 0.0;
	for (const std::string& label : VAR_LIST)
	{
		assert(p.contains(label));
		assert(p.contains_variable(label));
		assert(!p.contains_function(label));
		assert(!p.get_function(label));
		assert(p.get_variable_ptr(label));
		assert(p.get_variable_val<double>(label) == val);
		val += 1.0;
	}

	// testing with multiple existing functions

	for (const std::string& label : FUNC_LIST)
	{
		assert(!p.contains(label));
		assert(!p.contains_variable(label));
		assert(!p.contains_function(label));
	}

	size_t func_count = p.function_count();
	p.bind_functions(FUNC_LIST);
	assert_throws(p.bind_functions(FUNC_LIST));
	assert(p.function_count() == func_count + 3);

	int i = 1;
	for (const std::string& label : FUNC_LIST)
	{
		assert(p.contains(label));
		assert(p.contains_function(label));
		assert(!p.contains_variable(label));
		assert(p.get_function(label));
		assert(!p.get_variable_ptr(label));
	}

	int(*f)() = (decltype(f))p.get_function(GET_FUNC);
	assert(f() == 89);

	p.clear_symbols();
	assert(p.count() == 0);
	assert(p.function_count() == 0);
	assert(p.variable_count() == 0);

	puts("All tests passed");
	return 0;
}
