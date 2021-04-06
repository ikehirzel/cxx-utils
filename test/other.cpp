#include "other.h"

#include <hirzel/logger.h>
#include <string>

void logop()
{
	std::string m = "message!";
	PRINT("Logging in other.cpp\n");
	INFO("This is an info %s", m);
	SUCCESS("This is a success %s", m);
	WARNING("This is a warning %s", m);
	ERROR("This is an error %s", m);
	FATAL("This is a fatal %s", m);
}
