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

#define assert_throws(x)\
{\
	bool throws = false;\
	try\
	{\
		x;\
	}\
	catch (const std::exception& e)\
	{\
		throws = true;\
		std::cout << "Expected exception: " << e.what() << std::endl;\
	}\
	assert(throws);\
}

int main()
{
	using hirzel::Plugin;

	// assure empty string gives error
	assert_throws(Plugin(""));
	assert_throws(Plugin(DNE_OBJ));

	auto p = Plugin(OBJ);

	// assert throw when double binding
	assert(p.count() == 0);

	assert(!p.contains(GET_FUNC));
	assert(!p.contains(ADD_FUNC));
	assert(!p.contains(MSG_VAR));
	assert(!p.contains(I_VAR));
	assert(!p.contains(DNE_SYM));


	// testing non-existent variable
	assert_throws(p = Plugin(OBJ, { }, { DNE_SYM }));
	assert_throws(p.get_variable_ptr(DNE_SYM));
	assert_throws(p.get_function_ptr(DNE_SYM));
	assert(!p.contains(DNE_SYM));
	assert(!p.contains_variable(DNE_SYM));
	assert(!p.contains_function(DNE_SYM));
	assert(p.count() == 0);

	// testing non-existent function
	assert_throws(p = Plugin(OBJ, { DNE_SYM }));
	assert_throws(p.get_variable_ptr(DNE_SYM));
	assert_throws(p.get_function_ptr(DNE_SYM));
	assert(!p.contains(DNE_SYM));
	assert(!p.contains_variable(DNE_SYM));
	assert(!p.contains_function(DNE_SYM));
	assert(p.count() == 0);

	// testing existing function
	p = Plugin(OBJ, { GET_FUNC });
	assert(p.count() == 1);
	assert(p.contains(GET_FUNC));
	assert(p.contains_function(GET_FUNC));
	assert(!p.contains_variable(GET_FUNC));
	assert_throws(p.get_variable_ptr(GET_FUNC));
	assert_throws(p.get_variable<int>(GET_FUNC));
	assert(p.get_function_ptr(GET_FUNC) != nullptr);
	p.call_function<int>(GET_FUNC);
	assert(p.call_function<int>(GET_FUNC) == 88);
	
	p = Plugin(OBJ, { ADD_FUNC });
	assert(p.count() == 2);
	assert(p.contains(ADD_FUNC));
	assert(p.contains_function(ADD_FUNC));
	assert(!p.contains_variable(ADD_FUNC));
	assert_throws(p.get_variable<int>(ADD_FUNC));
	assert(p.get_function_ptr(ADD_FUNC) != nullptr);
	assert_throws(p.get_variable_ptr(ADD_FUNC));
	p.call_function<void>(ADD_FUNC);
	assert(p.call_function<int>(GET_FUNC) == 89);
	
	// testing existing global variable
	p = Plugin(OBJ, { }, { MSG_VAR });
	assert(p.count() == 3);
	assert(p.contains(MSG_VAR));
	assert(p.contains_variable(MSG_VAR));
	assert(!p.contains_function(MSG_VAR));
	assert_throws(p.get_function_ptr(MSG_VAR));
	assert_throws(p.call_function<void>(MSG_VAR));
	assert(p.get_variable_ptr(MSG_VAR));
	assert(!strcmp("this is a message", p.get_variable<const char *>(MSG_VAR)));
	
	p = Plugin(OBJ, { GET_FUNC, ADD_FUNC }, { MSG_VAR, I_VAR });
	assert(p.count() == 4);
	assert(p.contains(I_VAR));
	assert(p.contains_variable(I_VAR));
	assert(!p.contains_function(I_VAR));
	assert_throws(p.get_function_ptr(I_VAR));
	assert_throws(p.call_function<void>(I_VAR));
	assert(p.get_variable_ptr(I_VAR));
	assert(p.get_variable<int>(I_VAR) == 89);

	// testing multiple non-existent functions / variables
	assert_throws(p = Plugin(OBJ, DNE_LIST));
	assert_throws(p = Plugin(OBJ, {}, DNE_LIST));

	p = Plugin(OBJ);
	for (const std::string& label : DNE_LIST)
	{
		assert(!p.contains(label));
		assert(!p.contains_function(label));
		assert(!p.contains_variable(label));
		assert_throws(p.get_function_ptr(label));
		assert_throws(p.get_variable_ptr(label));
	}

	// testing with multiple existing variables
	for (const std::string& label : VAR_LIST)
	{
		assert(!p.contains(label));
		assert(!p.contains_variable(label));
		assert(!p.contains_function(label));
	}

	p = Plugin(OBJ, {}, VAR_LIST);
	assert(p.count() == 7);

	double val = 0.0;
	for (const std::string& label : VAR_LIST)
	{
		assert(p.contains(label));
		assert(p.contains_variable(label));
		assert(!p.contains_function(label));
		assert_throws(p.get_function_ptr(label));
		assert(p.get_variable_ptr(label));
		assert(p.get_variable<double>(label) == val);
		val += 1.0;
	}

	// testing with multiple existing functions

	p = Plugin(OBJ);

	for (const std::string& label : FUNC_LIST)
	{
		assert(!p.contains(label));
		assert(!p.contains_variable(label));
		assert(!p.contains_function(label));
	}

	p = Plugin(OBJ, FUNC_LIST);

	for (const std::string& label : FUNC_LIST)
	{
		assert(p.contains(label));
		assert(p.contains_function(label));
		assert(!p.contains_variable(label));
		assert(p.get_function_ptr(label));
		assert_throws(p.get_variable_ptr(label));
	}

	puts("All tests passed");
	return 0;
}
