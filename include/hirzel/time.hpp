#ifndef HIRZEL_TIME_HPP
#define HIRZEL_TIME_HPP

#include <hirzel/primitives.hpp>

namespace hirzel::time
{
	primitives::u64 secondsSinceEpoch();
	primitives::u64 millisecondsSinceEpoch();
	primitives::u64 microsecondsSinceEpoch();
	void sleepForSeconds(u64 seconds);
	void sleepForMilliseconds(u64 milliseconds);
	void sleepForMicroseconds(u64 microseconds);
}

#endif
