#include <iostream>
#define HIRZEL_FOUNTAIN_IMPLEMENTATION
#include <hirzel/fountain.h>
#include "other.h"
using namespace hirzel;

int main()
{
	fountain::init();
	std::string m = "message!";
	PRINT("Logging in main.cpp");
	INFO("This is an info %s", m);
	SUCCESS("This is a success %s", m);
	WARNING("This is a warning %s", m);
	ERROR("This is an error %s", m);
	FATAL("This is a fatal %s", m );
	logop();
	return 0;
}