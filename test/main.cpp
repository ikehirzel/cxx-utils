#include <iostream>
#include <hirzel/logger.h>
#include "other.h"
using namespace hirzel;

int main()
{
	Logger::init();
	Logger log("Main");
	std::string m = "message!";
	log.print("Logging in main.cpp\n");
	log.info("This is an info %s", { m });
	log.success("This is a success %s", { m });
	log.warning("This is a warning %s", { m });
	log.error("This is an error %s", { m });
	log.fatal("This is a fatal %s", { m });
	logop();
	return 0;
}