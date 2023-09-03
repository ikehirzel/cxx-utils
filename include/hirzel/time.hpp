#ifndef HIRZEL_TIME_HPP
#define HIRZEL_TIME_HPP

#include <cstdint>

namespace hirzel::time
{
	uint64_t secondsSinceEpoch();
	uint64_t millisecondsSinceEpoch();
	uint64_t microsecondsSinceEpoch();
	void sleepForSeconds(uint64_t seconds);
	void sleepForMilliseconds(uint64_t milliseconds);
	void sleepForMicroseconds(uint64_t microseconds);
}

#endif
