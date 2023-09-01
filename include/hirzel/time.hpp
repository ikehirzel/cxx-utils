#ifndef HIRZEL_TIME_HPP
#define HIRZEL_TIME_HPP

#include <hirzel/primitives.hpp>

namespace hirzel::time
{
	u64 secondsSinceEpoch();
	u64 millisecondsSinceEpoch();
	u64 microsecondsSinceEpoch();
	void sleepForSeconds(u64 seconds);
	void sleepForMilliseconds(u64 milliseconds);
	void sleepForMicroseconds(u64 microseconds);
}

#endif
