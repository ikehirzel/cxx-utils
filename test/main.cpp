#include <iostream>
#include <hirzel/logger.h>
#include "other.h"
using namespace hirzel;

void test_methods(const Logger& log)
{
	std::cout << "Testing methods...\n";
	log.info("Hello");
	std::cout << "\tAll tests passed\n\n";
}

int main()
{
	Logger::init_log_file("./log.txt");
	Logger::enable_color(true);

	Logger::print("hello, {} A.K.A. {1}\n", { "Isaac", "Ike" });
	Logger::println("hello, {} A.K.A. {1}", { "Isaac", "Ike" });

	Logger log("Main");
	test_methods(log);

	std::string m = "message!";
	log.print("Logging in main.cpp\n");
	log.debug("This is a debug {}", { m });
	log.info("This is an info {}", { m });
	log.success("This is a success {}", { m });
	log.warning("This is a warning {}", { m });
	log.error("This is an error {}", { m });
	log.fatal("This is a fatal {}", { m });
	logop();

	return 0;
}