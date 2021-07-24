#include <iostream>
#include <hirzel/logger.h>
#include "other.h"
using namespace hirzel;

class Object
{
private:
	static Logger log;
	int val = 0;
public:
	void do_thing()
	{
		val += 1;
		log.info("hello");
	}
};

Logger Object::log("Object");

void test_methods(const Logger& log)
{
	std::cout << "Testing methods...\n";
	log.info("Hello");
	std::cout << "\tAll tests passed\n\n";
}

int main()
{
	Logger::init();
	// Object o;
	// o.do_thing();

	Logger log("Main");
	test_methods(log);

	std::string m = "message!";
	log.print("Logging in main.cpp\n");
	log.debug("This is a debug %s", { m });
	log.info("This is an info %s", { m });
	log.success("This is a success %s", { m });
	log.warning("This is a warning %s", { m });
	log.error("This is an error %s", { m });
	log.fatal("This is a fatal %s", { m });
	logop();

	return 0;
}