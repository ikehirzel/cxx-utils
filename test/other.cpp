#include "other.h"

#include <hirzel/logger.h>
#include <string>

void logop()
{
	using hirzel::Logger;

	std::string m = "message!";

	Logger::print("Logging in other.cpp\n");
	Logger::log_debug(__func__, "This is a debug {}", { m });
	Logger::log_info(__func__, "This is an info {}", { m });
	Logger::log_success(__func__, "This is a success {}", { m });
	Logger::log_warning(__func__, "This is a warning {}", { m });
	Logger::log_error(__func__, "This is an error {}", { m });
	Logger::log_fatal(__func__, "This is a fatal {}", { m });
}
