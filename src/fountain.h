#pragma once

#include <string>
#include <vector>

#define FOUNTAIN_INFO		0
#define FOUNTAIN_SUCCESS	1
#define FOUNTAIN_WARNING	2
#define FOUNTAIN_ERROR		3
#define FOUNTAIN_FATAL		4

namespace hirzel
{	
	void log_init(const char* _logfilename);
	void log_push(int level, const char* name, int line, const char* str, ...);
	void log_dump();
	
}

#define log_info(msg) hirzel::log_push(FOUNTAIN_INFO, __FILE__, __LINE__, msg, NULL)
#define log_infof(msg, ...) hirzel::log_push(FOUNTAIN_INFO, __FILE__, __LINE__, msg, __VA_ARGS__)

#define log_success(msg) hirzel::log_push(FOUNTAIN_SUCCESS, __FILE__, __LINE__, msg, NULL)
#define log_successf(msg, ...) hirzel::log_push(FOUNTAIN_SUCCESS, __FILE__, __LINE__, msg, __VA_ARGS__)

#define log_warning(msg) hirzel::log_push(FOUNTAIN_WARNING, __FILE__, __LINE__, msg, NULL)
#define log_warningf(msg, ...) hirzel::log_push(FOUNTAIN_WARNING, __FILE__, __LINE__, msg, __VA_ARGS__)

#define log_error(msg) hirzel::log_push(FOUNTAIN_ERROR, __FILE__, __LINE__, msg, NULL)
#define log_errorf(msg, ...) hirzel::log_push(FOUNTAIN_ERROR, __FILE__, __LINE__, msg, __VA_ARGS__)

#define log_fatal(msg) hirzel::log_push(FOUNTAIN_FATAL, __FILE__, __LINE__, msg, NULL)
#define log_fatalf(msg, ...) hirzel::log_push(FOUNTAIN_FATAL, __FILE__, __LINE__, msg, __VA_ARGS__)
