#include <iostream>
#include <hirzel/logger.h>
#include "other.h"
using namespace hirzel;

int main()
{
	logger::init();
	std::string m = "message!";
	PRINT("Logging in main.cpp\n");
	INFO("This is an info %s", m);
	SUCCESS("This is a success %s", m);
	WARNING("This is a warning %s", m);
	ERROR("This is an error %s", m);
	FATAL("This is a fatal %s", m );
	logop();
	return 0;
}