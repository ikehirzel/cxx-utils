#pragma once

#include <string>
#include <vector>

namespace hirzel
{
	struct Log
	{
		unsigned int level = 0;
		std::string classname = "";
		std::string msg = "";
		std::string timestamp = "";

		Log(unsigned int level, const std::string& classname, const std::string& msg);
		std::string content();
		void print();
	};

	void log_init(const std::string& _logfilename);
	void log_set_name(const std::string& _loggername);

	void log_info(const std::string& msg);
	void log_success(const std::string& msg);
	void log_warning(const std::string& msg);
	void log_error(const std::string& msg);
	void log_fatal(const std::string& msg);

	void log_dump();
}
