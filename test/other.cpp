#include "other.h"

#include <hirzel/logger.h>
#include <string>

void logop()
{
	using hirzel::Logger;

	std::string m = "message!";

	Logger::print("Logging in other.cpp\n");
	Logger::info(__func__, "This is an info %s", { m });
	Logger::success(__func__, "This is a success %s", { m });
	Logger::warning(__func__, "This is a warning %s", { m });
	Logger::error(__func__, "This is an error %s", { m });
	Logger::fatal(__func__, "This is a fatal %s", { m });
}
