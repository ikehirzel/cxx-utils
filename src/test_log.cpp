#include "hirzel/log.hpp"

using namespace hirzel::log;

int main()
{
	debug("This is a debug message.");
	info("This is an info log.");
	warning("This is a warning.");
	error("This is an error.");
	fatal("This is a fatal error.");
}
