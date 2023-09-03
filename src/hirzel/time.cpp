#include <hirzel/time.hpp>
#include <chrono>
#include <thread>

namespace hirzel::time
{
	uint64_t secondsSinceEpoch()
	{
		return std::chrono::duration_cast<std::chrono::seconds>
			(std::chrono::system_clock::now().time_since_epoch()).count();
	}

	uint64_t millisecondsSinceEpoch()
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>
			(std::chrono::system_clock::now().time_since_epoch()).count();
	}

	uint64_t microsecondsSinceEpoch()
	{
		return std::chrono::duration_cast<std::chrono::microseconds>
			(std::chrono::system_clock::now().time_since_epoch()).count();
	}

	void sleepForSeconds(uint64_t seconds)
	{
		std::this_thread::sleep_for(std::chrono::seconds(seconds));
	}

	void sleepForMilliseconds(uint64_t milliseconds)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
	}

	void sleepForMicroseconds(uint64_t microseconds)
	{
		std::this_thread::sleep_for(std::chrono::microseconds(microseconds));
	}
}
